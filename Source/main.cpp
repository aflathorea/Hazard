#include "hspeedlow.h"
#include "hlog.h"
#include "h55.h"
#include <chrono>

using namespace std::chrono;

void h55History()
{
    printf("exec55_01(). start.\n");

    HSERIE history;
    string lastDate;
    HSET last;
    readSerieWithoutLast(history, last, lastDate);

    char sLogFileName[64] = {};
    sprintf(sLogFileName, "H55_01.%s.txt", lastDate.data());
    HLog log(sLogFileName);

    log.write(lastDate + "\n");
    log.separator();
    log.write(hserieToString(history));
    log.separator();
    log.write(hsetToString(last) + "\n");
    log.separator();

    H55UT01 h55UT01(&log, history, last, lastDate, false);
    h55UT01.run();
    h55UT01.log04();

    printf("exec55_01(). end.\n");
}

void h55Forecast()
{
    printf("exec55_02(). start.\n");

    HSERIE history;
    string lastDate;
    HSET last;
    readSerieWithoutLast(history, last, lastDate);

    char sLogFileName[64] = {};
    sprintf(sLogFileName, "H55_02.%s.txt", lastDate.data());
    HLog log(sLogFileName);

    log.write(lastDate + "\n");
    log.separator();
    log.write(hserieToString(history));
    log.separator();
    log.write(hsetToString(last) + "\n");
    log.separator();

    H55UT01 h55UT01(&log, history, last, lastDate, false);
    h55UT01.run();
    h55UT01.log04();

    H55 h55(&log, history, lastDate);
    h55.run();
    h55.log05();

    printf("exec55_02(). end.\n");

}

void h55All()
{
    printf("exec55_03(). start.\n");

    HSERIE history;
    string lastDate;
    readSerieFull(history, lastDate);

    char sLogFileName[64] = {};
    sprintf(sLogFileName, "H55_03.%s.txt", lastDate.data());
    HLog log(sLogFileName);

    log.write(lastDate + "\n");
    log.separator();
    log.write(hserieToString(history));
    log.separator();

    H55UT02 h55UT02(&log, history, lastDate);
    h55UT02.run();
    h55UT02.log03();
    h55UT02.log04();

    printf("exec55_03(). end.\n");
}

int main(int argc, char *argv[])
{
    high_resolution_clock::time_point tStart = high_resolution_clock::now();

    int bOne = true;
    if(argc > 1)
    {\
        char* param = argv[1];
        if(strcmp(param, "a") == 0 || strcmp(param, "A") == 0)
        {
            h55All();
            bOne = false;
        }
        else if(strcmp(param, "h") == 0 || strcmp(param, "H") == 0)
        {
            h55History();
            bOne = false;
        }
    }

    if(bOne)
    {
        h55Forecast();
    }

    high_resolution_clock::time_point tEnd = high_resolution_clock::now();
    duration<long> time_span = duration_cast<duration<long>>(tEnd - tStart);
    printf("Time exec = %lu seconds.\n", time_span.count());

    return 0;
}
