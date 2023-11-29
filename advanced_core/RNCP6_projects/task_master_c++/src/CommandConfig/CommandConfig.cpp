#include <CommandConfig.hpp>

CommandConfig::CommandConfig(std::string conf) { this->raw_conf = conf; }
bool CommandConfig::same_config(const CommandConfig& compare) {
    return (compare.raw_conf == this->raw_conf);
}
CommandConfig::CommandConfig(const CommandConfig& copy) { *this = copy; }
CommandConfig& CommandConfig::operator=(const CommandConfig& copy) {
    this->raw_conf = copy.raw_conf;
    this->cmd = copy.cmd;
    this->envs = copy.envs;
    this->amt = copy.amt;
    this->time_of_launch = copy.time_of_launch;
    this->restart_tries = copy.restart_tries;
    this->restart = copy.restart;
    this->args = copy.args;
    this->unexpected_signals = copy.unexpected_signals;
    this->gracefull_signal = copy.gracefull_signal;
    this->gracefull_period = copy.gracefull_period;
    this->stderr_fd = copy.stderr_fd;
    this->stdout_fd = copy.stdout_fd;
    this->work_dir = copy.work_dir;
    this->name = copy.name;
    return (*this);
}
CommandConfig::~CommandConfig() {}
