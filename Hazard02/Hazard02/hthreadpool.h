#ifndef HTHREADPOOL_H
#define HTHREADPOOL_H


#include <mutex>
#include <thread>
#include "hspeedlow.h"
#include "hlog.h"


#define HTREADPOOL_SIZE 4


class HRunnable
{
public:
    virtual void run() = 0;
};


class HThreadPool;
void hThreadWorker(HThreadPool* hThreadPool);

class HThreadPool
{
friend void hThreadWorker(HThreadPool* hThreadPool);
private:
    mutex mMutex;
    deque<HRunnable*> mRunnables;
    thread* mThreadsProcessing[HTREADPOOL_SIZE];

public:
    HThreadPool();
    ~HThreadPool();

private:
    HRunnable* pop_front();

public:
    void push_back(HRunnable* runnable);
    void run();
};

#endif // HTHREADPOOL_H
