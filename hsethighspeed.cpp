#include "hsethighspeed.h"


bool compareNeighboursIntersection(const QPair<HSTAT, size_t>* x, const QPair<HSTAT, size_t>* y)
{
    return x->second > y->second;
}

HSET generateHSET(const HSOLUTION& solution)
{
    HSET result = 0;

    QVectorIterator<HDIV> it(solution);
    while(it.hasNext())
        result |= it.next();

    return result;
}

void generateHSerie5(const HSOLUTION& solution, HSERIE& serie5)
{
    for(int i1 = 0; i1 < solution.size(); ++i1) {
        HSET hset = 0;
        for(int i2 = 0; i2 < solution.size(); ++i2) {
            if(i1 != i2) {
                hset |= solution[i2];
            }
        }
        serie5.append(hset);
    }
}

HSET generateHSET(HSPLIT list, HDIV div1)
{
    HSET result = div1;

    for(int index = 0; index < HSET_LEN; index++)
    {
        HDIV div2 = list[index];
        result |= div2;
    }

    return result;
}

HSET generateHSETOneFull()
{
    HSET result = HZERO;

    for(HDIV div = HONE; div < (HONE << HDIV_MAX); div = div << HONE)
    {
        result |= div;
    }

    return result;
}

HSET hsetReverse(HSET hset)
{
    HSET result = hset ^ generateHSETOneFull();

    return result;
}

bool hsetContains(HSET big, HSET small)
{
    bool result = ((big & small) == small);

    return result;
}

void bckt1(HSOLUTION& solution)
{
//    cout << qPrintable(QString::number(sizeof(HSET), 10)) << endl;
//    cout << qPrintable(hsetToString(limit, true)) << endl;

    if(solution.size() >= HSET_LEN)
    {
        //HSET hset = generateHSET(solution);
        //cout << qPrintable(hsetToString(hset, false)) << endl;
        //cout << qPrintable(hsetToString(hset, true)) << endl;
        return;
    }

    HDIV start = HONE;
    if(solution.size() > 0)
    {
        start = solution.last() << HONE;
    }

    for(HDIV div = start; div < HSET_LIMIT; div = div << HONE)
    {
        solution.push(div);
        if(true)//check(solution)
        {
            bckt1(solution);
        }
        solution.pop();
    }
}

void splitHSET(HSET hset, HSPLIT result)
{
    memset(result, 0, sizeof(HDIV) * HSET_LEN);

    int index = 0;
    for(HDIV div = HONE; div <= hset; div = div << HONE)
    {
        if(div & hset)
        {
            result[index] = div;
            index++;
        }
    }
}

void splitHSET(HSET hset, QList<HDIV>& result)
{
    for(HDIV div = HONE; div < (HONE << HDIV_MAX); div = div << HONE)
    {
        if(div & hset)
        {
            result.append(div);
        }
    }

    qSort(result);
}

void splitHSET(HSET hset, HSOLUTION &result)
{
    for(HDIV div = HONE; div < (HONE << HDIV_MAX); div = div << HONE)
    {
        if(div & hset)
        {
            result.push(div);
        }
    }
}

void generateNeighbours(HSET hset, HSERIE& result)
{
    HDIV originals[HSET_LEN] = {};
    splitHSET(hset, originals);

    for(int index = 0; index < HSET_LEN; index++)
    {
        HDIV subset[HSET_LEN] = {}; //HSET_LEN-1, the last is not used
        generateSubset(originals, index, subset);

        for(HDIV newDiv = HONE; newDiv < HSET_LIMIT; newDiv = newDiv << HONE)
        {
            if(!equals(newDiv, originals))
            {
                HSET neighbour = generateHSET(subset, newDiv);
                result.append(neighbour);
            }
        }
    }
}

