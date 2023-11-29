#ifndef CommandsWrapper_DEFINED
#define CommandsWrapper_DEFINED

class CommandsWrapper
{
	public:
		CommandsWrapper();
		CommandsWrapper(const CommandsWrapper &copy);
		CommandsWrapper&		operator=(const CommandsWrapper &copy);
		~CommandsWrapper();
};

#endif // !CommandsWrapper
