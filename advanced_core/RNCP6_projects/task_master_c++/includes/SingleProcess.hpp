#ifndef SingleProcess_DEFINED
#define SingleProcess_DEFINED

#include <cstddef>
class SingleProcess {
  public:
    SingleProcess();
    SingleProcess(const SingleProcess& copy);
    SingleProcess& operator=(const SingleProcess& copy);
    ~SingleProcess();

  private:
    // size_t remaining_retries;
};

#endif // !SingleProcess
