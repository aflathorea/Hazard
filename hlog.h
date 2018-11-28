#ifndef HLOG_H
#define HLOG_H

#include <QFile>
#include <QTextStream>
#include <QMutex>

class HLog
{

private:
    QFile *m_file;
    QTextStream *m_textStream;
    QMutex m_mutex;

public:
    HLog(const QString& fileName);
    ~HLog();

public:
    void write(const QString& text, bool logInConsole = true);
    void separator(bool logInConsole = true);
};

#endif // HLOG_H
