// #include "UI.hpp"
#include <TaskMaster.hpp>

TaskMaster::TaskMaster(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    this->_logger.open("Log_TaskMaster.txt", std::ios::trunc);
    LOGGING
}
TaskMaster::TaskMaster(const TaskMaster& copy) {
    LOGGING
    *this = copy;
}
TaskMaster& TaskMaster::operator=(const TaskMaster& copy) {
    LOGGING(void) copy;
    return (*this);
}

TaskMaster::~TaskMaster() {
    LOGGING
    std::cout << "deconstructor TaskMaster" << std::endl;
    _logger.close();
}

void TaskMaster::unser_input_loop() {
    LOGGING
    std::string input;

    while (true) {
        this->_ui.render();
        std::getline(std::cin, input); // blocked until command givven
        std::cout << "Our Inout: \"" << input << "\"" << std::endl;
        if (input == "q" || input == "quit")
            return;
        if (input == "j" || input == "up")
            this->cmd_j();
        if (input == "k" || input == "down")
            this->cmd_k();
        if (input == "r" || input == "reload")
            this->reload();
        if (input == "s" || input == "select")
            this->select();
        // if (input == "p" || input == "print") // print details
        //     this->print();
        if (input == "t" || input == "kill")
            this->kill();
        if (input == "d" || input == "deselect")
            this->deselect();
    }
}

void TaskMaster::reload() {
    LOGGING
    std::cout << "Called Reload commad" << std::endl;
}
void TaskMaster::select() {
    LOGGING
    this->_ui.select();
}
// void TaskMaster::print() {
//     LOGGING
//     this->_ui.print();
// }
/// Changes UIState (-1)
void TaskMaster::deselect() {
    LOGGING
    this->_ui.deselect();
}
/// Kills depending on UIState either a whole Task or a single Process
void TaskMaster::kill() {
    LOGGING
    std::cout << "Called kill cmd" << std::endl;
}
void TaskMaster::cmd_j() {
    LOGGING
    // _logger << "UIState: { mode: " << this->_ui.get_state().mode
    //                    << ", idx: " << this->_ui.get_state().idx << " }\n";
    _logger << "Before: " << this->_ui;
    this->_ui.cmd_j();
    _logger << "After: " << this->_ui;
    // _logger << "UIState: { mode: " << this->_ui.get_state().mode
    //         << ", idx: " << this->_ui.get_state().idx << " }\n";
}
void TaskMaster::cmd_k() {
    LOGGING
    // _logger << "UIState: { mode: " << this->_ui.get_state().mode
    //         << ", idx: " << this->_ui.get_state().idx << " }\n";
    _logger << "Before: " << this->_ui;
    this->_ui.cmd_k();
    _logger << "After: " << this->_ui;
    // _logger << "UIState: { mode: " << this->_ui.get_state().mode
    // << ", idx: " << this->_ui.get_state().idx << " }\n";
}
