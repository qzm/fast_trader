#pragma once
#ifndef __FT_EVENT_H__
#define __FT_EVENT_H__

#include <memory>

#define STRATEGY_EVENT_InsertOrder     'A' // 报单插入
#define STRATEGY_EVENT_CancelOrder     'B' // 撤单插入
#define STRATEGY_EVENT_RtnOrder        'C' // 报单回报
#define STRATEGY_EVENT_RtnTrade        'D' // 成交回报
#define STRATEGY_EVENT_RtnInstrument   'E' // 合约查询应答
#define STRATEGY_EVENT_RtnAccount      'F' // 资金查询应答
#define STRATEGY_EVENT_RtnPosition     'G' // 持仓查询应答
#define STRATEGY_EVENT_DepthMarketData 'H' // 深度行情通知

#define FRAMEWORK_EVENT_InsertOrder   'a' // 下单
#define FRAMEWORK_EVENT_CancelOrder   'b' // 撤单
#define FRAMEWORK_EVENT_QryInstrument 'c' // 合约查询请求
#define FRAMEWORK_EVENT_QryAccount    'd' // 资金查询请求
#define FRAMEWORK_EVENT_QryPosition   'e' // 持仓查询请求

typedef char EventType;

// Event事件
struct Event {
    EventType type;
    std::shared_ptr<void> event_ptr;
    Event()
    {
        memset(this, 0, sizeof(*this));
    }
};

#endif