#include "cxxopts.hpp"
#include "ft_event_handler.h"
#include "ft_utils.h"
#include <iostream>

int main(int argc, char const* argv[])
{
    cxxopts::Options options("Fast Trader", "Light weight hft framework.");
    options.add_options()
        // help
        ("h,help", "Print usage.")
        // config
        ("c,config", "Config file name. Ex: fast_trader -c config.json", cxxopts::value<std::string>());

    auto result = options.parse(argc, argv);
    if (result.count("help") || result.count("config") == 0) {
        std::cout << options.help() << std::endl;
        exit(0);
    }
    std::string filename = result["config"].as<std::string>();

    auto handler = EventHandler(filename);
    spdlog::info("Compiler: {} Date: {} Time: {}", __VERSION__, __DATE__, __TIME__);

    handler.load_config();
    handler.start_threads();
    handler.start_trader();
    handler.start_quote();
    handler.load_strategies();
    handler.start_strategies();

    char i;
    while (true) {
        std::cin >> i;
        std::cout << i << std::endl;
    }
}
