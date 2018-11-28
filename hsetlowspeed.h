#ifndef HSETLOWSPEED_H
#define HSETLOWSPEED_H

#include "hsethighspeed.h"
#include <QString>
#include <QMap>
#include <QPair>

#define FILTER_INTERSECTIONSCOUNT_COUNT_MIN  2
#define STRING_NULL     "NULL"

struct IntersectionCount
{
    size_t intersection[HSET_LEN];
    size_t count;

    IntersectionCount(HSTAT hIntersection, size_t counter)
        : count(counter)
    {
        for(int index = 0; index < HSET_LEN; index++)
        {
            intersection[index] = hIntersection[index];
        }
    }
};

struct PlayingSet
{
    QString intersection;
    size_t count;
    int indexHset;
    HSET hset;

    PlayingSet(const QString& intersect, size_t counter, int indexFromHorizon, HSET hsetFromHorizon)
        : intersection(intersect), count(counter), indexHset(indexFromHorizon), hset(hsetFromHorizon)
    {
    }
};

typedef QList<IntersectionCount*> HINTERSECTIONS_COUNT;
typedef QList<PlayingSet*> PLAYING_SETS;

void hsetToString(HSET hset, bool binary, QString& result);
void hsetToString2(HSET hset, QString& result);
QString hsetToString3(HSET hset);
void hdivToString(HDIV hdiv, bool binary, QString& result);
void hsplitToString(HSPLIT list, bool binary, QString& result);
void hsplitToString(const QList<HDIV> &list, bool binary, QString& result);
void hlimitToString(HLIMIT list, bool binary, QString& result);
void hserieToString(const HSERIE& list, bool binary, QString& result);
void hserieToString(const HSERIE& list, int& index, QString& result);
QString serieToString02(const HSERIE& list);
size_t hStatHash(const HSTAT stat);
void hStatHashReverse(size_t hash, HSTAT stat);
void hStatToString(const HSTAT stat, QString& result, char separator = ',');
void hStatToString(const HSTAT stat, size_t max, QString& result, char separator = ',');
void hStatToString2(HSTAT stat, QString& result);
void hStatsToString(const QList<HSTAT>& stats, QString& result);
void hSerieStatToString(QList<HSTAT>& serieStat, QString& result);
void neighboursIntersectionToString(const QList<QPair<HSTAT, size_t>*>& list, QString& result);
HSET stringToHset(const QString &text, const char separator = ',');
HDIV intToHDIV(int value);
void hIntersectionCountToString(const IntersectionCount& intersectionCount, QString& result);
void hIntersectionsCountToString(const HINTERSECTIONS_COUNT& list, QString& result);
bool readRawData(const QString &filePath, HSERIE& result, QString& lastDate);
bool readRawDataWithDate(const QString &filePath, QList<QPair<QString, HSET>*>& result);
bool parseRawDataLine(const QString& line, HSET& result, QString& date);
void bottomUp(QList<QPair<QString, HSET>*>& list);
void subserie(const HSERIE& original, int starTop, int lastsLen, HSERIE& result);
void subserie(const QList<QPair<QString, HSET>*>& original, int starTop, int lastsLen, HSERIE& result);
void subserieWithoutLastsRemoved(const HSERIE& original, int lastsRemovedLen, HSERIE& result);
void serieLast(const HSERIE& original, HSERIE& history, HSET& last);
void filterIntersectionCount(const HINTERSECTIONS_COUNT& in, HINTERSECTIONS_COUNT& out);
void intersectionCount(const HSERIE& serie, HINTERSECTIONS_COUNT& intersectionsCount);
void addIntersection(HSTAT const intersection, HINTERSECTIONS_COUNT& intersectionsCount);
bool compareIntersectionCount(const IntersectionCount* x, const IntersectionCount* y);
QString dateCompact(const QString date);
void clear(HINTERSECTIONS_COUNT& intersectionsCount);
void clear(PLAYING_SETS& playingSets);
void initRand();
bool hserieLoad(const QString &fileName, HSERIE &serie);
void hsetCountingAdd(QMap<HSET, size_t>& counter, HSET hset, size_t count);
void hsetCountingToString(QMap<HSET, size_t>& counter, QString& result, QString separator = "");
void statAdd(QMap<size_t, size_t>& stat, size_t key);
void statAdd(QMap<size_t, size_t>& stat, size_t key, size_t value);
void statAdd(QMap<size_t, size_t>& x, const QMap<size_t, size_t>& y);
void statAdjustment(const QMap<size_t, size_t>& stat, size_t intervals, QMap<size_t, size_t> &result);
size_t statInverval(const QMap<size_t, size_t>& stat, size_t intervals, size_t value);
void statToString(const QMap<size_t, size_t>& stat, QString& result, QString separator = "");
QString statToString02(const QMap<size_t, size_t>& stat);
void statToStringSortByValue(const QMap<size_t, size_t> &stat, QString &result, QString separator);
QString statToStringSortByValueDesc(const QMap<size_t, size_t> &stat);
QString statToStringLast5(const QMap<size_t, size_t> &stat, int lastsLen = 5);
size_t statKeyMax(const QMap<size_t, size_t> &stat);
void statToStringBoth(const QMap<size_t, size_t> &stat, QString &result);
void genericStatToString(const QMap<int, int>& stat, QString& result);
bool hserieReadAll(const QString &dir, QList<QPair<QString, HSERIE*>*>& series, QString &log);
void hserieSplitByCount(const QList<QPair<QString, HSET>*>& original, int count, QList<QPair<QString, HSERIE*>*>& series);
void seriesToString(const QList<QPair<QString, HSERIE*>*>& series, QString& result);
void clear(QList<QPair<QString, HSERIE*>*>& series);
void hsetAndNeighboursStatToString(HSET hset, const HSERIE& serie, QString& result);
void readHistoryFiles22(const QString &dir, QStringList& result);
bool hStatFromString(const QString& in, HSTAT result);
void indexesToString(const QList<int>& list, QString& result, const char separator = ',');
bool indexesFromString(const QString& text, QList<int>& result);
QString seriesToString01(const HSERIES& series, HSET last);
QString seriePairToString01(const HSERIE_PAIR& serie);
void statSeriePair(const HSERIE_PAIR& serie, QMap<size_t, size_t>& stat);
QString sp5HitToString01(const HSERIE_PAIR& sp5, HSET last);
size_t serie6MaxPair5(const HSERIE& serie);
void initStat(size_t stat[HSET_LEN]);
QString blockToString(HSET blocks[BLOCK_COUNT]);
QString hSerieWithLast(const HSERIE& serie, HSET last);
QString serie6ToSerie5TraceLastToString(const HSERIE& serie, HSET last, int showLastLen = 10);

#endif // HSETLOWSPEED_H
