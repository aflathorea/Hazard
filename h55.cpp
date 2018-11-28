#include "h55.h"
#include <QThreadPool>


////////////////////////////////////////////////////////////

H55HSet6::H55HSet6(const HSET hset6,const HSERIE& serie5)
    : mHSet6(hset6), mScore(0)
{
    for(int index = 0; index < serie5.size(); ++index)
    {
        mHSet5[index] = serie5[index];
        mHSet5Score[index] = 0;
    }
}

H55HSet6::~H55HSet6()
{

}

void H55HSet6::scoreSum()
{
    for(int index = 0; index < HSET_LEN; ++index)
    {
        mScore += mHSet5Score[index];
    }
}

void H55HSet6::scoreMax()
{
    for(int index = 0; index < HSET_LEN; ++index)
    {
        mScore = qMax(mScore, mHSet5Score[index]);
    }
}

bool H55HSet6::filterByNeighbours(size_t score6[H55_HSET6_FILTER_NEIGHBOUR_SCORE_FIRSTS])
{
    size_t countGreaterLow = 0;
    size_t countGreaterHigh = 0;
    for(int index = 0; index < HSET_LEN; ++index)
    {
        if(mHSet5Score[index] >= H55_HSET6_FILTER_NEIGHBOUR_SCORE_LOW)
        {
            countGreaterLow++;
            if(mHSet5Score[index] >= H55_HSET6_FILTER_NEIGHBOUR_SCORE_HIGH)
            {
                countGreaterHigh++;
            }
        }
    }

    if(countGreaterLow < H55_HSET6_FILTER_NEIGHBOUR_COUNT_LOW)
    {
        return false;
    }

    if(countGreaterHigh < H55_HSET6_FILTER_NEIGHBOUR_COUNT_HIGH)
    {
        return false;
    }

    bool found = false;
    for(int index = 0; index < H55_HSET6_FILTER_NEIGHBOUR_SCORE_FIRSTS; ++index)
    {
        if(mScore == score6[index])
        {
            found = true;
            break;
        }
    }

    return found;
}

////////////////////////////////////////////////////////////

H55Horizon::H55Horizon(HLog *log, const QString& parent, const size_t stat[], const HSET hset)
    : mLog(log), mParent(parent)
{
    for(int index = 0; index < HSET_LEN; ++index)
    {
        mStat[index] = stat[index];
    }

    add(hset);
}

H55Horizon::~H55Horizon()
{
    for(auto crt : mL6Raw)
        delete crt;
}

int H55Horizon::compare(const size_t stat[HSET_LEN]) const
{
    for(int index = 0; index < HSET_LEN; ++index)
    {
        if(stat[index] < mStat[index])
        {
            return -1;
        }
        else if(stat[index] > mStat[index])
        {
            return 1;
        }
    }

    return 0;
}

void H55Horizon::neighbours()
{
    QString sStat;
    hStatToString((const HSTAT)mStat, sStat, '|');

    mLog->write(QString("H55Horizon::neighbours(%1. [%2])\n").arg(mParent).arg(sStat));

    for(HSET hset6 : mSerie6)
    {
        HSERIE neighbours5;
        generateSubsetMinus1(hset6, neighbours5, HSET_LEN);

        H55HSet6* h55hset6 = new H55HSet6(hset6,neighbours5);
        mL6Raw.append(h55hset6);

        mSerie5.append(neighbours5);
    }

    qSort(mSerie5.begin(), mSerie5.end());
    generateSeriePair(mSerie5, mSeriePair5);

    for(auto crt : mL6Raw)
    {
        for(int index = 0; index < HSET_LEN; ++index)
        {
            crt->getHSet5Score(index) = countBinarySearch(
                        mSeriePair5,
                        crt->getHSet5(index),
                        0, mSeriePair5.size() - 1);
        }
        crt->scoreSum();
    }

    qSort(mL6Raw.begin(), mL6Raw.end(), [](const H55HSet6* x, const H55HSet6* y)
    {
        return x->getScore() > y->getScore();
    });

    filterByNeighbours(mL6Raw, mL6FilteredNeighbours);
    for(auto crt : mL6FilteredNeighbours)
    {
        mSerie6FilteredNeighbours.append(crt->getHSET6());
    }
}

