#ifndef HTHREADPOOL_H
#define HTHREADPOOL_H


#include <mutex>
#include <thread>
#include "hspeedlow.h"
#include "hlog.h"


#define HTREADPOOL_SIZE 4

/*
 * An interface implemented by classes who are clients for thread pool.
 */
class HRunnable
{
public:
    virtual void run() = 0;
};


class HThreadPool;
void hThreadWorker(HThreadPool* hThreadPool);

/*
 * Executes in parallel 4 functions. The number of functions executed in parallel is given by HTREADPOOL_SIZE.
 * Has a queue of functions. When a function is finished get another one from queue until empty the queue.
 */
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
