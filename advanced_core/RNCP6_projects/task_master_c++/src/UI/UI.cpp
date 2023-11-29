#include <UI.hpp>

UI::UI() {}
UI::UI(const UI& copy) { *this = copy; }
UI& UI::operator=(const UI& copy) {
    (void)copy;
    return (*this);
}
UI::~UI() {}
