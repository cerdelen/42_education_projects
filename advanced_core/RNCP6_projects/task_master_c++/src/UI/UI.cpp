#include <UI.hpp>
UI::UI() {
    _ui_idx = 0;
    this->_mode = ViewMode::OverView;
    this->_mode_render = "OverView";
    // this->_middle_part;
    this->_bottom_part = "Commands: Quit:q Down:j Up:k Reload:r Select:s Deselect:d Kill:t\nYour Command: ";
}
UI::UI(const UI& copy) { *this = copy; }
UI& UI::operator=(const UI& copy) {
    (void)copy;
    return (*this);
}
UI::~UI() {
    std::cout << "Deconstructor UI" << std::endl;
}
void	UI::cmd_j() {
    std::cout << "Called cmd_j" << std::endl;
};
void	UI::cmd_k() {
    std::cout << "Called cmd_k" << std::endl;
};
void	UI::select() {
    std::cout << "Called select" << std::endl;
}
void	UI::print() {
    std::cout << "Called print" << std::endl;
}
void	UI::deselect() {
    std::cout << "Called deselect" << std::endl;
}
void	UI::render() {
    std::cout << "\033[2J\033[H" << _mode_render << "\n" << _middle_part << "\n" << _bottom_part;
}
size_t	UI::get_idx() {
    return (_ui_idx);
}
void UI::set_idx(int change) {
    // check if updated value possible
    _ui_idx += change;
}
