#ifndef UI_DEFINED
#define UI_DEFINED

#include <iostream>

enum ViewMode {
	OverView,
	TaskView,
	ProcessView,
};

class UI
{
	public:
		UI();
		UI(const UI &copy);
		UI&		operator=(const UI &copy);
		~UI();

		void	cmd_j();
		void	cmd_k();
		void	select();
		void	print();
		void	deselect();

		void	render();
		size_t	get_idx();
		void	set_idx(int change);

	private:
		size_t			_ui_idx;
		ViewMode		_mode;
		std::string		_mode_render;
		std::string		_middle_part;
		std::string		_bottom_part;
};

#endif // !UI