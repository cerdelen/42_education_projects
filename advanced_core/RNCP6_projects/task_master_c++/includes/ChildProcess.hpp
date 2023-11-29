#ifndef ChildProcess_DEFINED
#define ChildProcess_DEFINED

#include <cstdlib>
#include <CommandConfig.hpp>

class ChildProcess
{
	public:
		ChildProcess(const CommandConfig& conf);
		ChildProcess(const ChildProcess &copy);
		ChildProcess&		operator=(const ChildProcess &copy);
		~ChildProcess();
		void	restart();
		void	kill();
	
	private:
		void	spawn();
		pid_t		_id;
		//  cmd;
};

#endif // !ChildProcess
