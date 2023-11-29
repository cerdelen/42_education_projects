#include <ChildProcess.hpp>

ChildProcess::ChildProcess(const CommandConfig& conf) {
    (void) conf;
}
ChildProcess::ChildProcess(const ChildProcess& copy) { *this = copy; }
ChildProcess& ChildProcess::operator=(const ChildProcess& copy) {
    (void)copy;
    return (*this);
}
ChildProcess::~ChildProcess() {}
