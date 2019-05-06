#ifndef HSPEEDHIGH_H
#define HSPEEDHIGH_H

#include <vector>
#include <deque>
#include <map>
#include <climits>
#include <utility>
#include <algorithm>

using namespace std;

#define HDIV_MAX                            49ULL
#define HSET_LEN                            6
#define HONE                                1ULL
#define HZERO                               0ULL
#define HALL                                ULONG_MAX
#define HSET_LIMIT                          (HONE << HDIV_MAX)
#define HSET_INTERSECTION_LEN               10
#define BLOCK_COUNT                         7
#define SERIES_STAT_INTERSECTION_MIN        2


typedef unsigned __int64 HSET;
typedef deque<HSET> HSERIE;
typedef unsigned __int64 HDIV;
typedef vector<HDIV> HSOLUTION;
typedef deque<pair<HSET, size_t>>  HSERIE_PAIR;


#define MIN(x, y)((x) < (y) ? (x) : (y))
#define MAX(x, y)((x) > (y) ? (x) : (y))

void serieAdd(HSERIE& base, const HSERIE& diff);
void serieLast(const HSERIE& original, HSERIE& history, HSET& last);
void subserie(const HSERIE& original, int starTop, int lastsLen, HSERIE& result);
void hsetToArray(HSET hset, unsigned int result[HSET_LEN]);
size_t hsetBitCount(HSET hset);
HSET generateHSET(const HSOLUTION& solution);
HSET generateHSET(const HDIV divs[HSET_LEN]);
void statAdd(size_t* stat, unsigned int size, size_t value);
void splitHSET(HSET hset, HDIV result[HSET_LEN]);
void splitHSET(HSET hset, HSOLUTION &result);
void hdivCopy(const HDIV source[HSET_LEN], HDIV dest[HSET_LEN]);
void generateSubsetMinus1(HSET hset, HSERIE &result, int len);
void generateSeriePair(const HSERIE& in, HSERIE_PAIR &out);
size_t countBinarySearch(const HSERIE_PAIR& sp, HSET key, int indexMin, int indexMax);
void statAdd(map<size_t, size_t>& stat, size_t key);
void statAdd(map<size_t, size_t>& stat, size_t key, size_t value);
void statAdd(map<size_t, size_t>& base, const map<size_t, size_t>& diff);
void statSeriePair(const HSERIE_PAIR& serie, map<size_t, size_t>& stat);
void intersect2(HSET hset, const HSERIE& list,  size_t stat[HSET_LEN]);
HDIV intToHDIV(int value);
size_t intersectBlockInterlacedCountMax(HSET hset, HDIV* blockInterlaced, int blockInterlacedLen);
bool intersect(HSET hset, const HSERIE& list, size_t stat[HSET_LEN]);
void statInit(size_t stat[HSET_LEN]);
void generateBlockAndInterlaced(HSET blocks[BLOCK_COUNT], HSET interlaced[BLOCK_COUNT]);
HSET generateBlock(int divMax, int blocksCount, int index);
HSET generateInterlaced(int divMax, int interlacedCount, int index);
void generateBlockInterlacedStat(const HSERIE& serie, const HSET* filter, int filterCount, map<size_t, size_t>& stat);
void generateBlockInterlacedStat(const HSET& hset, const HSET* filter, int filterCount, map<size_t, size_t> &stat);
void hserieLimits(const HSERIE& list, HDIV min[HSET_LEN], HDIV max[HSET_LEN]);
void limitInit(HDIV limit[HSET_LEN]);

#endif // HSPEEDHIGH_H
