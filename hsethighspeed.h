#ifndef HSETHIGHSPEED_H
#define HSETHIGHSPEED_H

#include <QStack>
#include <QList>
#include <QPair>
#include <QMap>

#define HDIV_MAX                            49
#define HSET_LEN                            6
#define HONE                                1ULL
#define HZERO                               0ULL
#define HALL                                ULLONG_MAX
#define HSET_LIMIT                          (HONE << HDIV_MAX)
#define HSET_INTERSECTION_LEN               10
#define SERIES_STAT_INTERSECTION_MIN        2
#define SERIES_STAT_INTERSECTION_RND        350
#define BLOCK_COUNT                         7
#define BLOCK_INTERSECTION_COUNT_MAX        2
#define INTERLACED_INTERSECTION_COUNT_MAX   2
#define PLAY_HORIZONS_FIRSTS_COUNT          6
#define PLAY_HORIZON_SETS_COUNT             1
#define SERIE_SIZE_STANDARD                 110


#define MIN(x, y)((x) < (y) ? (x) : (y))
#define MAX(x, y)((x) > (y) ? (x) : (y))

/*
100->5min
250->10min
300->12min
350->15min
500->20min
*/

typedef quint64         HDIV;
typedef quint64         HSET;
typedef QList<HSET>     HSERIE;
typedef HDIV*           HSPLIT;         //HSET_LEN
typedef size_t*         HSTAT;          //HSET_LEN
typedef HDIV*           HLIMIT;         //HSET_LEN
typedef QStack<HDIV>    HSOLUTION;
typedef QList<HSERIE*>  HSERIES;
typedef QList<QPair<HSET, size_t>>  HSERIE_PAIR;

HSET generateHSET(const HSOLUTION& solution);
void generateHSerie5(const HSOLUTION& solution, HSERIE& serie5);
HSET generateHSET(HSPLIT list, HDIV div = 0);
HSET generateHSETOneFull();
HSET hsetReverse(HSET hset);
bool hsetContains(HSET big, HSET small);
void bckt1(HSOLUTION& solution);
void splitHSET(HSET hset, HSPLIT result);
void splitHSET(HSET hset, QList<HDIV> &result);
void splitHSET(HSET hset, HSOLUTION &result);
void generateNeighbours(HSET hset, HSERIE& result);
void intersectNeighbours(const HSERIE& neighbours, const HSERIE& serie, QList<QPair<HSTAT, size_t>*>& result);
bool equals(HDIV div, HSPLIT list);
bool equals(HSTAT const x, HSTAT const y);
bool equals(HSTAT const x, HSTAT const y, size_t max);
bool equals(const HSERIE& x, const HSERIE& y);
void generateSubset(HSPLIT const original, int nonIndex, HSPLIT result);
void generateSubsetMinus1(HSET hset, HSERIE &result, int len = HSET_LEN);
void generateSerieMinus1(const HSERIE& serieIn, int hsetLen, HSERIE& serieOut);
void generateSerieMinus1Unique(const HSERIE& serieIn, HSERIE& serieOut);
size_t hsetBitCount(HSET hset);
size_t hdivToNumber(HDIV hdiv);
HDIV hdivFromNumber(int number);
void hsplitToIndexes(HSPLIT split, int* result);
void hsplitFromIndexes(int* indexes, HSPLIT hsplit);
HSET hserieUnion(const HSERIE& serie);
size_t hserieUnionCount(const HSERIE& serie);
bool intersect(HSET hset, const HSERIE& list, size_t max, HSTAT stat);
void intersect2(HSET hset, const HSERIE& list, HSTAT stat);
void intersect2(HSET hset, const HSET* serie, int serieSize, HSTAT stat);
void intersect(const HSERIE& x, const HSERIE& y, HSTAT stat);
void intersect6(const HSERIE& x, const HSERIE& y, HSERIE& result);
void intersect7(HSET hset, const HSERIE& list, HSTAT stat); //stat[HSET_LEN + 1]
void diff6(const HSERIE& x, const HSERIE& y, HSERIE& result);
void hserieLimits(const HSERIE& list, HLIMIT min, HLIMIT max);
int intersectionMax(const HSTAT stat);
void copySplit(HSPLIT source, HSPLIT dest);
size_t intersectBlockInterlacedCountMax(HSET hset, HDIV* blockInterlaced, int blockInterlacedLen);
bool hsetSortByFirstAndLast(const HSET& x, const HSET& y);
void hserieSortByFirstAndLast(HSERIE& serie);
void shuffle(HSERIE& serie, int repeatCount);
void hstatAdd(HSTAT first, HSTAT second);
bool hEqualFuzzy(size_t x, size_t y, float aproximatePercent);
bool hEqualFuzzy2(size_t x, size_t y, size_t aproximate);
size_t doPercent(size_t x, size_t y);
void copy(const HSTAT source, HSTAT dest);
bool contains(const QList<HSTAT>& stats, HSTAT stat);
bool statCompare(HSTAT const x, HSTAT const y);
void generateBlockAndInterlaced(int divMax, const int& blocksCount, int& interlacedCount, HSET*& blocks, HSET*& interlaced);
void generateBlockAndInterlaced(HSET blocks[BLOCK_COUNT], HSET interlaced[BLOCK_COUNT]);
HSET generateBlock(int divMax, int blocksCount, int index);
HSET generateInterlaced(int divMax, int interlacedCount, int index);
void generateBlockInterlacedStat(const HSERIE& serie, const HSET* filter, int filterCount, QMap<size_t, size_t>& stat);
void generateBlockInterlacedStat(const HSET& hset, const HSET* filter, int filterCount, QMap<size_t, size_t> &stat);
size_t count5In5(const HSERIE& serie5, HSET hset6);
size_t count5In6(const HSERIE& serie6, HSET hset6);
void serie6ToSerie5(const HSERIE& serie6, HSERIE& serie5);
void clearSeries(HSERIES &series);
HSERIES* cloneSeries(const HSERIES &original);
void series6ToSeries5(const HSERIES& series6, HSERIES& series5);
void generateSeriePair(const HSERIE& in, HSERIE_PAIR& out);
void generateMapCounterSerie(const HSERIE_PAIR& pair, size_t counterMin, QMap<size_t, HSERIE*>& mapCounterSerie);
size_t countBinarySearch(const HSERIE_PAIR& sp, HSET key, int indexMin, int indexMax);

#endif // HSETHIGHSPEED_H
