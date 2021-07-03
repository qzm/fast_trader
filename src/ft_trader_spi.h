#pragma once
#ifndef __FT_TRADER_SPI_H__
#define __FT_TRADER_SPI_H__

#include "ThostFtdcTraderApi.h"

class TraderSpi : public CThostFtdcTraderSpi {
public:
    json& config;
    std::atomic<int> request_seq(0);
    bool is_ready = false;

    CThostFtdcTraderApi* trader_api;
    Queue<std::shared_ptr<StrategyEvent>>& strategy_event_queue;

    std::string app_id;
    std::string user_id;
    std::string password;
    std::string auth_code;
    std::string front_addr;

public:
    TraderSpi(json& config, CThostFtdcTraderApi* trader_api, Queue<std::shared_ptr<StrategyEvent>>& strategy_event_queue)
        : config(config)
        , trader_api(trader_api)
        , strategy_event_queue(strategy_event_queue)
    {
        app_id = config["trade"]["app_id"].get<std::string>();
        user_id = config["trade"]["user_id"].get<std::string>();
        password = config["trade"]["password"].get<std::string>();
        auth_code = config["trade"]["auth_code"].get<std::string>();
        front_addr = config["trade"]["front_addr"].get<std::string>();
    };
    void OnFrontConnected();
    void OnFrontDisconnected(int nReason);
    void OnRtnOrder(CThostFtdcOrderField* pOrder);
    void OnRtnTrade(CThostFtdcRtnTradeField* pTrade);
    void OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    void OnRspQryAccount(CThostFtdcAccountField* pAccount, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    void OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    void OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    void OnRspOrderInsert(CThostFtdcRspOrderInsertField* pRspOrderInsert, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    void OnRspOrderAction(CThostFtdcRspOrderActionField* pRspOrderAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    void OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticate, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField* pPosition, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
};

#endif