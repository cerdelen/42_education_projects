#ifndef CommandConfig_DEFINED
#define CommandConfig_DEFINED

#include <map>
#include <string>
#include <vector>

class CommandConfig {
  public:
    CommandConfig(std::string conf);
    CommandConfig(const CommandConfig& copy);
    CommandConfig& operator=(const CommandConfig& copy);
    bool same_config(const CommandConfig& copy);
    ~CommandConfig();

  private:
    std::string raw_conf;
    std::string cmd;               // cmd to be launched
    std::vector<std::string> args; // arguments for cmd
    size_t amt;            // How many processes to be spawned of this cmd
    size_t time_of_launch; // what is c++ time type?
    int restart;           //     Always, Never, OnUnexpectedExits
    std::vector<int> unexpected_signals; // vec of signals to be interpreted as
                                         // unexpected signals
    size_t restart_tries; // if to be restarted how many tries until we stop to
                          // restart
    std::vector<int> gracefull_signal; // vec of signals to be interpreted as
                                       // gracefull signals
    size_t gracefull_period;           // what is c++ time type?
    int stdout_fd;                     // init to def if not givven
    int stderr_fd;                     // init to def if not givven
    std::map<std::string, std::string> envs; // optional enviornment for command
    std::string work_dir;
    std::string name; // optional name for this CommandC onfiguration
    // An umask to set before launching the programm??
};

#endif // !CommandConfig
