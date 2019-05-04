#include "hlog.h"

HLog::HLog(const char *fileName)
    : mFile(NULL)
{
    mFile = fopen(fileName, "w");
    if(mFile == NULL)
    {
        printf("Cannot open log file.\n");
    }
}

HLog::HLog(const string& fileName)
    : mFile(NULL)
{
    mFile = fopen(fileName.data(), "w");
    if(mFile == NULL)
    {
        printf("Cannot open log file.\n");
    }
}

HLog::~HLog()
{
    if(mFile != NULL)
    {
        fclose(mFile);
    }
}

void HLog::write(const char* text, bool logInConsole)
{
    unique_lock<std::mutex> lock(mMutex);

    if(logInConsole)
    {
        printf("%s", text);
    }

    if(mFile != NULL)
    {
        fprintf(mFile, "%s", text);
    }
}

void HLog::write(const string &text, bool logInConsole)
{
    write(text.data(), logInConsole);
}

void HLog::separator(bool logInConsole)
{
    string sSeparator(16, '=');
    sSeparator += "\n";
    this->write(sSeparator, logInConsole);
}
