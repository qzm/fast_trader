#pragma once
#ifndef __FT_QUOTE_SPI_H__
#define __FT_QUOTE_SPI_H__

#include "ThostFtdcMdApi.h"

class QuoteSpi : public CThostFtdcMdSpi {
public:
    json& config;
    std::atomic<int> request_seq(0);
    bool is_ready = false;

    CThostFtdcMdApi* quote_api;
    Queue<std::shared_ptr<Event>>& strategy_event_queue;

    std::string user_id;
    std::string password;
    std::string broker_id;
    std::string front_addr;

public:
    QuoteSpi(json& config, CThostFtdcMdApi* quote_api, Queue<std::shared_ptr<Event>>& strategy_event_queue)
        : config(config)
        , quote_api(quote_api)
        , strategy_event_queue(strategy_event_queue)
    {
        user_id = config["quote"]["user_id"].get<std::string>();
        password = config["quote"]["password"].get<std::string>();
        broker_id = config["quote"]["broker_id"].get<std::string>();
        front_addr = config["quote"]["front_addr"].get<std::string>();
    };
    void OnFrontConnected();
    void OnFrontDisconnected(int nReason);
    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData);
    void OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
};

#endif