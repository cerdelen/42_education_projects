#include <Task.hpp>

Task::Task(CommandConfig& conf) : _conf(conf) {}
Task::Task(const Task& copy)
    : _conf(copy._conf) {
    *this = copy;
}
Task& Task::operator=(const Task& copy) {
    (void)copy;
    return (*this);
}
Task::~Task() {}
