#include "hlog.h"
#include <iostream>
#include <cstdarg>

using namespace std;

HLog::HLog(const QString &fileName)
{
    m_file = new QFile(fileName);
    if(!m_file->open(QIODevice::Append | QIODevice::Text))
    {
        cout << qPrintable("Cannot open log file.") << endl;
        return;
    }

    m_textStream = new QTextStream(m_file);
}

HLog::~HLog()
{
    m_textStream->flush();
    delete m_textStream;
    m_textStream = 0;

    m_file->close();
    delete m_file;
    m_file = 0;
}

void HLog::write(const QString& text, bool logInConsole)
{
    m_mutex.lock();

    if(logInConsole)
    {
        cout << qPrintable(text);
    }

    (*m_textStream) << text;

    m_mutex.unlock();
}

void HLog::separator(bool logInConsole)
{
    QString sSeparator = QString("%1\n").arg(QString(16, '='));
    this->write(sSeparator, logInConsole);
}