bool H55Horizon::filterByStat() const
{
    for(int index = 0; index < 2; ++index)
    {
        if(mStat[index] == 0)
        {
            return false;
        }
    }

    if(mStat[2] > 3)
    {
        return false;
    }
    else if(mStat[2] > mStat[1] + 1)
    {
        return false;
    }

    if(mStat[3] > 1)
    {
        return false;
    }
    else if(mStat[2] == 0 && mStat[3] > 0)
    {
        return false;
    }

    for(int index = 4; index < HSET_LEN; ++index)
    {
        if(mStat[index] > 0)
        {
            return false;
        }
    }

    return true;
}

void H55Horizon::filterByNeighbours(const QList<H55HSet6*>& lin, QList<H55HSet6*>& lout)
{
    QMap<size_t, size_t> stat6;
    for(auto crt : mL6Raw)
    {
        statAdd(stat6, crt->getScore());
    }

    QList<QPair<size_t, size_t>> l6Score;
    for(size_t key : stat6.keys())
    {
        l6Score.append(QPair<size_t, size_t>(key, stat6[key]));
    }

    qSort(l6Score.begin(), l6Score.end(), [](const QPair<size_t, size_t>& x, const QPair<size_t, size_t>& y)
    {
        return x.second > y.second;
    });

    // get the firsts 10 with higher score
    size_t score6[H55_HSET6_FILTER_NEIGHBOUR_SCORE_FIRSTS] = {};
    for(int index = 0; index < H55_HSET6_FILTER_NEIGHBOUR_SCORE_FIRSTS; ++index)
    {
        if(index < l6Score.size())
        {
            score6[index] = l6Score[index].first;
        }
    }

    for(auto h6 : lin)
    {
        if(h6->filterByNeighbours(score6))
        {
            lout.append(h6);
        }
    }

    qSort(lout.begin(), lout.end(), [](const H55HSet6* x, const H55HSet6* y){
        return x->getScore() > y->getScore();
    });
}

QString H55Horizon::toStringStatL6(const QList<H55HSet6*>& l6) const
{
    QMap<size_t, size_t> stat6;
    for(auto h6 : l6)
    {
        statAdd(stat6, h6->getScore());
    }

    return statToStringSortByValueDesc(stat6);
}

QString H55Horizon::toString00() const
{
    QString sStat;
    hStatToString((const HSTAT)mStat, sStat, '|');

    QMap<size_t, size_t> sp5Stat;
    statSeriePair(mSeriePair5, sp5Stat);

    QString result = QString("[%1]->%2\n\t%3").arg(sStat).arg(mSerie6.size()).arg(statToStringLast5(sp5Stat, INT_MAX));

    return result;
}

QString H55Horizon::toString01(HSET last) const
{
    QString sStat;
    hStatToString((const HSTAT)mStat, sStat, '|');

    size_t statLast[HSET_LEN] = {};
    intersect2(last, mSerie6, statLast);

    QString s5;
    QString s6;
    bool found = false;
    if(statLast[HSET_LEN -2] == 1)
    {
        found = true;
        HSERIE neighbours5;
        generateSubsetMinus1(last, neighbours5, HSET_LEN);

        for(auto h5 : neighbours5)
        {
            s5 += QString("%1,").arg(
                        countBinarySearch(
                            mSeriePair5,
                            h5,
                            0, mSeriePair5.size() - 1));
        }

        QMap<size_t, size_t> stat6;
        size_t scoreLast;
        for(auto h6 : mL6Raw)
        {
            statAdd(stat6, h6->getScore());
            if(h6->getHSET6() == last)
            {
                scoreLast = h6->getScore();
            }
        }

        s6 += QString("%1\n\t%2").arg(scoreLast).arg(statToStringSortByValueDesc(stat6));
    }

    QString sStatLast;
    hStatToString(statLast, sStatLast, '|');


    QString result = QString("%1[%2]->%3 [%4]").arg(found ? "!" : "").arg(sStat).arg(mSerie6.size()).arg(sStatLast);
    if(found)
    {
        result += QString("\n\t{%1}").arg(s5);
    }

    if(!mSeriePair5.isEmpty())
    {
        QMap<size_t, size_t> sp5Stat;
        statSeriePair(mSeriePair5, sp5Stat);
        result += QString("\n\t%1").arg(statToStringLast5(sp5Stat, INT_MAX));
    }

    if(!s6.isEmpty())
    {
        result += QString("\n\t%1").arg(s6);
    }

    return result;
}

