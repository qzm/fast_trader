
#include "ft_event_handler.h"
#include "ft_strategy.h"
#include "ft_strategy_manager.h"
#include "ft_utils.h"

// 加载配置
bool EventHandler::load_config()
{
    try {
        std::ifstream in(filename);
        in >> config;
        spdlog::info("Load config file successed!");
        return true;
    } catch (const std::exception& e) {
        spdlog::error(e.what());
        return false;
    }
};

// 加载策略
bool EventHandler::load_strategies()
{
    void* strategy_handle;

    for (auto& item : config["strategies"]) {
        int library_id = item["id"].get<int>();
        std::string library = item["library"].get<std::string>();

        strategy_handle = dlopen(library.c_str(), RTLD_NOW);
        if (!strategy_handle) {
            spdlog::error("Load {} failed!", library);
            continue;
        }

        dlerror();

        funcCreateStrategy createStrategy = (funcCreateStrategy)dlsym(strategy_handle, CREATE_STRATEGY_NAME);
        if (!createStrategy) {
            spdlog::error("Get function addresss({}) failed!", CREATE_STRATEGY_NAME);
            continue;
        }

        auto destoryStrategy = (funcDestoryStrategy)dlsym(strategy_handle, DESTORY_STRATEGY_NAME);
        if (!destoryStrategy) {
            spdlog::error("Get function addresss({}) failed!", DESTORY_STRATEGY_NAME);
            continue;
        }

        Strategy* strategy = createStrategy();
        if (!strategy) {
            spdlog::error("Create strategy object failed!");
            continue;
        } else {
            spdlog::info("Create strategy object successed!");
        }
        try {
            std::string user_id = config["trade"]["user_id"].get<std::string>().c_str();
            StrategyManager* sm = new StrategyManager(item["params"], strategy, user_id, framework_event_queue);
            strategy_manager_vector.push_back(sm);
            spdlog::info("Strategy {} loaded!", library);
        } catch (const std::exception& e) {
            spdlog::error(e.what());
        }
    }
    return true;
};

// 开始交易
bool EventHandler::start_trader()
{
    trader_api = CThostFtdcTraderApi::CreateFtdcTraderApi();
    if (trader_api == nullptr) {
        spdlog::error("Create CThostFtdcTraderApi failed!");
        return false;
    }
    spdlog::info("Trader API version: {}", trader_api->GetApiVersion());
    trader_spi = new TraderSpi(config, request_seq, trader_api, strategy_event_queue);
    if (trader_spi == nullptr) {
        spdlog::error("Create TraderSpi failed!");
        return false;
    }
    std::string front_addr = config["trade"]["front_addr"].get<std::string>();

    trader_api->RegisterSpi(trader_spi);
    trader_api->RegisterFront(front_addr.c_str());
    trader_api->SubscribePrivateTopic(TAC_TERT_RESUME);
    trader_api->SetLogFilePath(LOG_FILE_PATH);
    trader_api->SetLogLevel(true);
    trader_api->Init();
    return wait_until([&] { return trader_spi->is_ready; }, 100, 30 * 1000);
};

// 开始行情
bool EventHandler::start_quote()
{
    quote_api = CThostFtdcMdApi::CreateFtdcMdApi();
    if (quote_api == nullptr) {
        spdlog::error("Create CThostFtdcMdApi failed!");
        return false;
    }
    spdlog::info("Quote API version: {}", quote_api->GetApiVersion());
    quote_spi = new QuoteSpi(config, request_seq, quote_api, quote_event_queue);
    if (quote_spi == nullptr) {
        spdlog::error("Create QuoteSpi failed!");
        return false;
    }
    std::string front_addr = config["quote"]["front_addr"].get<std::string>();

    quote_api->RegisterSpi(quote_spi);
    quote_api->RegisterFront((char*)front_addr.c_str());
    quote_api->Init();
    return wait_until([&] { return quote_spi->is_ready; }, 100, 30 * 1000);
};

// 开始策略
bool EventHandler::start_strategies()
{
    for (auto* it : strategy_manager_vector) {
        try {
            it->strategy->on_start();
        } catch (const std::exception& e) {
            spdlog::error(e.what());
            return false;
        }
    }
    return true;
};

// 启动线程
bool EventHandler::start_threads()
{
    try {
        strategy_event_thread = std::thread(&EventHandler::strategy_event_process, this);
        strategy_event_thread.detach();
        spdlog::info("Start strategy event thread successed.");
    } catch (const std::exception& e) {
        spdlog::error(e.what());
        return false;
    }

    try {
        framework_event_thread = std::thread(&EventHandler::framework_event_process, this);
        framework_event_thread.detach();
        spdlog::info("Start framework event thread successed.");
    } catch (const std::exception& e) {
        spdlog::error(e.what());
        return false;
    }
    return true;
};

// 执行策略事件
void EventHandler::strategy_event_process()
{
    std::shared_ptr<Event> strategy_event;
    while (true) {
        if (strategy_event_queue.wait_dequeue(strategy_event)) {
            for (auto* sm : strategy_manager_vector) {
                sm->strategy_event_queue.enqueue(strategy_event);
            }
        }
    }
}

// 执行框架事件
void EventHandler::framework_event_process()
{
    std::shared_ptr<Event> framework_event;
    while (true) {
        if (framework_event_queue.wait_dequeue(framework_event)) {
            auto event = framework_event.get();
            void* event_ptr = event->event_ptr.get();
            try {
                switch (event->type) {
                case FRAMEWORK_EVENT_InsertOrder:
                    trader_api->ReqOrderInsert((CThostFtdcInputOrderField*)event_ptr, request_seq++);
                    break;
                case FRAMEWORK_EVENT_CancelOrder:
                    trader_api->ReqOrderAction((CThostFtdcInputOrderActionField*)event_ptr, request_seq++);
                    break;
                case FRAMEWORK_EVENT_QryInstrument:
                    trader_api->ReqQryInstrument((CThostFtdcQryInstrumentField*)event_ptr, request_seq++);
                    break;
                case FRAMEWORK_EVENT_QryAccount:
                    trader_api->ReqQryAccount((CThostFtdcQryAccountField*)event_ptr, request_seq++);
                    break;
                case FRAMEWORK_EVENT_QryPosition:
                    trader_api->ReqQryPosition((CThostFtdcQryPositionField*)event_ptr, request_seq++);
                    break;
                default:
                    spdlog::error("Unkonw Framework Event!");
                    break;
                }
            } catch (const std::exception& e) {
                spdlog::error("Framewrok catch error!");
                spdlog::error(e.what());
            }
        }
    }
}