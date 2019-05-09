#ifndef H55_H
#define H55_H

#include "hspeedlow.h"
#include "hlog.h"
#include "hthreadpool.h"

#define H55_SERIE_SIZE_MIN                      10
#define H55_MSG_SERIE_SIZE_SMALL                "The serie must have 10 elements minim.\n"
#define H55_MSG_HEADER                          "index.neibgbour.set\n"
#define H55_BLOCK_STAT_MAX                      3
#define H55_THREAD_SIZE                         3
#define H55_HSET6_FILTER_NEIGHBOUR_SCORE_HIGH   8
#define H55_HSET6_FILTER_NEIGHBOUR_SCORE_LOW    4
#define H55_HSET6_FILTER_NEIGHBOUR_COUNT_HIGH   2
#define H55_HSET6_FILTER_NEIGHBOUR_COUNT_LOW    4
#define H55_HSET6_FILTER_NEIGHBOUR_SCORE_FIRSTS 10
#define H55_FILE_NEXT_02                        "H55_NEXT.%s_%s.txt"

/*
 * Used to compute the neighbours.
 */
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

/*
 * Keeps an horizon.
 */
class H55Horizon : public HRunnable
{
private:
    HLog* mLog;
    const string mParent;
    size_t mStat[HSET_LEN];
    HSERIE mSerie6;
    HSERIE mSerie5;
    HSERIE_PAIR mSeriePair5;
    deque<H55HSet6*> mL6Raw;
    deque<H55HSet6*> mL6FilteredNeighbours;
    HSERIE mSerie6FilteredNeighbours;

public:
    H55Horizon(HLog* log, const string& parent, const size_t stat[HSET_LEN], const HSET hset);
    ~H55Horizon();

private:
    void filterByNeighbours(const deque<H55HSet6*>& lin, deque<H55HSet6*>& lout);
    string toStringStatL6(const deque<H55HSet6*>& l6) const;

public:
    const HSERIE& getSerie6() const {return mSerie6;}
    const HSERIE& getSerie6FilteredNeighbours() const {return mSerie6FilteredNeighbours;}
    int compare(const size_t stat[HSET_LEN]) const;
    bool compareStat(const H55Horizon& visitor) const {return compare(visitor.mStat) > 0;}
    void add(const HSET hset){mSerie6.push_back(hset);}
    void neighbours();
    bool filterByStat() const;
    void run() {neighbours();}

public:
    string toString00() const;
    string toString01(HSET last) const;
    bool toString02(HSET last, string& result) const;
    bool toString03(HSET last, string& result) const;
    bool toString04(HSET last, string& result) const;
    string toString04_01(HSET last) const;
    string toString05() const;
    void log05(const string& date) const;
};

/*
 * The main class of the application.
 * 1.Generates all the sets (14 millions) and intersect each set with the 10 sets from rawdata.txt file.
 *   The intersection statistic represents an horizon.
 *   Based on the intersection associate the set with an horizon.
 * 2.Inside the horizons compute the neighbors of each set.
 */
class H55
{
private:
    HLog* mLog;
    HSERIE mSerie;
    const string mDate;
    deque<H55Horizon*> mHorizonsRaw;
    deque<H55Horizon*> mHorizonsFilteredStat;

private:
    HDIV mLimitMin[HSET_LEN];
    HDIV mLimitMax[HSET_LEN];
    HDIV mBlock[BLOCK_COUNT];
    HDIV mInterlaced[BLOCK_COUNT];
    map<size_t, size_t> mStatBlock;
    map<size_t, size_t> mStatInterlaced;

public:
    H55(HLog* log, const HSERIE& serie, const string& date);
    ~H55();

private:
    size_t horizonsSum() const;
    bool check(const HSOLUTION& solution, size_t stat[HSET_LEN]);
    void bckt(HSOLUTION& solution, size_t stat[HSET_LEN]);
    void add(const HSET hset, const size_t stat[HSET_LEN], int indexMin, int indexMax);
    void filterByStat(const deque<H55Horizon*>& lin , deque<H55Horizon *> &lout);

public:
    void run();

public:
    string toString01() const;
    string toString02(HSET last) const;
    string toString03(HSET last, bool onlySuccess) const;
    string toString04(HSET last, bool onlySucces) const;
    string toString05() const;
    void log01() const;
    void log05() const;
};

/*
 * Unit test class.
 */
class H55UT01 : public HRunnable
{
private:
    HLog* mLog;
    const HSERIE mSerie;
    const HSET mLast;
    const string mDate;
    H55* mH55;
    bool mFullTest;
    string mString03;
    string mString04;

private:
    HDIV mLimitMin[HSET_LEN];
    HDIV mLimitMax[HSET_LEN];

public:
    H55UT01(HLog* log, const HSERIE& serie, const HSET last, const string& date, bool fullTest);
    ~H55UT01();

public:
    string getDate() const {return mDate;}
    void run();

public:
    string toString02();
    string toString03();
    string toString04();
    void log02();
    void log04();
};


/*
 * Unit test class.
 */
class H55UT02
{
private:
    HLog* mLog;
    const HSERIE mSerie;
    const string mDate;
    deque<H55UT01*> mLH55UT01;
    string mResult03;
    string mResult04;

public:
    H55UT02(HLog* log, const HSERIE& serie, const string& date);
    ~H55UT02();

public:
    void run();

public:
    void log03();
    void log04();
};

#endif // H55_H
