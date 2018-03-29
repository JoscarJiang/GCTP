#pragma once

#include <queue>
#include <memory>
#include <mutex>
#include <pthread.h>
#include "ApiCommand.h"

//指令队列
class CommandQueue{

public:
	CommandQueue();
	~CommandQueue();
	void addCommand(std::shared_ptr<ApiCommand> newCommand);
	std::queue<std::shared_ptr<ApiCommand>> commandQueue;
	std::mutex queueMutex;
	bool working = true;				//工作状态
};
void *run_instr(void* Param);