void intersectNeighbours(const HSERIE& neighbours, const HSERIE &serie, QList<QPair<HSTAT, size_t>*>& result)
{
    for(HSERIE::const_iterator it = neighbours.begin(); it != neighbours.end(); ++it)
    {
        HSET hset = *it;

        size_t stat[HSET_LEN] = {};
        intersect(hset, serie, HSET_LEN, stat);

        bool found = false;
        for(QList<QPair<HSTAT, size_t>*>::iterator it = result.begin(); it != result.end(); ++it)
        {
            QPair<HSTAT, size_t>* pair = *it;
            if(equals(pair->first, stat))
            {
                pair->second++;
                found = true;
                break;
            }
        }

        if(!found)
        {
            HSTAT newStat = new size_t[HSET_LEN];
            copy(stat, newStat);

            QPair<HSTAT, size_t>* newPair = new QPair<HSTAT, size_t>(newStat, 1);
            result.append(newPair);
        }
    }

    qSort(result.begin(), result.end(), compareNeighboursIntersection);
}

bool equals(HDIV div, HSPLIT list)
{
    for(int index = 0; index < HSET_LEN; index++)
    {
        HDIV crt = list[index];

        if(crt == 0)
            continue;

        if(div == crt)
            return true;
    }

    return false;
}

bool equals(HSTAT const x, HSTAT const y)
{
    return equals(x, y, HSET_LEN);
}

bool equals(HSTAT const x, HSTAT const y, size_t max)
{
    if(x == NULL && y == NULL)
        return true;

    if(x == NULL || y == NULL)
        return false;

    for(size_t index = 0; index < max; index++)
    {
        if(x[index] != y[index])
            return false;
    }

    return true;
}

bool equals(const HSERIE& x, const HSERIE& y)
{
    if(x.size() != y.size())
        return false;

    for(auto it1 = x.begin(), it2 = y.begin(); it1 != x.end(), it2 != y.end(); ++it1, ++it2 ) {
        if(*it1 != *it2)
            return false;
    }

    return true;
}

void generateSubset(HSPLIT const original, int nonIndex, HSPLIT result)
{
    memset(result, 0, sizeof(HDIV) * HSET_LEN);

    int index2 = 0;
    for(int index1 = 0; index1 < HSET_LEN; index1++)
    {
        if(index1 != nonIndex)
        {
            result[index2] = original[index1];
            index2++;
        }
    }
}

///
/// \brief generateSubsetMinus1
/// \param hset
/// \param result
/// \param len is the input value: 6->5 is 6, 5->4 is 4.
///
void generateSubsetMinus1(HSET hset, HSERIE &result, int len)
{
    HDIV divs[HSET_LEN] = {};
    splitHSET(hset, divs);

    for(int index = 0; index < len; index++)
    {
        HDIV divs2[HSET_LEN] = {};
        memcpy(divs2, divs, sizeof(HDIV) * HSET_LEN);
        divs2[index] = 0;

        HSET hset2 = generateHSET(divs2);
        result.append(hset2);
    }
}

///
/// \brief generateSerieMinus1
/// \param serieIn
/// \param hsetLen: is the input value; 6->5 is 6, for 5->4 is 5
/// \param serieOut
///
void generateSerieMinus1(const HSERIE& serieIn, int hsetLen, HSERIE& serieOut)
{
    for(HSET hsetCrt : serieIn) {
        HSERIE serieMinus1;
        generateSubsetMinus1(hsetCrt, serieMinus1, hsetLen);

        for(HSET hsetMinus1 : serieMinus1) {
            serieOut.append(hsetMinus1);
        }
    }

    qSort(serieOut.begin(), serieOut.end());
}

void generateSerieMinus1Unique(const HSERIE& serieIn, HSERIE& serieOut)
{
    HSET last = 0;
    foreach(HSET crt, serieIn) {
        if(crt != last) {
            serieOut.append(crt);
        }

        last = crt;
    }
}

size_t hsetBitCount(HSET hset)
{
    size_t result = 0;

    while(hset)
    {
        result++;
        hset &= hset - 1;
    }

    return result;
}

