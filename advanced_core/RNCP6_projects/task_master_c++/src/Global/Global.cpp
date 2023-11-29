#include <Global.hpp>

Global::Global() {}
Global::Global(const Global& copy) { *this = copy; }
Global& Global::operator=(const Global& copy) {
    (void)copy;
    return (*this);
}
Global::~Global() {}