bool H55Horizon::toString02(HSET last, QString& result) const
{
    QString sStat;
    hStatToString((const HSTAT)mStat, sStat, '|');

    size_t statLast[HSET_LEN] = {};
    intersect2(last, mSerie6, statLast);

    if(statLast[HSET_LEN -2] == 1)
    {
        QString s5;
        QString s6;

        HSERIE neighbours5;
        generateSubsetMinus1(last, neighbours5, HSET_LEN);

        for(auto h5 : neighbours5)
        {
            s5 += QString("%1,").arg(
                        countBinarySearch(
                            mSeriePair5,
                            h5,
                            0, mSeriePair5.size() - 1));
        }

        QMap<size_t, size_t> stat6;
        size_t scoreLast;
        for(auto h6 : mL6Raw)
        {
            statAdd(stat6, h6->getScore());
            if(h6->getHSET6() == last)
            {
                scoreLast = h6->getScore();
            }
        }

        s6 += QString("%1\n\t%2").arg(scoreLast).arg(statToStringSortByValueDesc(stat6));

        QString sStatLast;
        hStatToString(statLast, sStatLast, '|');

        QMap<size_t, size_t> sp5Stat;
        statSeriePair(mSeriePair5, sp5Stat);

        result = QString("[%1]->%2 [%3]").arg(sStat).arg(mSerie6.size()).arg(sStatLast);
        result += QString("\n\t{%1}").arg(s5);
        result += QString("\n\t%1\n\t%2").arg(statToStringLast5(sp5Stat, INT_MAX)).arg(s6);

        return true;
    }
    else
    {
        return false;
    }
}

bool H55Horizon::toString03(HSET last, QString& result) const
{
    QString sStat;
    hStatToString((const HSTAT)mStat, sStat, '|');

    size_t statLast[HSET_LEN] = {};
    intersect2(last, mSerie6, statLast);

    if(statLast[HSET_LEN -2] == 1)
    {
        QString sStatLast;
        hStatToString((const HSTAT)statLast, sStatLast, '|');

        result = QString("[%1]->%2\n\t[%3]").arg(sStat).arg(mSerie6.size()).arg(sStatLast);

        return true;
    }
    else
    {
        return false;
    }
}

bool H55Horizon::toString04(HSET last, QString& result) const
{
    QString sStat;
    hStatToString((const HSTAT)mStat, sStat, '|');

    size_t statLast[HSET_LEN] = {};
    intersect2(last, mSerie6FilteredNeighbours, statLast);

    if(statLast[HSET_LEN -2] == 1)
    {
        QString s5;
        QString s6;

        HSERIE neighbours5;
        generateSubsetMinus1(last, neighbours5, HSET_LEN);

        for(auto h5 : neighbours5)
        {
            s5 += QString("%1,").arg(
                        countBinarySearch(
                            mSeriePair5,
                            h5,
                            0, mSeriePair5.size() - 1));
        }

        QMap<size_t, size_t> stat6;
        size_t scoreLast;
        for(auto h6 : mL6FilteredNeighbours)
        {
            statAdd(stat6, h6->getScore());
            if(h6->getHSET6() == last)
            {
                scoreLast = h6->getScore();
            }
        }

        s6 += QString("%1\n\t%2").arg(scoreLast).arg(statToStringSortByValueDesc(stat6));

        QString sStatLast;
        hStatToString(statLast, sStatLast, '|');

        QMap<size_t, size_t> sp5Stat;
        statSeriePair(mSeriePair5, sp5Stat);

        result = QString("[%1]->%2 [%3]").arg(sStat).arg(mSerie6FilteredNeighbours.size()).arg(sStatLast);
        result += QString("\n\t{%1}").arg(s5);
        result += QString("\n\t%1\n\t%2").arg(statToStringLast5(sp5Stat, INT_MAX)).arg(s6);

        return true;
    }
    else
    {
        size_t statLast[HSET_LEN] = {};
        intersect2(last, mSerie6, statLast);

        if(statLast[HSET_LEN -2] == 1)
        {
            QString sStatLast;
            hStatToString((const HSTAT)statLast, sStatLast, '|');

            result = QString("[%1]->%2\n\t[%3]").arg(sStat).arg(mSerie6.size()).arg(sStatLast);

            return true;
        }
        else
        {
            return false;
        }
    }
}