size_t hdivToNumber(HDIV hdiv)
{
    size_t result = 0;

    while(hdiv)
    {
        result++;
        hdiv = hdiv >> 1;
    }

    return result;
}

HDIV hdivFromNumber(int number)
{
    return HONE << number;
}

void hsplitToIndexes(HSPLIT split, int* result)
{
    for(int indexSplit = 0; indexSplit < HSET_LEN; ++indexSplit)
    {
        result[indexSplit] = hdivToNumber(split[indexSplit]) - 1;
    }
}

void hsplitFromIndexes(int* indexes, HSPLIT hsplit)
{
    for(int indexSplit = 0; indexSplit < HSET_LEN; ++indexSplit)
    {
        hsplit[indexSplit] = hdivFromNumber(indexes[indexSplit]);
    }
}

HSET hserieUnion(const HSERIE& serie)
{
    HSET result = HZERO;

    for(HSERIE::const_iterator it = serie.begin(); it != serie.end(); it++)
    {
        HSET crt = *it;
        result |= crt;
    }

    return result;
}

size_t hserieUnionCount(const HSERIE& serie)
{
    HSET serieUnion = hserieUnion(serie);
    size_t result = hsetBitCount(serieUnion);

    return result;
}

bool intersect(HSET hset, const HSERIE& list, size_t max, HSTAT stat)
{
    bool result = true;

    memset(stat, 0, sizeof(size_t) * HSET_LEN);

    for(int index = 0; index < list.size(); index++)
    {
        HSET hsetIntersection = hset & list.at(index);
        size_t bitCount = hsetBitCount(hsetIntersection);

        if(bitCount < max)
        {
            stat[bitCount]++;
        }
        else
        {
            result = false;
        }
    }

    return result;
}

void intersect2(HSET hset, const HSERIE& list, HSTAT stat)
{
    memset(stat, 0, sizeof(size_t) * HSET_LEN);

    for(int index = 0; index < list.size(); index++)
    {
        HSET hsetIntersection = hset & list.at(index);
        size_t bitCount = hsetBitCount(hsetIntersection);

        if(bitCount < SERIES_STAT_INTERSECTION_MIN)
            continue;

        stat[bitCount - SERIES_STAT_INTERSECTION_MIN]++;
    }
}

void intersect2(HSET hset, const HSET* serie, int serieSize, HSTAT stat)
{
    memset(stat, 0, sizeof(size_t) * HSET_LEN);

    for(int index = 0; index < serieSize; index++)
    {
        HSET hsetIntersection = hset & serie[index];
        size_t bitCount = hsetBitCount(hsetIntersection);

        if(bitCount < SERIES_STAT_INTERSECTION_MIN)
            continue;

        stat[bitCount - SERIES_STAT_INTERSECTION_MIN]++;
    }
}

void intersect(const HSERIE& x, const HSERIE& y, HSTAT stat)
{
    memset(stat, 0, sizeof(size_t) * HSET_LEN);

    for(int index1 = 0; index1 < x.size(); index1++)
    {
        HSET hsetX = x.at(index1);

        for(int index2 = 0; index2 < SERIES_STAT_INTERSECTION_RND; index2++)
        {
            int index3 = qrand() % y.size();
            HSET hsetY = y.at(index3);

            HSET intersection = hsetX & hsetY;
            size_t bitCount = hsetBitCount(intersection);

            if(bitCount < SERIES_STAT_INTERSECTION_MIN)
                continue;

            stat[bitCount - SERIES_STAT_INTERSECTION_MIN]++;
        }
    }
}

//intersect the common 6 divs of a hset; y should be sort
void intersect6(const HSERIE& x, const HSERIE& y, HSERIE& result)
{
    if(x.size() > y.size()) {
        intersect6(y, x, result);
        return;
    }

    for(HSERIE::const_iterator it1 = x.begin(); it1 != x.end(); ++it1)
    {
        HSET hsetX = *it1;

        HSERIE::const_iterator it2 = qBinaryFind(y.begin(), y.end(), hsetX);

        if(it2 != y.end()) {
            result.append(hsetX);
        }
    }
}

