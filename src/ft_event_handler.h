#pragma once
#ifndef __FT_EVENT_HANDLER_H__
#define __FT_EVENT_HANDLER_H__

class EventHandler {
private:
    json config;
    std::atomic<int> request_seq(0);

    CThostFtdcMdApi* quote_api;
    CTacFtdcTraderApi* trader_api;

    QuoteSpi* quote_spi;
    TraderSpi* trader_spi;

public:
    std::vector<StrategyManager*> strategy_manager_vector;

    Queue<std::shared_ptr<StrategyEvent>> strategy_event_queue;
    Queue<std::shared_ptr<FrameworkEvent>> framework_event_queue;

    std::thread strategy_event_thread;
    std::thread framework_event_thread;

    void strategy_event_process();
    void framework_event_process();

public:
    EventHandler(std::string& filename)
        : filename(filename) {};
    bool load_config();
    bool load_strategies();

    bool start_trader();
    bool start_quote();
    bool start_threads();
    bool start_strategies();
};

#endif