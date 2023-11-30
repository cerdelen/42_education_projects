#include "SingleProcess.hpp"
#include "Task.hpp"
#include <UI.hpp>
#include <vector>
UI::UI() {

    this->_state.idx = 0;
    this->_state.mode = ViewMode::OverView;
    this->_state.pointed_at = NULL;
    this->_mode_render = "OverView";
    // this->_middle_part;
    this->_bottom_part = "Commands: Quit:q Down:j Up:k Reload:r Select:s "
                         "Deselect:d Kill:t\nYour Command: ";
}
UI::UI(const UI& copy) { *this = copy; }
UI& UI::operator=(const UI& copy) {
    (void)copy;
    return (*this);
}
UI::~UI() { std::cout << "Deconstructor UI" << std::endl; }
void UI::cmd_j() {
    std::cout << "Called cmd_j" << std::endl;
    if (this->_state.idx + 1 < this->get_size_of_state())
        this->_state.idx += 1;
};
void UI::cmd_k() {
    std::cout << "Called cmd_k" << std::endl;
    if (this->_state.idx > 0)
        this->_state.idx -= 1;
};
void UI::select() { std::cout << "Called select" << std::endl; }
void UI::change_state(void* target) {
    (void)target;
    std::cout << "Called select" << std::endl;
}
void UI::deselect() {
    if (this->_state.mode == ViewMode::OverView)
        return;
    /// call select on -1 vector (if before single process was selected now the
    /// Task will be input for select)
    this->change_state(NULL);
    std::cout << "Called deselect" << std::endl;
}
void UI::render() {
    std::cout << "\033[2J\033[H" << _mode_render << "\n"
              << _middle_part << "\n"
              << _bottom_part;
}
const UIState& UI::get_state() { return (_state); }

size_t UI::get_size_of_state() {
    if (this->_state.pointed_at == NULL)
        return 0;
    if (this->_state.mode == ViewMode::OverView)
        return (((std::vector<Task>*)this->_state.pointed_at)->size());
    if (this->_state.mode == ViewMode::TaskView)
        return (((std::vector<SingleProcess>*)this->_state.pointed_at)->size());
    if (this->_state.mode == ViewMode::ProcessView)
        return 1;
    return 0;
}
