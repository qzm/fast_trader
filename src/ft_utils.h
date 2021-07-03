#pragma once
#ifndef __FT_UTILS_H__
#define __FT_UTILS_H__
#include "json.hpp"
#include "readerwriterqueue.h"
#include "spdlog/spdlog.h"
#include <chrono>
#include <functional>
#include <thread>

using json = nlohmann::json;

template <typename Type>
using Queue = moodycamel::BlockingReaderWriterQueue<Type>;

// 等待，直到callback返回真值
bool wait_until(std::function<bool()> callback, int interval, int timeout)
{
    int counter = 0;
    while (!callback()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        counter += interval;
        if (counter >= timeout) {
            return false;
        }
    }
    return true;
}
#endif