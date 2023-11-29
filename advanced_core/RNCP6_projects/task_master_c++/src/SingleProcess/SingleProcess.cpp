#include <SingleProcess.hpp>

SingleProcess::SingleProcess() {}
SingleProcess::SingleProcess(const SingleProcess& copy) { *this = copy; }
SingleProcess& SingleProcess::operator=(const SingleProcess& copy) {
    (void)copy;
    return (*this);
}
SingleProcess::~SingleProcess() {}
