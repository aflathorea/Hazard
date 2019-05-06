#include "h55.h"

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
        mScore = MAX(mScore, mHSet5Score[index]);
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

H55Horizon::H55Horizon(HLog *log, const string& parent, const size_t stat[], const HSET hset)
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
    string sStat = statToStringLine(mStat);

    mLog->write(toStringFormatS2("H55Horizon::neighbours(%s. %s)\n", mParent, sStat));

    for(HSET hset6 : mSerie6)
    {
        HSERIE neighbours5;
        generateSubsetMinus1(hset6, neighbours5, HSET_LEN);

        H55HSet6* h55hset6 = new H55HSet6(hset6,neighbours5);
        mL6Raw.push_back(h55hset6);

        serieAdd(mSerie5, neighbours5);
    }

    sort(mSerie5.begin(), mSerie5.end());
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

    sort(mL6Raw.begin(), mL6Raw.end(), [](const H55HSet6* x, const H55HSet6* y)
    {
        return x->getScore() > y->getScore();
    });

    filterByNeighbours(mL6Raw, mL6FilteredNeighbours);
    for(auto crt : mL6FilteredNeighbours)
    {
        mSerie6FilteredNeighbours.push_back(crt->getHSET6());
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

void H55Horizon::filterByNeighbours(const deque<H55HSet6*>& lin, deque<H55HSet6*>& lout)
{
    map<size_t, size_t> stat6;
    for(auto crt : mL6Raw)
    {
        statAdd(stat6, crt->getScore());
    }

    deque<pair<size_t, size_t>> l6Score;
    for(auto it : stat6)
    {
        l6Score.push_back(pair<size_t, size_t>(it.first, it.second));
    }

    sort(l6Score.begin(), l6Score.end(), [](const pair<size_t, size_t>& x, const pair<size_t, size_t>& y)
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
            lout.push_back(h6);
        }
    }

    sort(lout.begin(), lout.end(), [](const H55HSet6* x, const H55HSet6* y){
        return x->getScore() > y->getScore();
    });
}

string H55Horizon::toStringStatL6(const deque<H55HSet6*>& l6) const
{
    map<size_t, size_t> stat6;
    for(auto h6 : l6)
    {
        statAdd(stat6, h6->getScore());
    }

    return statToStringSortByValueDesc(stat6);
}

string H55Horizon::toString00() const
{
    string sStat = statToStringLine(mStat);

    map<size_t, size_t> sp5Stat;
    statSeriePair(mSeriePair5, sp5Stat);

    char sText[SPEEDLOW_SPRINTF_LEN] = {};
    sprintf(sText, "[%s]->%lu\n\t%s", sStat.data(), mSerie6.size(), statToStringLast5(sp5Stat, INT_MAX).data());
    string result(sText);

    return result;
}

string H55Horizon::toString01(HSET last) const
{
    string sStat = statToStringLine(mStat);

    size_t statLast[HSET_LEN] = {};
    intersect2(last, mSerie6, statLast);

    string s5;
    string s6;
    bool found = false;
    if(statLast[HSET_LEN -2] == 1)
    {
        found = true;
        HSERIE neighbours5;
        generateSubsetMinus1(last, neighbours5, HSET_LEN);

        for(auto h5 : neighbours5)
        {
            s5 += countBinarySearch(
                            mSeriePair5,
                            h5,
                            0, mSeriePair5.size() - 1) + ",";
        }

        map<size_t, size_t> stat6;
        size_t scoreLast;
        for(auto h6 : mL6Raw)
        {
            statAdd(stat6, h6->getScore());
            if(h6->getHSET6() == last)
            {
                scoreLast = h6->getScore();
            }
        }

        s6 += to_string(scoreLast) + "\n\t" + statToStringSortByValueDesc(stat6);
    }

    string sStatLast = statToStringLine(statLast);

    string result = toStringFormatS2("%s%s", (found ? "!" : ""), sStat) + "->" + to_string(mSerie6.size()) + " " + sStatLast;
    if(found)
    {
        result += "\n\t{" + s5 + "}";
    }

    if(!mSeriePair5.empty())
    {
        map<size_t, size_t> sp5Stat;
        statSeriePair(mSeriePair5, sp5Stat);
        result += "\n\t" + statToStringLast5(sp5Stat, INT_MAX);
    }

    if(!s6.empty())
    {
        result += "\n\t" + s6;
    }

    return result;
}

