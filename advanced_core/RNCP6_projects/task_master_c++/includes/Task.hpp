#ifndef TASK_DEFINED
#define TASK_DEFINED

#include <CommandConfig.hpp>
#include <SingleProcess.hpp>
#include <vector>

class Task {
  public:
    Task(CommandConfig& conf);
    Task(const Task& copy);
    Task& operator=(const Task& copy);
    ~Task();

  private:
    CommandConfig&                _conf;
    std::vector<SingleProcess>    _processes;
};

#endif // !Task
