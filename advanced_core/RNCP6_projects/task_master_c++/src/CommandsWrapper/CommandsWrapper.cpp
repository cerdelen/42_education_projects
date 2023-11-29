#include <CommandsWrapper.hpp>

CommandsWrapper::CommandsWrapper() {}
CommandsWrapper::CommandsWrapper(const CommandsWrapper& copy) { *this = copy; }
CommandsWrapper& CommandsWrapper::operator=(const CommandsWrapper& copy) {
    (void)copy;
    return (*this);
}
CommandsWrapper::~CommandsWrapper() {}