bool H55Horizon::toString02(HSET last, string& result) const
{
    string sStat = statToStringLine(mStat);

    size_t statLast[HSET_LEN] = {};
    intersect2(last, mSerie6, statLast);

    if(statLast[HSET_LEN -2] == 1)
    {
        string s5;
        string s6;

        HSERIE neighbours5;
        generateSubsetMinus1(last, neighbours5, HSET_LEN);

        for(auto h5 : neighbours5)
        {
            s5 += countBinarySearch(
                    mSeriePair5,
                    h5,
                    0, mSeriePair5.size() - 1) + ",";
        }

        map<size_t, size_t> stat6;
        size_t scoreLast;
        for(auto h6 : mL6Raw)
        {
            statAdd(stat6, h6->getScore());
            if(h6->getHSET6() == last)
            {
                scoreLast = h6->getScore();
            }
        }

        s6 += scoreLast + "\n\t" + statToStringSortByValueDesc(stat6);

        string sStatLast = statToStringLine(statLast);

        map<size_t, size_t> sp5Stat;
        statSeriePair(mSeriePair5, sp5Stat);

        result = sStat + "->" + to_string(mSerie6.size()) + " " + sStatLast;
        result += "\n\t{" + s5 + "}";
        result += "\n\t" + statToStringLast5(sp5Stat, INT_MAX) + "\n\t" + s6;

        return true;
    }
    else
    {
        return false;
    }
}

bool H55Horizon::toString03(HSET last, string& result) const
{
    string sStat = statToStringLine(mStat);

    size_t statLast[HSET_LEN] = {};
    intersect2(last, mSerie6, statLast);

    if(statLast[HSET_LEN -2] == 1)
    {
        string sStatLast = statToStringLine(statLast);

        result = sStat + "->" + to_string(mSerie6.size()) + "\n\t" + sStatLast;

        return true;
    }
    else
    {
        return false;
    }
}

bool H55Horizon::toString04(HSET last, string& result) const
{
    string sStat = statToStringLine(mStat);

    size_t statLast[HSET_LEN] = {};
    intersect2(last, mSerie6FilteredNeighbours, statLast);

    if(statLast[HSET_LEN -2] == 1)
    {
        string s5;
        string s6;

        HSERIE neighbours5;
        generateSubsetMinus1(last, neighbours5, HSET_LEN);

        for(auto h5 : neighbours5)
        {
            s5 += countBinarySearch(
                    mSeriePair5,
                    h5,
                    0, mSeriePair5.size() - 1) + ",";
        }

        map<size_t, size_t> stat6;
        size_t scoreLast;
        for(auto h6 : mL6FilteredNeighbours)
        {
            statAdd(stat6, h6->getScore());
            if(h6->getHSET6() == last)
            {
                scoreLast = h6->getScore();
            }
        }

        s6 += scoreLast + "\n\t" + statToStringSortByValueDesc(stat6);

        string sStatLast = statToStringLine(statLast);

        map<size_t, size_t> sp5Stat;
        statSeriePair(mSeriePair5, sp5Stat);

        result = sStat + "->" + to_string(mSerie6FilteredNeighbours.size()) + " " + sStatLast;
        result += "\n\t{" + s5 + "}";
        result += "\n\t" + statToStringLast5(sp5Stat, INT_MAX) + "\n\t" + s6;

        return true;
    }
    else
    {
        size_t statLast[HSET_LEN] = {};
        intersect2(last, mSerie6, statLast);

        if(statLast[HSET_LEN -2] == 1)
        {
            string sStatLast = statToStringLine(statLast);

            result = sStat + "->" + to_string(mSerie6.size()) + "\n\t" + sStatLast;

            return true;
        }
        else
        {
            return false;
        }
    }
}

string H55Horizon::toString04_01(HSET last) const
{
    string sStat = statToStringLine(mStat);

    size_t statLast[HSET_LEN] = {};
    intersect2(last, mSerie6FilteredNeighbours, statLast);

    string s5;
    string s6;
    bool found = false;
    if(statLast[HSET_LEN -2] == 1)
    {
        found = true;
        HSERIE neighbours5;
        generateSubsetMinus1(last, neighbours5, HSET_LEN);

        for(auto h5 : neighbours5)
        {
            s5 += countBinarySearch(
                    mSeriePair5,
                    h5,
                    0, mSeriePair5.size() - 1) + ",";
        }

        map<size_t, size_t> stat6;
        size_t scoreLast;
        for(auto h6 : mL6FilteredNeighbours)
        {
            statAdd(stat6, h6->getScore());
            if(h6->getHSET6() == last)
            {
                scoreLast = h6->getScore();
            }
        }

        s6 += to_string(scoreLast) + "\n\t" + statToStringSortByValueDesc(stat6);
    }

    string sStatLast = statToStringLine(statLast);

    string result = (found ? "!" : "") + sStat + "->" + to_string(mSerie6FilteredNeighbours.size()) + " " + sStatLast;
    if(found)
    {
        result += "\n\t{" + s5 + "}";

        if(!mSeriePair5.empty())
        {
            map<size_t, size_t> sp5Stat;
            statSeriePair(mSeriePair5, sp5Stat);
            result += "\n\t" + statToStringLast5(sp5Stat, INT_MAX);
        }

        if(!s6.empty())
        {
            result += "\n\t" + s6;
        }
    }

    return result;
}