void intersect7(HSET hset, const HSERIE& list, HSTAT stat)
{
    for(int index = 0; index < HSET_LEN + 1; ++index) {
        stat[index] = 0;
    }

    for(int index = 0; index < list.size(); index++)
    {
        HSET hsetIntersection = hset & list.at(index);
        size_t bitCount = hsetBitCount(hsetIntersection);
        stat[bitCount]++;
    }
}

//like intersect 6. result = x - y; y should be sort
void diff6(const HSERIE& x, const HSERIE& y, HSERIE& result)
{
    for(HSERIE::const_iterator it1 = x.begin(); it1 != x.end(); ++it1)
    {
        HSET hsetX = *it1;

        HSERIE::const_iterator it2 = qBinaryFind(y.begin(), y.end(), hsetX);

        if(it2 == y.end()) {
            result.append(hsetX);
        }
    }
}

void hserieLimits(const HSERIE& list, HLIMIT min, HLIMIT max)
{
    memset(min, 255, sizeof(HDIV) * HSET_LEN);
    memset(max, 0, sizeof(HDIV) * HSET_LEN);

    for(int index1 = 0 ; index1 < list.size(); index1++)
    {
        HSET hset = list.at(index1);

        HDIV divs[HSET_LEN] = {};
        splitHSET(hset, divs);

        for(size_t index2 = 0; index2 < HSET_LEN; index2++)
        {
            HDIV div = divs[index2];

            if(div < min[index2])
            {
                min[index2] = div;
            }

            if(div > max[index2])
            {
                max[index2] = div;
            }
        }
    }
}

int intersectionMax(const HSTAT stat)
{
    int result = 0;

    for(int index = 0; index < HSET_LEN; index++)
    {
        if(stat[index] > 0)
            result = index;
    }

    return result;
}

void copySplit(HSPLIT source, HSPLIT dest)
{
    for(int index = 0; index < HSET_LEN; index++)
    {
        dest[index] = source[index];
    }
}

size_t intersectBlockInterlacedCountMax(HSET hset, HDIV* blockInterlaced, int blockInterlacedLen)
{
    size_t result = 0;

    for(int index = 0; index < blockInterlacedLen; index++)
    {
        HSET intersection = hset & blockInterlaced[index];
        size_t count = hsetBitCount(intersection);
        if(count > result)
            result = count;
    }

    return result;
}

bool hsetSortByFirstAndLast(const HSET& x, const HSET& y)
{
    HDIV splitX[HSET_LEN] = {};
    splitHSET(x, splitX);

    HDIV splitY[HSET_LEN] = {};
    splitHSET(y, splitY);

    for(int index1 = 0; index1 < HSET_LEN; index1++)
    {
        int index2 = index1;

        if(index1 == 1)
            index2 = HSET_LEN - 1;
        else if(index1 > 1)
            index2 = index1 - 1;

        HDIV divX = splitX[index2];
        HDIV divY = splitY[index2];

        if(divX == divY)
        {
            continue;
        }
        else
        {
            return divX < divY;
        }
    }

    return true;
}

void hserieSortByFirstAndLast(HSERIE& serie)
{
    qSort(serie.begin(), serie.end(), hsetSortByFirstAndLast);
}

void shuffle(HSERIE& serie, int repeatCount)
{
    for(int indexRepeat = 0; indexRepeat < repeatCount; indexRepeat++)
    {
        int index1 = qrand() % serie.size();
        int index2 = qrand() % serie.size();

        HSET temp = serie[index1];
        serie[index1] = serie[index2];
        serie[index2] = temp;
    }
}

void hstatAdd(HSTAT first, HSTAT second)
{
    for(int index  = 0; index < HSET_LEN; ++index)
    {
        first[index] += second[index];
    }
}

