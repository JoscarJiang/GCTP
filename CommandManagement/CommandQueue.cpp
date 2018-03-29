#include "CommandQueue.h"
#include <chrono>

#pragma comment(lib, "pthreadVC2.lib")  // 必须加上这句  
using std::lock_guard;
using std::mutex;

CommandQueue::~CommandQueue(){
	working = false;		// 停止线程工作
}

CommandQueue::CommandQueue() {
}

void CommandQueue::addCommand(std::shared_ptr<ApiCommand> newCommand){
	lock_guard<mutex> lock(queueMutex);
	commandQueue.push(newCommand);
}

// 一秒之内不能连续发送指令
void* run_instr(void* _This){
	std::cout << ">>> Now we enter the command queue thread" << std::endl;
	CommandQueue *This_ = (CommandQueue *)_This;
	while (This_->working){
		if (!This_->commandQueue.empty()){
			int NOT_ZERO = 100;
			int result = NOT_ZERO;
			{
				lock_guard<mutex> lock(This_->queueMutex);
				// 这里可以再加一层检查机制，对于价格与预期差距较大的则抛出，由策略类来做，返回1(继续)或者0(舍弃)
				
				auto command = This_->commandQueue.front();
				result = command->execute();
				if (result == 0){
					// result为0意味着发送指令成功
					std::cout << ">>> The command is sent successfully." << std::endl;
					This_->commandQueue.pop();
				}
			}
			// 如果发送了指令休息一秒，把这段代码放出来是为了释放锁，使得沉睡的一秒期间其它代码可以访问临界资源
			if (result == 0){
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
	}
	std::cout << ">>> Now we get out the command queue thread" << std::endl;
	return NULL;
}