string H55Horizon::toString05() const
{
    string sStat = statToStringLine(mStat);

    string result = sStat + ". " + to_string(mSerie6FilteredNeighbours.size()) + "\n\t" + toStringStatL6(mL6FilteredNeighbours);

    return result;
}

void H55Horizon::log05(const string &date) const
{
    string sStat4 = to_string(mStat[0]) + to_string(mStat[1]) + to_string(mStat[2]) + to_string(mStat[3]);
    string sFileName = toStringFormatS2(H55_FILE_NEXT_02, date, sStat4);

    HLog fout(sFileName);

	char sText[SPEEDLOW_SPRINTF_LEN] = {};
    sprintf(sText, "H55Horizon::log05(%s)\n", sFileName.data());
	printf(sText);

    string sStat = statToStringLine(mStat);
    fout.write(sStat + "\n", false);
    fout.write(string(H55_MSG_HEADER), false);

    int index = 0;
    for(auto crt : mL6FilteredNeighbours)
    {
        fout.write(to_string(++index) + "-" + to_string(crt->getScore()) + ". " + hsetToString(crt->getHSET6()) + "\n", false);
    }
}

////////////////////////////////////////////////////////////

H55::H55(HLog* log, const HSERIE& serie, const string &date)
    : mLog(log), mSerie(serie), mDate(date)
{
    while(mSerie.size() > HSET_INTERSECTION_LEN)
        mSerie.pop_front();

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

    int indexSolution = (int)solution.size() - 1;

    //limit min
    if(solution.back() < mLimitMin[indexSolution]) {
        return false;
    }

    //limit max
    if(solution.back() > mLimitMax[indexSolution]) {
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
        statInit(stat);
        bool ok = intersect(hset, mSerie, stat);

        return ok;
    }

    return true;
}

void H55::add(const HSET hset, const size_t stat[HSET_LEN], int indexMin, int indexMax)
{
    if(mHorizonsRaw.empty())
    {
        H55Horizon* mH55Horizon = new H55Horizon(mLog, mDate, stat, hset);
        mHorizonsRaw.push_back(mH55Horizon);

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
            mHorizonsRaw.insert(mHorizonsRaw.begin() + indexMiddle, mH55Horizon);
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
            mHorizonsRaw.insert(mHorizonsRaw.begin() + indexMiddle + 1, mH55Horizon);
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
        start = solution.back() << HONE;
    }

    for(HDIV div = start; div < HSET_LIMIT; div = div << HONE)
    {
        solution.push_back(div);
        if(check(solution, stat)) {
            bckt(solution, stat);
        }
        solution.pop_back();
    }
}

void H55::filterByStat(const deque<H55Horizon*>& lin , deque<H55Horizon*>& lout)
{
    for(auto crt : lin)
    {
        if(crt->filterByStat())
        {
            lout.push_back(crt);
        }
    }
}

