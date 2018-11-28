#include <QCoreApplication>
#include <QElapsedTimer>
#include <iostream>
#include "hsetlowspeed.h"
#include "h55.h"

#define H55_LOG_FILE    "H55_Log.%1.txt"
#define H55_RAW_DATA    "rawdata.txt"
#define H55_RAW_DATA_ERROR  "Error reading rawdata.txt file\n"


bool readSerieFull(const QString& filePath, HSERIE& serie, QString& lastDate) {
    if(!readRawData(filePath, serie, lastDate))
    {
        printf(H55_RAW_DATA_ERROR);
        return false;
    }

    return true;
}

bool readSerieFull(HSERIE& serie, QString& lastDate)
{
    return readSerieFull(H55_RAW_DATA, serie, lastDate);
}

void h55All()
{
    printf("h55All(). start.\n");

    HSERIE history;
    QString lastDate;

    readSerieFull(history, lastDate);

    QString sLogFileName = QString(H55_LOG_FILE).arg(lastDate);
    HLog log(sLogFileName);

    log.write(lastDate + "\n");
    log.separator();

    QString sHistory;
    hserieToString(history, false, sHistory);
    log.write(sHistory);
    log.separator();

    H55UT02 h55UT02(&log, history, lastDate);
    h55UT02.run();
    h55UT02.log03();
    h55UT02.log04();

    printf("h55All(). end.\n");
}

void h55One()
{
    printf("h55One(). start.\n");

    HSERIE history;
    QString lastDate;

    readSerieFull(history, lastDate);

    QString sLogFileName = QString(H55_LOG_FILE).arg(lastDate);
    HLog log(sLogFileName);

    log.write(lastDate + "\n");
    log.separator();

    QString sHistory;
    hserieToString(history, false, sHistory);
    log.write(sHistory);
    log.separator();

    HSET last;
    HSERIE hist10;
    serieLast(history, hist10, last);

    H55UT01 h55UT01(&log, hist10, last, lastDate, false);
    h55UT01.run();
    h55UT01.log04();

    H55 h55(&log, history, lastDate);
    h55.run();
    h55.log05();

    printf("h55One(). end.\n");
}

int main(int argc, char *argv[])
{
    QElapsedTimer timer;
    timer.start();

    bool bAll = false;
    if(argc > 1)
    {\
        char* param = argv[1];
        if(strcmp(param, "a") == 0 || strcmp(param, "A") == 0)
        {
            bAll = true;
        }
    }

    if(bAll)
    {
        h55All();
    }
    else
    {
        h55One();
    }

    qint64 duration = timer.nsecsElapsed();
    QString sTime;
    sTime.sprintf("Elapsed time: %'lld seconds", duration / 1000000000);
    cout << qPrintable(sTime) << endl;
}
