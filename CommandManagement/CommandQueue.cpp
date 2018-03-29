#include "CommandQueue.h"
#include <chrono>

#pragma comment(lib, "pthreadVC2.lib")  // ����������  
using std::lock_guard;
using std::mutex;

CommandQueue::~CommandQueue(){
	working = false;		// ֹͣ�̹߳���
}

CommandQueue::CommandQueue() {
}

void CommandQueue::addCommand(std::shared_ptr<ApiCommand> newCommand){
	lock_guard<mutex> lock(queueMutex);
	commandQueue.push(newCommand);
}

// һ��֮�ڲ�����������ָ��
void* run_instr(void* _This){
	std::cout << ">>> Now we enter the command queue thread" << std::endl;
	CommandQueue *This_ = (CommandQueue *)_This;
	while (This_->working){
		if (!This_->commandQueue.empty()){
			int NOT_ZERO = 100;
			int result = NOT_ZERO;
			{
				lock_guard<mutex> lock(This_->queueMutex);
				// ��������ټ�һ������ƣ����ڼ۸���Ԥ�ڲ��ϴ�����׳����ɲ���������������1(����)����0(����)
				
				auto command = This_->commandQueue.front();
				result = command->execute();
				if (result == 0){
					// resultΪ0��ζ�ŷ���ָ��ɹ�
					std::cout << ">>> The command is sent successfully." << std::endl;
					This_->commandQueue.pop();
				}
			}
			// ���������ָ����Ϣһ�룬����δ���ų�����Ϊ���ͷ�����ʹ�ó�˯��һ���ڼ�����������Է����ٽ���Դ
			if (result == 0){
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
	}
	std::cout << ">>> Now we get out the command queue thread" << std::endl;
	return NULL;
}