bool hEqualFuzzy(size_t x, size_t y, float aproximatePercent)
{
    size_t approximate = 0;
    size_t diff = 0;
    if(x > y)
    {
        approximate = (size_t)((float)x * aproximatePercent);
        diff = x - y;
    }
    else
    {
        approximate = (size_t)(float)y * aproximatePercent;
        diff = y - x;
    }

    bool result = (diff <= approximate);

    return result;
}

bool hEqualFuzzy2(size_t x, size_t y, size_t aproximate)
{
    if(x > y)
    {
        return x <= y + aproximate;
    }
    else
    {
        return y <= x + aproximate;
    }
}

size_t doPercent(size_t x, size_t y)
{
    if(y == 0)
        return 0;

    size_t result = (size_t)((float)x / (float)y * 100.0f);

    return result;
}

void copy(const HSTAT source, HSTAT dest)
{
    for(int index = 0; index < HSET_LEN; index++)
        dest[index] = source[index];
}

bool contains(const QList<HSTAT>& stats, HSTAT stat)
{
    for(QList<HSTAT>::const_iterator it = stats.begin(); it != stats.end(); ++it)
    {
        HSTAT statCrt = *it;
        if(equals(stat, statCrt))
            return true;
    }

    return false;
}

bool statCompare(HSTAT const x, HSTAT const y)
{
    if(x == NULL || y == NULL)
        return true;

    for(int index = 0; index < HSET_LEN; index++)
    {
        if(x[index] == y[index])
            continue;
        else
            return x[index] < y[index];
    }

    return true;
}

void generateBlockAndInterlaced(int divMax, const int& blocksCount, int& interlacedCount, HSET*& blocks, HSET*& interlaced)
{
    if(interlacedCount == 0)
    {
        interlacedCount = divMax / blocksCount;
        if(interlacedCount * blocksCount < divMax)
            interlacedCount++;
    }

    blocks = new HSET[blocksCount];
    memset(blocks, 0, sizeof(HSET)*blocksCount);

    for(int index = 0; index < blocksCount; index++)
    {
        blocks[index] = generateBlock(divMax, blocksCount, index);
    }

    interlaced = new HSET[interlacedCount];
    memset(interlaced, 0, sizeof(HSET)*interlacedCount);

    for(int index = 0; index < interlacedCount; index++)
    {
        interlaced[index] = generateInterlaced(divMax, interlacedCount, index);
    }
}

void generateBlockAndInterlaced(HSET blocks[BLOCK_COUNT], HSET interlaced[BLOCK_COUNT])
{
    for(int index = 0; index < BLOCK_COUNT; index++)
    {
        blocks[index] = generateBlock(HDIV_MAX, BLOCK_COUNT, index);
        interlaced[index] = generateInterlaced(HDIV_MAX, BLOCK_COUNT, index);
    }
}

HSET generateBlock(int divMax, int blocksCount, int index)
{
    int len = divMax / blocksCount;
    if(len * blocksCount < divMax)
        len++;

    int offset = len * index;

    HSET result = 0;

    for(int index2 = 0; index2 < len; index2++)
    {
        int shift = offset + index2;
        if(shift <= divMax)
        {
            HSET one = HONE << shift;
            result |= one;
        }
    }

    return result;
}

HSET generateInterlaced(int divMax, int interlacedCount, int index)
{
    int count = divMax / interlacedCount;
    if(count * interlacedCount < divMax)
        count++;

    HSET result = 0;
    for(int index2 = 0; index2 < count; index2++)
    {
        int shift = index2 * interlacedCount + index;
        if(shift <= divMax)
        {
            HSET one = HONE << shift;
            result |= one;
        }
    }

    return result;
}

