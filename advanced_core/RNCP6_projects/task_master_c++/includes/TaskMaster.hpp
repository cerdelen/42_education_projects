#ifndef TASKMASTER_DEFINED
#define TASKMASTER_DEFINED

#include <CommandConfig.hpp>
#include <Task.hpp>
#include <UI.hpp>
#include <vector>
#include <unistd.h>
#include <iostream>

#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

// #define ANNOUNCE_ME std::cout << __FUNCTION__ << std::endl;
#define LOGGING _logger << __FUNCTION__ << "\n";

class TaskMaster {
  public:
    TaskMaster(int argc, char* argv[]);
    TaskMaster(const TaskMaster& copy);
    TaskMaster& operator=(const TaskMaster& copy);
    ~TaskMaster();

    void reload_taskmaster(char* config_file);
    void unser_input_loop();
    void quit();
    void reload();
    void kill();
    void cmd_j();
    void cmd_k();
    void select();
    void print();
    void deselect();

  private:
    std::ofstream                 _logger;
    UI                  _ui;
    // CommandConfig	 				command_config;
    std::vector<Task>   _tasks;
};

#endif // !TaskMaster