#ifndef Global_DEFINED
#define Global_DEFINED

#include <CommandConfig.hpp>
#include <Task.hpp>
#include <UI.hpp>
#include <vector>
class Global {
  public:
    Global();
    Global(const Global& copy);
    Global& operator=(const Global& copy);
    ~Global();

    void reload_taskmaster(char* config_file);

  private:
    UI ui;
    // CommandConfig					command_config;
    std::vector<Task> current_tasks;
};

#endif // !Global