QString H55Horizon::toString04_01(HSET last) const
{
    QString sStat;
    hStatToString((const HSTAT)mStat, sStat, '|');

    size_t statLast[HSET_LEN] = {};
    intersect2(last, mSerie6FilteredNeighbours, statLast);

    QString s5;
    QString s6;
    bool found = false;
    if(statLast[HSET_LEN -2] == 1)
    {
        found = true;
        HSERIE neighbours5;
        generateSubsetMinus1(last, neighbours5, HSET_LEN);

        for(auto h5 : neighbours5)
        {
            s5 += QString("%1,").arg(
                        countBinarySearch(
                            mSeriePair5,
                            h5,
                            0, mSeriePair5.size() - 1));
        }

        QMap<size_t, size_t> stat6;
        size_t scoreLast;
        for(auto h6 : mL6FilteredNeighbours)
        {
            statAdd(stat6, h6->getScore());
            if(h6->getHSET6() == last)
            {
                scoreLast = h6->getScore();
            }
        }

        s6 += QString("%1\n\t%2").arg(scoreLast).arg(statToStringSortByValueDesc(stat6));
    }

    QString sStatLast;
    hStatToString(statLast, sStatLast, '|');

    QString result = QString("%1[%2]->%3 [%4]").arg(found ? "!" : "").arg(sStat).arg(mSerie6FilteredNeighbours.size()).arg(sStatLast);
    if(found)
    {
        result += QString("\n\t{%1}").arg(s5);

        if(!mSeriePair5.isEmpty())
        {
            QMap<size_t, size_t> sp5Stat;
            statSeriePair(mSeriePair5, sp5Stat);
            result += QString("\n\t%1").arg(statToStringLast5(sp5Stat, INT_MAX));
        }

        if(!s6.isEmpty())
        {
            result += QString("\n\t%1").arg(s6);
        }
    }

    return result;
}

QString H55Horizon::toString05() const
{
    QString sStat;
    hStatToString((const HSTAT)mStat, sStat, '|');

    QString result = QString("[%1]. %2\n\t%3").arg(sStat).arg(mSerie6FilteredNeighbours.size()).arg(toStringStatL6(mL6FilteredNeighbours));

    return result;
}

void H55Horizon::log05(const QString &date) const
{
    QString sStat4 = QString("%1%2%3%4").arg(mStat[0]).arg(mStat[1]).arg(mStat[2]).arg(mStat[3]);
    QString sFileName = QString(H55_FILE_NEXT_02).arg(date).arg(sStat4);

    HLog fout(sFileName);

    QString sStat;
    hStatToString((const HSTAT)mStat, sStat, '|');
    fout.write(QString("[%1]\n").arg(sStat));
    fout.write(QString(H55_MSG_HEADER), false);

    int index = 0;
    for(auto crt : mL6FilteredNeighbours)
    {
        fout.write(QString("%1.%2.%3\n").arg(++index).arg(crt->getScore()).arg(hsetToString3(crt->getHSET6())), false);
    }
}

////////////////////////////////////////////////////////////

H55::H55(HLog* log, const HSERIE& serie, const QString &date)
    : mLog(log), mSerie(serie), mDate(date)
{
    while(mSerie.size() > HSET_INTERSECTION_LEN)
        mSerie.removeFirst();

    mLimitMin[0] = intToHDIV(1);
    mLimitMin[1] = intToHDIV(2);
    mLimitMin[2] = intToHDIV(3);
    mLimitMin[3] = intToHDIV(10);
    mLimitMin[4] = intToHDIV(16);
    mLimitMin[5] = intToHDIV(25);

    mLimitMax[0] = intToHDIV(31);
    mLimitMax[1] = intToHDIV(33);
    mLimitMax[2] = intToHDIV(41);
    mLimitMax[3] = intToHDIV(45);
    mLimitMax[4] = intToHDIV(48);
    mLimitMax[5] = intToHDIV(49);
}

H55::~H55()
{
    for(auto crt : mHorizonsRaw)
        delete crt;
}

