#include "hthreadpool.h"

HThreadPool::HThreadPool()
{

}

HThreadPool::~HThreadPool()
{

}

HRunnable* HThreadPool::pop_front()
{
    unique_lock<mutex> lock(mMutex);

    if(mRunnables.empty()) {
        return nullptr;
    } else {
        HRunnable* result = mRunnables.front();
        mRunnables.pop_front();

        return result;
    }
}

void HThreadPool::push_back(HRunnable* runnable)
{
    unique_lock<mutex> lock(mMutex);

    mRunnables.push_back(runnable);
}

void HThreadPool::run()
{
    for(int index = 0; index < HTREADPOOL_SIZE; ++index) {
        mThreadsProcessing[index] = new thread(hThreadWorker, this);
    }

    for(int index = 0; index < HTREADPOOL_SIZE; ++index) {
        mThreadsProcessing[index]->join();
    }

    for(int index = 0; index < HTREADPOOL_SIZE; ++index) {
        delete mThreadsProcessing[index];
        mThreadsProcessing[index] = nullptr;
    }
}

void hThreadWorker(HThreadPool *hThreadPool)
{
    HRunnable* runnable = nullptr;
    do
    {
        runnable = hThreadPool->pop_front();
        if(runnable != nullptr) {
            runnable->run();
        }
    }while(runnable != nullptr);
}
