#ifndef HLOG_H
#define HLOG_H

#include <cstdio>
#include <string>
#include <cstring>
#include <mutex>

using namespace std;

/*
 * Write in files.
 * Thread safe.
 */
class HLog
{
private:
    FILE *mFile;
    mutex mMutex;

public:
    HLog(const char* fileName);
    HLog(const string& fileName);
    ~HLog();

public:
    void write(const char* text, bool logInConsole = true);
    void write(const string& text, bool logInConsole = true);
    void separator(bool logInConsole = true);
};

#endif // HLOG_H
