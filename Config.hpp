// Config.hpp
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include "Expected.hpp"

enum ConfigError
{
    ERR_EMPTY_CONFIG,
    ERR_MISSING_SERVER_BLOCK,
    ERR_UNCLOSED_BRACE,
    ERR_UNKNOWN_DIRECTIVE,
    ERR_INVALID_PORT,
    ERR_MISSING_SEMICOLON
};

struct ServerConfig
{
    int         port;
    std::string server_name;
    std::string root;

    // Default values if omitted in config
    ServerConfig() : port(8080), server_name("localhost"), root("./www") {}
};

Expected<ServerConfig, ConfigError> parse_config_file(const std::string& raw);

#endif