size_t H55::horizonsSum() const
{
    size_t result = 0;
    for(auto crt : mHorizonsRaw)
        result += crt->getSerie6().size();

    return result;
}

bool H55::check(const HSOLUTION& solution, size_t stat[HSET_LEN])
{
    if(solution.empty())
        return true;

    int indexSolution = solution.size() - 1;

    //limit min
    if(solution.top() < mLimitMin[indexSolution]) {
        return false;
    }

    //limit max
    if(solution.top() > mLimitMax[indexSolution]) {
        return false;
    }

    //combination, asc
    if(indexSolution > 0) {
        if(!(solution[indexSolution - 1] < solution[indexSolution])) {
            return false;
        }
    }

    HSET hset = generateHSET(solution);

    //block
    size_t blockStatMax = intersectBlockInterlacedCountMax(hset, mBlock, BLOCK_COUNT);
    if(blockStatMax > H55_BLOCK_STAT_MAX) {
        return false;
    }

    //interlaced
    size_t interlacedStatMax = intersectBlockInterlacedCountMax(hset, mInterlaced, BLOCK_COUNT);
    if(interlacedStatMax > H55_BLOCK_STAT_MAX) {
        return false;
    }

    if(solution.size() == HSET_LEN)
    {
        bool ok = intersect(hset, mSerie, HSET_LEN, stat);

        return ok;
    }

    return true;
}

void H55::add(const HSET hset, const size_t stat[HSET_LEN], int indexMin, int indexMax)
{
    if(mHorizonsRaw.isEmpty())
    {
        H55Horizon* mH55Horizon = new H55Horizon(mLog, mDate, stat, hset);
        mHorizonsRaw.append(mH55Horizon);

        return;
    }

    int indexMiddle = (indexMin + indexMax) >> 1;
    H55Horizon* horizon = mHorizonsRaw[indexMiddle];
    int compareMiddle = horizon->compare(stat);

    switch(compareMiddle)
    {
    case -1:
        if(indexMin == indexMiddle)
        {
            H55Horizon* mH55Horizon = new H55Horizon(mLog, mDate, stat, hset);
            mHorizonsRaw.insert(indexMiddle, mH55Horizon);
            return;
        }
        else
        {
            add(hset, stat, indexMin, indexMiddle - 1);
        }
        break;
    case 1:
        if(indexMiddle == indexMax)
        {
            H55Horizon* mH55Horizon = new H55Horizon(mLog, mDate, stat, hset);
            mHorizonsRaw.insert(indexMiddle + 1, mH55Horizon);
            return;
        }
        else
        {
            add(hset, stat, indexMiddle + 1, indexMax);
        }
        break;
    default://0
        horizon->add(hset);
        break;
    }
}

void H55::bckt(HSOLUTION& solution, size_t stat[HSET_LEN])
{
    if(solution.size() == HSET_LEN) {
        add(generateHSET(solution), stat, 0, mHorizonsRaw.size() - 1);
        return;
    }

    HDIV start = HONE;
    if(solution.size() > 0) {
        start = solution.last() << HONE;
    }

    for(HDIV div = start; div < HSET_LIMIT; div = div << HONE)
    {
        solution.push(div);
        if(check(solution, stat)) {
            bckt(solution, stat);
        }
        solution.pop();
    }
}

void H55::filterByStat(const QList<H55Horizon*>& lin , QList<H55Horizon*>& lout)
{
    for(auto crt : lin)
    {
        if(crt->filterByStat())
        {
            lout.append(crt);
        }
    }
}

void H55::run()
{
    if(mSerie.size() < H55_SERIE_SIZE_MIN)
    {
        mLog->write(H55_MSG_SERIE_SIZE_SMALL);
        return;
    }

    //block-interlaced
    generateBlockAndInterlaced(mBlock, mInterlaced);
    generateBlockInterlacedStat(mSerie, mBlock, BLOCK_COUNT, mStatBlock);
    generateBlockInterlacedStat(mSerie, mInterlaced, BLOCK_COUNT, mStatInterlaced);

    HSOLUTION solution;
    size_t stat[HSET_LEN] = {};
    bckt(solution, stat);
    mLog->write(QString("H55::run(%1).end=%2/%3\n").arg(mDate).arg(mHorizonsRaw.size()).arg(horizonsSum()));

    qSort(mHorizonsRaw.begin(), mHorizonsRaw.end(), [](const H55Horizon* x, const H55Horizon* y)
    {
        return x->compareStat(*y);
    });

    filterByStat(mHorizonsRaw, mHorizonsFilteredStat);

    for(auto crt : mHorizonsFilteredStat)
    {
        crt->neighbours();
    }

    qSort(mHorizonsFilteredStat.begin(), mHorizonsFilteredStat.end(), [](const H55Horizon* x, const H55Horizon* y)
    {
        return x->getSerie6FilteredNeighbours().size() > y->getSerie6FilteredNeighbours().size();
    });

}