void H55::filterByOrder(const deque<H55Horizon*>& lin , deque<H55Horizon*>& lout)
{
	int index = 0;
    for(auto crt : lin)
    {
        if(index < 20)
        {
            lout.push_back(crt);
        }
		else
		{
			break;
		}
		index++;
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

	char sBcktStart[SPEEDLOW_SPRINTF_LEN] = {};
    sprintf(sBcktStart, "H55::bckt(%s).start.\n", mDate.data());
	printf(sBcktStart);

	HSOLUTION solution;
    size_t stat[HSET_LEN] = {};
    bckt(solution, stat);

    char sBcktEnd[SPEEDLOW_SPRINTF_LEN] = {};
    sprintf(sBcktEnd, "H55::bckt(%s).end=%lu/%lu\n", mDate.data(), mHorizonsRaw.size(), horizonsSum());
    mLog->write(sBcktEnd);

    sort(mHorizonsRaw.begin(), mHorizonsRaw.end(), [](const H55Horizon* x, const H55Horizon* y)
    {
        //return x->compareStat(*y);
		return x->compareSize(*y);
    });

    filterByOrder(mHorizonsRaw, mHorizonsFilteredStat);

    HThreadPool hThreadPool;
    for(auto crt : mHorizonsFilteredStat) {
        hThreadPool.push_back(crt);
    }
    hThreadPool.run();

    sort(mHorizonsFilteredStat.begin(), mHorizonsFilteredStat.end(), [](const H55Horizon* x, const H55Horizon* y)
    {
        return x->getSerie6FilteredNeighbours().size() > y->getSerie6FilteredNeighbours().size();
    });
}

string H55::toString01() const
{
    string result;
    int counter = 0;
    size_t horizonsSerie6SizeSum = 0;
    for(auto crt : mHorizonsRaw)
    {
        result += to_string(counter++) + ". " + crt->toString00() + "\n";
        horizonsSerie6SizeSum += crt->getSerie6().size();
    }
    result += toStringFormatUL2("HorizonsSizes=%lu\n", horizonsSerie6SizeSum, 0);

    return result;
}

string H55::toString02(HSET last) const
{
    string result;
    int counter = 0;
    size_t horizonsSerie6SizeSum = 0;
    for(auto crt : mHorizonsRaw)
    {
        result += to_string(counter++) + ". " + crt->toString01(last) + "\n";
        horizonsSerie6SizeSum += crt->getSerie6().size();
    }
    result += string("==========\n");
    result += "HorizonsSizes=" + to_string(horizonsSerie6SizeSum) + "\n";
    result += string("==========\n");

    return result;
}

string H55::toString03(HSET last, bool onlySuccess) const
{
    if(onlySuccess)
    {
        bool found = false;
        string result;

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
        string result;

        for(auto crt : mHorizonsFilteredStat)
        {
            result += crt->toString04_01(last) + "\n";
        }

        return result;
    }
}

string H55::toString04(HSET last, bool onlySucces) const
{
    if(onlySucces)
    {
        bool found = false;
        string result;

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
        string result;
        for(auto crt : mHorizonsFilteredStat)
        {
            result += crt->toString04_01(last) + "\n";
        }

        return result;
    }
}

string H55::toString05() const
{
    string result;
    for(auto crt : mHorizonsFilteredStat)
    {
        result += crt->toString05() + "\n";
    }

    return result;
}

void H55::log01() const
{
    mLog->write(toString01());
    mLog->separator();
}

void H55::log05() const
{
    mLog->write(toString05());

    for(auto crt : mHorizonsFilteredStat)
    {
        crt->log05(mDate);
    }
}

////////////////////////////////////////////////////////////

H55UT01::H55UT01(HLog* log, const HSERIE& serie, const HSET last, const string &date, bool fullTest)
    : mLog(log), mSerie(serie), mLast(last), mDate(date), mH55(NULL), mFullTest(fullTest)
{
    limitInit(mLimitMin);
    limitInit(mLimitMax);

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
		mLog->write(H55_MSG_HISTORY_SIZE_SMALL);
        //mLog->write(toStringFormatS2("H55UT01::run(%s).end: mH55 == NULL\n", mDate, ""));
		
        return;
    }

    mLog->write(toStringFormatS2("H55UT01::run(%s)\n", mDate, ""));
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

string H55UT01::toString02()
{
    if(mH55 == NULL)
    {
        return string();
    }

    string result = mH55->toString02(mLast);
    result += "limitMin=" + hlimitToString(mLimitMin) + "\n";
    result += "limitMax=" + hlimitToString(mLimitMax) + "\n";

    return result;
}

string H55UT01::toString03()
{
    return mString03;
}

string H55UT01::toString04()
{
    return mString04;
}

void H55UT01::log02()
{
    mLog->write(toString02());
    mLog->separator();
}

void H55UT01::log04()
{
    mLog->write(toString04());
    mLog->separator();
}

////////////////////////////////////////////////////////////

H55UT02::H55UT02(HLog* log, const HSERIE& serie, const string &date)
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

        H55UT01* h55UT01 = new H55UT01(mLog, s10, last, to_string(index), true);
        mLH55UT01.push_back(h55UT01);
    }

    HThreadPool hThreadPool;
    for(auto crt: mLH55UT01) {
        hThreadPool.push_back(crt);
    }
    hThreadPool.run();

    for(auto crt: mLH55UT01)
    {
        mResult03 += crt->getDate() + ". " + crt->toString03() + "\n";
        mResult04 += crt->getDate() + ". " + crt->toString04() + "\n";
    }
}

void H55UT02::log03()
{
    mLog->write(mResult03);
    mLog->separator();
}

void H55UT02::log04()
{
    mLog->write(mResult04);
    mLog->separator();
}