void generateBlockInterlacedStat(const HSERIE& serie, const HSET* filter, int filterCount, QMap<size_t, size_t>& stat)
{
    for(HSERIE::const_iterator it = serie.begin(); it != serie.end(); it++)
    {
        HSET hset = *it;
        generateBlockInterlacedStat(hset, filter, filterCount, stat);
    }
}

void generateBlockInterlacedStat(const HSET& hset, const HSET* filter, int filterCount, QMap<size_t, size_t> &stat)
{
    for(int index2 = 0; index2 < filterCount; index2++)
    {
        HSET block = filter[index2];
        HSET intersection = hset & block;
        size_t intersectionCount = hsetBitCount(intersection);
        if(stat.contains(intersectionCount))
        {
            stat[intersectionCount]++;
        }
        else
        {
            stat.insert(intersectionCount, 1);
        }
    }
}

size_t count5In5(const HSERIE& serie5, HSET hset6)
{
    HSERIE serie5From6;
    generateSubsetMinus1(hset6, serie5From6, HSET_LEN - 1);

    size_t result = 0;
    for(HSET hset5 : serie5From6) {
        HSERIE::const_iterator it2 = qBinaryFind(serie5.begin(), serie5.end(), hset5);
        if(it2 != serie5.end()) {
            ++result;
        }
    }

    return result;
}

size_t count5In6(const HSERIE& serie6, HSET hset6)
{
    size_t result = 0;
    for(auto crt : serie6) {
        HSET hsetInterset = crt & hset6;
        if(hsetBitCount(hsetInterset) == HSET_LEN - 1) {
            ++result;
        }
    }

    return result;
}

void serie6ToSerie5(const HSERIE& serie6, HSERIE& serie5)
{
    HSERIE many5;
    generateSerieMinus1(serie6, HSET_LEN, many5);
    generateSerieMinus1Unique(many5, serie5);
}

void clearSeries(HSERIES& series)
{
    for(auto crt : series) {
        delete crt;
    }
    series.clear();
}

HSERIES* cloneSeries(const HSERIES &original)
{
    HSERIES* result = new HSERIES();

    for(HSERIE* crt : original) {
        result->append(new HSERIE(*crt));
    }

    return result;
}

void series6ToSeries5(const HSERIES& series6, HSERIES& series5)
{
    for(auto crt : series6) {
        series5.append(new HSERIE());
        serie6ToSerie5(*crt, *series5.last());
    }
}

void generateSeriePair(const HSERIE& in, HSERIE_PAIR &out)
{
    HSET old = HALL;
    size_t counter = 0;
    for(HSET crt : in) {
        if(crt == old) {
            counter++;
        } else {
            if(old != HALL) {
                out.append(QPair<HSET, size_t>(old, counter));
            }
            old = crt;
            counter = 1;
        }
    }
    out.append(QPair<HSET, size_t>(old, counter));
}

void generateMapCounterSerie(const HSERIE_PAIR& pair, size_t counterMin, QMap<size_t, HSERIE*>& mapCounterSerie)
{
    for(const QPair<HSET, size_t>& crt : pair) {
        if(crt.second < counterMin) {
            continue;
        }

        if(!mapCounterSerie.contains(crt.second)) {
            mapCounterSerie.insert(crt.second, new HSERIE());
        }
        HSERIE* serie = mapCounterSerie[crt.second];
        serie->append(crt.first);
    }

    for(HSERIE* serie : mapCounterSerie.values()) {
        qSort(serie->begin(), serie->end());
    }
}

size_t countBinarySearch(const HSERIE_PAIR& sp, HSET key, int indexMin, int indexMax)
{
    if(indexMax < indexMin) {
        return 0;
    }

    int indexMiddle = (indexMin + indexMax) >> 1;
    HSET valMiddle = sp[indexMiddle].first;
    if (key < valMiddle)
        return countBinarySearch(sp, key, indexMin, indexMiddle - 1);
    else if (key > valMiddle)
        return countBinarySearch(sp, key, indexMiddle + 1, indexMax);
    else
        return sp[indexMiddle].second;
}