QString H55::toString01() const
{
    QString result;
    int counter = 0;
    int horizonsSerie6SizeSum = 0;
    for(auto crt : mHorizonsRaw)
    {
        result += QString("%1. %2\n").arg(counter++).arg(crt->toString00());
        horizonsSerie6SizeSum += crt->getSerie6().size();
    }
    result += QString("HorizonsSizes=%1\n").arg(horizonsSerie6SizeSum);

    return result;
}

QString H55::toString02(HSET last) const
{
    QString result;
    int counter = 0;
    int horizonsSerie6SizeSum = 0;
    for(auto crt : mHorizonsRaw)
    {
        result += QString("%1. %2\n").arg(counter++).arg(crt->toString01(last));
        horizonsSerie6SizeSum += crt->getSerie6().size();
    }
    result += QString("==========\n");
    result += QString("HorizonsSizes=%1\n").arg(horizonsSerie6SizeSum);
    result += QString("==========\n");

    return result;
}

QString H55::toString03(HSET last, bool onlySuccess) const
{
    if(onlySuccess)
    {
        bool found = false;
        QString result;

        for(auto crt : mHorizonsFilteredStat)
        {
            result.clear();
            if(crt->toString02(last, result))
            {
                found = true;
                break;
            }
        }

        if(!found)
        {
            for(auto crt : mHorizonsRaw)
            {
                result.clear();
                if(crt->toString03(last, result))
                {
                    found = true;
                    break;
                }
            }
        }

        return result;
    }
    else
    {
        QString result;

        for(auto crt : mHorizonsFilteredStat)
        {
            result += QString("%1\n").arg(crt->toString04_01(last));
        }

        return result;
    }
}

QString H55::toString04(HSET last, bool onlySucces) const
{
    if(onlySucces)
    {
        bool found = false;
        QString result;

        for(auto crt : mHorizonsFilteredStat)
        {
            result.clear();
            if(crt->toString04(last, result))
            {
                found = true;
                break;
            }
        }

        if(!found)
        {
            for(auto crt : mHorizonsRaw)
            {
                result.clear();
                if(crt->toString03(last, result))
                {
                    found = true;
                    break;
                }
            }
        }

        return result;
    }
    else
    {
        QString result;
        for(auto crt : mHorizonsFilteredStat)
        {
            result += QString("%1\n").arg(crt->toString04_01(last));
        }

        return result;
    }
}

QString H55::toString05() const
{
    QString result;
    for(auto crt : mHorizonsFilteredStat)
    {
        result += QString("%1\n").arg(crt->toString05());
    }

    return result;
}

void H55::log01() const
{
    QString sLogFileName = QString(H55_FILE_NOW_01).arg(mDate);
    HLog log(sLogFileName);

    log.write(mDate + "\n");
    log.separator();

    QString sSerie;
    hserieToString(mSerie, false, sSerie);
    log.write(sSerie);
    log.separator();

    log.write(toString01());
    log.separator();
}

void H55::log05() const
{
    HLog logSummary(QString(H55_FILE_NEXT_SUMMARY_05).arg(mDate));
    logSummary.write(toString05());

    for(auto crt : mHorizonsFilteredStat)
    {
        crt->log05(mDate);
    }
}

////////////////////////////////////////////////////////////

H55UT01::H55UT01(HLog* log, const HSERIE& serie, const HSET last, const QString &date, bool fullTest)
    : mLog(log), mSerie(serie), mLast(last), mDate(date), mH55(NULL), mFullTest(fullTest)
{
    if(serie.size() >= HSET_INTERSECTION_LEN)
    {
        HSERIE history10;
        subserie(serie, serie.size() - 1, HSET_INTERSECTION_LEN, history10);
        mH55 = new H55(log, history10, date);
    }
}

