#ifndef UI_DEFINED
#define UI_DEFINED

#include <iostream>

enum ViewMode {
    OverView,
    TaskView,
    ProcessView,
};

struct UIState {
    ViewMode mode;
    size_t idx;
    /// check which mode
    ///
    /// OverView    -> std::vector<Task>
    ///
    /// TaskView    -> std::vector<SingleProcess>
    ///
    /// ProcessView -> SingleProcess
    void* pointed_at;
};

class UI {
  public:
    UI();
    UI(const UI& copy);
    UI& operator=(const UI& copy);
    ~UI();

    void cmd_j();
    void cmd_k();
    void select();
    void change_state(void* target);
    void deselect();

    const UIState& get_state() const;
    void render();

  private:
    size_t get_size_of_state();
    UIState _state;
    std::string _mode_render;
    std::string _middle_part;
    std::string _bottom_part;
};

std::ostream& operator<<(std::ostream& os, const UI& ui);

#endif // !UI
