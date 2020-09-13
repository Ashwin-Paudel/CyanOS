#pragma once
#include <Result.h>
#include <Rule5.h>
#include <UniquePointer.h>

class Process;
class ProcessDescription
{
  private:
	explicit ProcessDescription(Process& process);
	Process& m_process;

  public:
	NON_COPYABLE(ProcessDescription);
	NON_MOVABLE(ProcessDescription);
	static Result<UniquePointer<ProcessDescription>> open(size_t pid, int access);
	~ProcessDescription();
	int wait_signal();

	friend class UniquePointer<ProcessDescription>;
};