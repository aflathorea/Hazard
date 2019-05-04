#include "hspeedhigh.h"

void serieAdd(HSERIE& base, const HSERIE& diff)
{
    for(auto crt : diff)
        base.push_back(crt);
}

void serieLast(const HSERIE& original, HSERIE& history, HSET& last)
{
    last = original.back();

    history = original;
    history.pop_back();
}

void subserie(const HSERIE& original, int starTop, int lastsLen, HSERIE& result)
{
    for(int index1 = 0; index1 < lastsLen; index1++)
    {
        int index2 = starTop - index1;
        result.push_front(original.at(index2));
    }
}

void hsetToArray(HSET hset, unsigned int result[HSET_LEN])
{
    unsigned int index2 = 0;
    for(unsigned int index1 = 0; index1 < HDIV_MAX; ++index1)
    {
        HSET mask = HONE << index1;
        if(hset & mask) {
            result[index2] = index1 + 1;
            index2++;
            if(index2 == HSET_LEN) {
                break;
            }
        }
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

HSET generateHSET(const HSOLUTION& solution)
{
    HSET result = 0;

    for(HDIV hdiv : solution)
        result |= hdiv;

    return result;
}

HSET generateHSET(const HDIV divs[HSET_LEN])
{
    HSET result = 0;

    for(int index = 0; index < HSET_LEN; ++index) {
        result |= divs[index];
    }

    return result;
}

void statAdd(size_t* stat, unsigned int size, size_t value)
{
    if(value < size)
    {
        stat[value]++;
    }
    else
    {
        stat[size-1]++;
    }
}

void splitHSET(HSET hset, HDIV result[HSET_LEN])
{
    int index = 0;
    for(HDIV div = HONE; div <= hset, index < HSET_LEN; div = div << HONE)
    {
        if(div & hset)
        {
            result[index] = div;
            index++;
        }
    }
}

void splitHSET(HSET hset, HSOLUTION &result)
{
    for(HDIV div = HONE; div < (HONE << HDIV_MAX); div = div << HONE)
    {
        if(div & hset)
        {
            result.push_back(div);
        }
    }
}

void hdivCopy(const HDIV source[HSET_LEN], HDIV dest[HSET_LEN])
{
    for(int index = 0; index < HSET_LEN; ++index) {
        dest[index] = source[index];
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
        hdivCopy(divs, divs2);
        divs2[index] = 0;

        HSET hset2 = generateHSET(divs2);
        result.push_back(hset2);
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
                out.push_back(pair<HSET, size_t>(old, counter));
            }
            old = crt;
            counter = 1;
        }
    }
    out.push_back(pair<HSET, size_t>(old, counter));
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

void statAdd(map<size_t, size_t>& stat, size_t key)
{
    if(stat.find(key) == stat.end()) {
        stat[key] = 1;
    } else {
        stat[key]++;
    }
}

void statAdd(map<size_t, size_t>& stat, size_t key, size_t value)
{
    if(stat.find(key) == stat.end()) {
        stat[key] = value;
    } else {
        stat[key] += value;
    }
}

void statAdd(map<size_t, size_t>& base, const map<size_t, size_t>& diff)
{
    for(map<size_t, size_t>::const_iterator it = diff.begin(); it != diff.end(); it++) {
        statAdd(base, it->first, it->second);
    }
}

void statSeriePair(const HSERIE_PAIR& serie, map<size_t, size_t>& stat)
{
    for(auto crt : serie) {
        statAdd(stat, crt.second);
    }
}

void intersect2(HSET hset, const HSERIE& list,  size_t stat[HSET_LEN])
{
    for(int index = 0; index < list.size(); index++)
    {
        HSET hsetIntersection = hset & list.at(index);
        size_t bitCount = hsetBitCount(hsetIntersection);

        if(bitCount < SERIES_STAT_INTERSECTION_MIN)
            continue;

        stat[bitCount - SERIES_STAT_INTERSECTION_MIN]++;
    }
}

HDIV intToHDIV(int value)
{
    return HONE << (value - 1);
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

bool intersect(HSET hset, const HSERIE& list, size_t stat[HSET_LEN])
{
    bool result = true;

    for(int index = 0; index < list.size(); index++)
    {
        HSET hsetIntersection = hset & list.at(index);
        size_t bitCount = hsetBitCount(hsetIntersection);

        if(bitCount < HSET_LEN)
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

void statInit(size_t stat[HSET_LEN])
{
    for(int index = 0; index < HSET_LEN; ++index) {
        stat[index] = 0;
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

void generateBlockInterlacedStat(const HSERIE& serie, const HSET* filter, int filterCount, map<size_t, size_t>& stat)
{
    for(HSERIE::const_iterator it = serie.begin(); it != serie.end(); it++)
    {
        HSET hset = *it;
        generateBlockInterlacedStat(hset, filter, filterCount, stat);
    }
}

void generateBlockInterlacedStat(const HSET& hset, const HSET* filter, int filterCount, map<size_t, size_t> &stat)
{
    for(int index2 = 0; index2 < filterCount; index2++)
    {
        HSET block = filter[index2];
        HSET intersection = hset & block;
        size_t intersectionCount = hsetBitCount(intersection);
        if(stat.find(intersectionCount) == stat.end())
        {
            stat[intersectionCount] = 1;
        }
        else
        {
            stat[intersectionCount]++;
        }
    }
}

void hserieLimits(const HSERIE& list, HDIV min[HSET_LEN], HDIV max[HSET_LEN])
{
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

void limitInit(HDIV limit[HSET_LEN])
{
    for(int index = 0; index < HSET_LEN; ++index) {
        limit[index] = 0;
    }
}

