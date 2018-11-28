#ifndef H55_H
#define H55_H

#include <QRunnable>
#include "hsetlowspeed.h"
#include "hlog.h"

using namespace std;


#define H55_SERIE_SIZE_MIN                      10
#define H55_MSG_SERIE_SIZE_SMALL                "Seria trebuia sa aiba minim 10 elemente.\n"
#define H55_MSG_HEADER                          "index.vecini.set\n"
#define H55_BLOCK_STAT_MAX                      3
#define H55_THREAD_SIZE                         3
#define H55_HSET6_FILTER_NEIGHBOUR_SCORE_HIGH   8
#define H55_HSET6_FILTER_NEIGHBOUR_SCORE_LOW    4
#define H55_HSET6_FILTER_NEIGHBOUR_COUNT_HIGH   2
#define H55_HSET6_FILTER_NEIGHBOUR_COUNT_LOW    4
#define H55_HSET6_FILTER_NEIGHBOUR_SCORE_FIRSTS 10
#define H55_FILE_NOW_01                         "H55_NOW.%1.txt"
#define H55_FILE_NEXT_02                        "H55_NEXT.%1_%2.txt"
#define H55_FILE_NEXT_SUMMARY_05                "H55_NEXT_SUMMARY.%1.txt"
#define H55_UT01_FILE_02                        "H55_HIST_02.%1.txt"
#define H55_UT01_FILE_04                        "H55_HIST_04.%1.txt"
#define H55_UT02_FILE_03                        "H55_ALL_03.%1.txt"
#define H55_UT02_FILE_04                        "H55_ALL_04.%1.txt"

class H55HSet6
{
private:
    const HSET mHSet6;
    HSET mHSet5[HSET_LEN];
    size_t mHSet5Score[HSET_LEN];
    size_t mScore;

public:
    H55HSet6(const HSET hset6,const HSERIE& serie5);
    ~H55HSet6();

public:
    size_t getHSet5(int index) const {return mHSet5[index];}
    size_t& getHSet5Score(int index) {return mHSet5Score[index];}
    size_t getScore() const {return mScore;}
    HSET getHSET6() const {return mHSet6;}
    void scoreSum();
    void scoreMax();
    bool filterByNeighbours(size_t score6[H55_HSET6_FILTER_NEIGHBOUR_SCORE_FIRSTS]);
};


class H55Horizon
{
private:
    HLog* mLog;
    const QString mParent;
    size_t mStat[HSET_LEN];
    HSERIE mSerie6;
    HSERIE mSerie5;
    HSERIE_PAIR mSeriePair5;
    QList<H55HSet6*> mL6Raw;
    QList<H55HSet6*> mL6FilteredNeighbours;
    HSERIE mSerie6FilteredNeighbours;

public:
    H55Horizon(HLog* log, const QString& parent, const size_t stat[HSET_LEN], const HSET hset);
    ~H55Horizon();

private:
    void filterByNeighbours(const QList<H55HSet6*>& lin, QList<H55HSet6*>& lout);
    QString toStringStatL6(const QList<H55HSet6*>& l6) const;

public:
    const HSERIE& getSerie6() const {return mSerie6;}
    const HSERIE& getSerie6FilteredNeighbours() const {return mSerie6FilteredNeighbours;}
    int compare(const size_t stat[HSET_LEN]) const;
    bool compareStat(const H55Horizon& visitor) const {return compare(visitor.mStat) > 0;}
    void add(const HSET hset){mSerie6.append(hset);}
    void neighbours();
    bool filterByStat() const;

public:
    QString toString00() const;
    QString toString01(HSET last) const;
    bool toString02(HSET last, QString& result) const;
    bool toString03(HSET last, QString& result) const;
    bool toString04(HSET last, QString& result) const;
    QString toString04_01(HSET last) const;
    QString toString05() const;
    void log05(const QString& date) const;
};


class H55
{
private:
    HLog* mLog;
    HSERIE mSerie;
    const QString mDate;
    QList<H55Horizon*> mHorizonsRaw;
    QList<H55Horizon*> mHorizonsFilteredStat;

private:
    HDIV mLimitMin[HSET_LEN];
    HDIV mLimitMax[HSET_LEN];
    HDIV mBlock[BLOCK_COUNT];
    HDIV mInterlaced[BLOCK_COUNT];
    QMap<size_t, size_t> mStatBlock;
    QMap<size_t, size_t> mStatInterlaced;

public:
    H55(HLog* log, const HSERIE& serie, const QString& date);
    ~H55();

private:
    size_t horizonsSum() const;
    bool check(const HSOLUTION& solution, size_t stat[HSET_LEN]);
    void bckt(HSOLUTION& solution, size_t stat[HSET_LEN]);
    void add(const HSET hset, const size_t stat[HSET_LEN], int indexMin, int indexMax);
    void filterByStat(const QList<H55Horizon*>& lin , QList<H55Horizon *> &lout);

public:
    void run();

public:
    QString toString01() const;
    QString toString02(HSET last) const;
    QString toString03(HSET last, bool onlySuccess) const;
    QString toString04(HSET last, bool onlySucces) const;
    QString toString05() const;
    void log01() const;
    void log05() const;
};


class H55UT01 : public QRunnable
{
private:
    HLog* mLog;
    const HSERIE mSerie;
    const HSET mLast;
    const QString mDate;
    H55* mH55;
    bool mFullTest;
    QString mString03;
    QString mString04;

private:
    HDIV mLimitMin[HSET_LEN];
    HDIV mLimitMax[HSET_LEN];

public:
    H55UT01(HLog* log, const HSERIE& serie, const HSET last, const QString& date, bool fullTest);
    ~H55UT01();

public:
    QString getDate() const {return mDate;}
    void run();

public:
    QString toString02();
    QString toString03();
    QString toString04();
    void log02();
    void log04();
};


class H55UT02
{
private:
    HLog* mLog;
    const HSERIE mSerie;
    const QString mDate;
    QList<H55UT01*> mLH55UT01;
    QString mResult03;
    QString mResult04;

public:
    H55UT02(HLog* log, const HSERIE& serie, const QString& date);
    ~H55UT02();

public:
    void run();

public:
    void log03();
    void log04();
};

#endif // H55_H
