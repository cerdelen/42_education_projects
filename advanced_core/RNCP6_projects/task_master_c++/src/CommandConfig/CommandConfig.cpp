#include <CommandConfig.hpp>

CommandConfig::CommandConfig() {}
CommandConfig::CommandConfig(const CommandConfig& copy) { *this = copy; }
CommandConfig& CommandConfig::operator=(const CommandConfig& copy) {
    (void)copy;
    return (*this);
}
CommandConfig::~CommandConfig() {}
