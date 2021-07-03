#pragma once
#ifndef __FT_EVENT_H__
#define __FT_EVENT_H__

#include <memory>

// 策略传入信息数据
#define STRATEGY_EVENT_InsertOrder     'A' // 报单插入
#define STRATEGY_EVENT_CancelOrder     'B' // 撤单插入
#define STRATEGY_EVENT_RtnOrder        'C' // 报单回报
#define STRATEGY_EVENT_RtnTrade        'D' // 成交回报
#define STRATEGY_EVENT_RtnInstrument   'E' // 合约查询应答
#define STRATEGY_EVENT_RtnAccount      'F' // 资金查询应答
#define STRATEGY_EVENT_RtnPosition     'G' // 持仓查询应答
#define STRATEGY_EVENT_DepthMarketData 'H' // 深度行情通知
typedef char StrategyEventType;

// 策略传出信息数据
#define FRAMEWORK_EVENT_InsertOrder   'A' // 下单
#define FRAMEWORK_EVENT_CancelOrder   'B' // 撤单
#define FRAMEWORK_EVENT_QryInstrument 'C' // 合约查询请求
#define FRAMEWORK_EVENT_QryAccount    'D' // 资金查询请求
#define FRAMEWORK_EVENT_QryPosition   'E' // 持仓查询请求
typedef char FrameworkEventType;

// 策略执行任务
struct StrategyEvent {
    StrategyEventType type;
    std::shared_ptr<void> event_ptr;

    StrategyEvent()
    {
        memset(this, 0, sizeof(*this));
    }
};

// 框架执行任务
struct FrameworkEvent {
    FrameworkEventType type;
    std::shared_ptr<void> event_ptr;
    FrameworkEvent()
    {
        memset(this, 0, sizeof(*this));
    }
};

#endif