H55UT01::~H55UT01()
{
    if(mH55 != NULL)
    {
        delete mH55;
        mH55 = NULL;
    }
}

void H55UT01::run()
{
    if(mH55 == NULL)
    {
        mLog->write(QString("H55UT01::run(%1).end: mH55 == NULL\n").arg(mDate));
        return;
    }

    mLog->write(QString("H55UT01::run(%1)\n").arg(mDate));
    hserieLimits(mSerie, mLimitMin, mLimitMax);

    mH55->run();

    mString03 = mH55->toString03(mLast, mFullTest);
    mString04 = mH55->toString04(mLast, mFullTest);

    if(mFullTest)
    {
       delete mH55;
       mH55 = NULL;
    }
}

QString H55UT01::toString02()
{
    if(mH55 == NULL)
    {
        return QString();
    }

    QString result = mH55->toString02(mLast);

    QString sLimitMin;
    hlimitToString(mLimitMin, false, sLimitMin);
    result += QString("limitMin=[%1]\n").arg(sLimitMin);

    QString sLimitMax;
    hlimitToString(mLimitMax, false, sLimitMax);
    result += QString("limitMax=[%1]\n").arg(sLimitMax);

    return result;
}

QString H55UT01::toString03()
{
    return mString03;
}

QString H55UT01::toString04()
{
    return mString04;
}

void H55UT01::log02()
{
    QString sLogFileName = QString(H55_UT01_FILE_02).arg(mDate);
    HLog log(sLogFileName);

    log.write(mDate + "\n");
    log.separator();

    QString sSerie;
    hserieToString(mSerie, false, sSerie);
    log.write(sSerie);
    log.separator();

    log.write(toString02());
    log.separator();
}

void H55UT01::log04()
{
    QString sLogFileName = QString(H55_UT01_FILE_04).arg(mDate);
    HLog log(sLogFileName);

    log.write(mDate + "\n");
    log.separator();

    QString sSerie;
    hserieToString(mSerie, false, sSerie);
    log.write(sSerie);
    log.separator();

    log.write(toString04());
    log.separator();
}

////////////////////////////////////////////////////////////

H55UT02::H55UT02(HLog* log, const HSERIE& serie, const QString &date)
    : mLog(log), mSerie(serie), mDate(date)
{
}

H55UT02::~H55UT02()
{
    for(auto crt : mLH55UT01)
        delete crt;
}

void H55UT02::run()
{
    for(int index = HSET_INTERSECTION_LEN; index < mSerie.size(); ++index)
    {
        HSERIE s11;
        subserie(mSerie, index, HSET_INTERSECTION_LEN + 1, s11);

        HSERIE s10;
        HSET last;
        serieLast(s11, s10, last);

        H55UT01* h55UT01 = new H55UT01(mLog, s10, last, QString("%1").arg(index), true);
        mLH55UT01.append(h55UT01);
    }

    QObject threadPoolParent;
    QThreadPool threadPool(&threadPoolParent);
    threadPool.setMaxThreadCount(H55_THREAD_SIZE);
    for(auto crt: mLH55UT01)
    {
        crt->setAutoDelete(false);
        threadPool.start(crt);
    }
    threadPool.waitForDone();


    for(auto crt: mLH55UT01)
    {
        mResult03 += QString("%1. %2\n").arg(crt->getDate()).arg(crt->toString03());
        mResult04 += QString("%1. %2\n").arg(crt->getDate()).arg(crt->toString04());
    }
}

void H55UT02::log03()
{
    QString sLogFileName = QString(H55_UT02_FILE_03).arg(mDate);
    HLog log(sLogFileName);

    log.write(mDate + "\n");
    log.separator();

    QString sSerie;
    hserieToString(mSerie, false, sSerie);
    log.write(sSerie);
    log.separator();

    log.write(mResult03);
    log.separator();
}

void H55UT02::log04()
{
    QString sLogFileName = QString(H55_UT02_FILE_04).arg(mDate);
    HLog log(sLogFileName);

    log.write(mDate + "\n");
    log.separator();

    QString sSerie;
    hserieToString(mSerie, false, sSerie);
    log.write(sSerie);
    log.separator();

    log.write(mResult04);
    log.separator();
}
