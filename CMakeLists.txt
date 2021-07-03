cmake_minimum_required(VERSION 3.0.0)
project(fast_trader VERSION 0.1.0)

include(CTest)
enable_testing()


# Library
include_directories(${CMAKE_SOURCE_DIR}/lib)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/spdlog)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/cxxopts)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/readerwriterqueue)
link_libraries(pthread spdlog cxxopts readerwriterqueue ${CMAKE_DL_LIBS})

# Add CTP
include_directories(${CMAKE_SOURCE_DIR}/lib/ctp/v6.6.1_P1/include)
link_directories(${CMAKE_SOURCE_DIR}/lib/ctp/v6.6.1_P1/lib)

add_executable(fast_trader
    src/main.cpp
)
target_link_libraries(
    fast_trader
    thostmduserapi
    thosttraderapi
)

install(
    FILES
    "libs/ctp/v6.6.1_P1/lib/libthostmduserapi.so"
    "libs/ctp/v6.6.1_P1/lib/libthosttraderapi.so"
    COMPONENT libraries
    DESTINATION "lib"
)

install(
    TARGETS fast_trader
    COMPONENT applications
    DESTINATION "bin"
)

install(
    FILES
    "ft_strategy.h"
    "ft_structures.h"
    "lib/json.hpp"
    "lib/ctp/v6.6.1_P1/include/ThostFtdcMdApi.h"
    "lib/ctp/v6.6.1_P1/include/ThostFtdcUserApiStruct.h"
    "lib/ctp/v6.6.1_P1/include/ThostFtdcUserApiDataType.h"
    DESTINATION "include"
    COMPONENT headers
)

SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "FastTrader")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
