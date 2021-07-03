#pragma once
#ifndef __FT_STRATEGY_MANAGER_H__
#define __FT_STRATEGY_MANAGER_H__

#include "ft_strategy.h"
#include "ft_utils.h"
#include <pthread.h>

class StrategyManager {
private:
    std::string client_id;

public:
    json& config;

    StrategyManager(json& config, Strategy* strategy, std::string client_id, Queue<std::shared_ptr<FrameworkTask>>& framework_task_queue);
    int bind_cpu = 0;
    Strategy* strategy;
    Queue<std::shared_ptr<StrategyTask>> strategy_task_queue;
    Queue<std::shared_ptr<FrameworkTask>>& framework_task_queue;
    std::thread strategy_task_thread;
    void strategy_task_process();
    // 返回数据给框架
    void ACCOUNT();
    void POSITION(const char* instrument_id);
    void INSTRUMENT(const char* instrument_id, char exchange_id);
    void CANCEL(uint64_t ref_id);
    uint64_t BUY(const char* instrument_id, char exchange_id, double price, int volume, char offset = OPEN, char order_type = LIMIT);
    uint64_t SELL(const char* instrument_id, char exchange_id, double price, int volume, char offset = CLOSE_TODAY, char order_type = LIMIT);
    uint64_t SHORT(const char* instrument_id, char exchange_id, double price, int volume, char offset = OPEN, char order_type = LIMIT);
    uint64_t COVER(const char* instrument_id, char exchange_id, double price, int volume, char offset = CLOSE_TODAY, char order_type = LIMIT);
};
#endif