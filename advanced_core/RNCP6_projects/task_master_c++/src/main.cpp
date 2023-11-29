#include <TaskMaster.hpp>


int main(int argc, char* argv[]) {
    TaskMaster  tasky(argc, argv);
    tasky.unser_input_loop();
    std::cout << "Loop ended" << std::endl;
    return (0);
}
