#include "hsetlowspeed.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "HCommon.h"
#include <QTime>
#include <QDir>
#include <qmath.h>

void hsetToString(HSET hset, bool binary, QString& result)
{
    if(binary)
    {
        result.append(QString::number(hset, 2));
    }
    else
    {
        for(HSET index = 0; index < HDIV_MAX; index++)
        {
            HSET mask = HONE << index;
            HSET indexIs1 = hset & mask;
            if(indexIs1)
            {
                if(result.length() > 0)
                    result.append(",");

                QString sNumber;
                sNumber.sprintf("%02llu", index + 1);
                result.append(sNumber);
            }
        }
    }
}

void hsetToString2(HSET hset, QString& result)
{
    QString sHset;
    hsetToString(hset, false, sHset);

    result.sprintf("[%s]", qPrintable(sHset));
}

QString hsetToString3(HSET hset)
{
    QString result;
    hsetToString2(hset, result);

    return result;
}

void hdivToString(HDIV hdiv, bool binary, QString& result)
{
    hsetToString(hdiv, binary, result);
}

void hsplitToString(HSPLIT list, bool binary, QString& result)
{
    for(int index = 0; index < HSET_LEN; index++)
    {
        QString sHSet;
        hsetToString(list[index], binary, sHSet);
        result.append(sHSet);
        result.append(",");
    }
}

void hsplitToString(const QList<HDIV> &list, bool binary, QString& result)
{
    for(QList<HDIV>::const_iterator it = list.begin(); it != list.end(); it++)
    {
        const HDIV hdiv = *it;
        QString sHSet;
        hsetToString(hdiv, binary, sHSet);
        result.append(sHSet);
        result.append(",");
    }
}

void hlimitToString(HLIMIT list, bool binary, QString& result)
{
    for(int index = 0; index < HSET_LEN; index++)
    {
        QString sHSet;
        hdivToString(list[index], binary, sHSet);
        result.append(sHSet);
        result.append(",");
    }
}

void hserieToString(const HSERIE& list, bool binary, QString& result)
{
    for(int index = 0; index < list.size(); index++)
    {
        QString sHSet;
        hsetToString(list.at(index), binary, sHSet);
        result.append(sHSet);
        result.append("\n");
    }
}

void hserieToString(const HSERIE& list, int& index, QString& result)
{
    for(HSERIE::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        HSET hset = *it;

        QString sHSet1;
        hsetToString(hset, false, sHSet1);

        QString sHSet2;
        sHSet2.sprintf("%'d. %s\n", index++, qPrintable(sHSet1));

        result.append(sHSet2);
    }
}

QString serieToString02(const HSERIE& list)
{
    QString result;

    for(auto crt : list) {
        result += QString("%1, ").arg(crt);
    }

    return result;
}

size_t hStatHash(const HSTAT stat)
{
    size_t result = 0;

    for(int index = 0; index < HSET_LEN; ++index)
        result += stat[index] * qPow(10, HSET_LEN - 1 - index);

    return result;
}

void hStatHashReverse(size_t hash, HSTAT stat)
{
    for(int index = 0; index < HSET_LEN; index++)
        stat[index] = 0;

    for(int index = 0; index < HSET_LEN; index++)
    {
        size_t tenPow = qPow(10, HSET_LEN - 1 - index);
        stat[index] = hash / tenPow;
        hash = hash % tenPow;
    }
}

void hStatToString(const HSTAT statIntersection, QString& result, char separator)
{
    hStatToString(statIntersection, HSET_LEN, result, separator);
}

void hStatToString(const HSTAT statIntersection, size_t max, QString& result, char separator)
{
    if(statIntersection == NULL)
    {
        result = STRING_NULL;
        return;
    }

    for(size_t index = 0; index < max; index++)
    {
        if(index > 0)
            result.append(separator);

        QString sNumber;
        sNumber.sprintf("%'lu", statIntersection[index]);
        result.append(sNumber);
    }
}

void hStatToString2(HSTAT stat, QString& result)
{
    int lastValidIndex = 0;
    for(int index = 0; index < HSET_LEN; index++)
    {
        if(stat[index] != 0)
            lastValidIndex = index;
    }

    const int limitMin = 2;

    if(lastValidIndex < limitMin)
        lastValidIndex = limitMin;

    for(int index = 0; index <= lastValidIndex; index++)
    {
        QString sNumber;
        sNumber.sprintf("%zu", stat[index]);
        result.append(sNumber);
    }
}

void hStatsToString(const QList<HSTAT>& stats, QString& result)
{
    int index = 0;
    for(QList<HSTAT>::const_iterator it = stats.begin(); it != stats.end(); ++it)
    {
        QString sStat;
        hStatToString(*it, HSET_LEN, sStat);

        QString sStat2;
        sStat2.sprintf("%2d. [%s]\n", index++, qPrintable(sStat));

        result += sStat2;
    }
}

void hIntersectionCountToString(const IntersectionCount& intersectionCount, QString& result)
{
    result.append("[");

    for(int index = 0; index < HSET_LEN; index++)
    {
        if(index > 0)
            result.append(",");

        size_t number = intersectionCount.intersection[index];

        result.append(QString::number(number, 10));
    }

    result.append("]->");
    result.append(QString::number(intersectionCount.count, 10));
}

void hIntersectionsCountToString(const HINTERSECTIONS_COUNT& list, QString& result)
{
    for(int index = 0; index < list.size(); index++)
    {
        IntersectionCount *intersectionCount = list.at(index);
        QString sIntersectionCount;

        hIntersectionCountToString(*intersectionCount, sIntersectionCount);
        result.append(sIntersectionCount + "\n");
    }
}

void hSerieStatToString(QList<HSTAT>& serieStat, QString& result)
{
    for(int index = 0; index < serieStat.size(); index++)
    {
        HSTAT stat = serieStat.at(index);
        QString sStat;

        hStatToString(stat, sStat);
        result.append(sStat + "\n");
    }
}

void neighboursIntersectionToString(const QList<QPair<HSTAT, size_t>*>& list, QString& result)
{

    for(QList<QPair<HSTAT, size_t>*>::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        QPair<HSTAT, size_t>* pair = *it;

        QString sStat;
        hStatToString(pair->first, sStat);

        QString sLine;
        sLine.sprintf("[%s]->%lu\n", qPrintable(sStat), pair->second);

        result.append(sLine);
    }
}

HSET stringToHset(const QString &text, const char separator)
{
    const QStringList numbers = text.split(separator);
//    if(numbers.size() != HSET_LEN)
//        throw EXCEPTION_HSET_LEN_WRONG;

    HSET result = 0;
    for(QStringList::const_iterator it = numbers.begin(); it != numbers.end(); it++)
    {
        QString sNumber = *it;

        bool ok = false;
        int number = sNumber.toInt(&ok);
        if(!ok)
        {
            throw EXCEPTION_HSET_CONVERT_WRONG;
        }

        HDIV div = HONE << (number - 1);
        result |= div;
    }

    return result;
}

HDIV intToHDIV(int value)
{
    return HONE << (value - 1);
}

bool readRawData(const QString& filePath, HSERIE& result, QString& lastDate)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return false;

     QTextStream in(&file);

     int index = 0;
     while (!in.atEnd()) {
         QString line = in.readLine();

         if(index > 0)
         {
             HSET hset;
             QString sDate;

             if(line.indexOf("--") == 0)
                 continue;

             if(line.trimmed().isEmpty()) {
                 continue;
             }

             if(!parseRawDataLine(line, hset, sDate))
             {
                 file.close();
                 return false;
             }

             result.prepend(hset);

             if(lastDate.length() == 0)
                 lastDate = sDate;
         }

         index++;
     }

     file.close();

     return true;
}

bool readRawDataWithDate(const QString &filePath, QList<QPair<QString, HSET>*>& result)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return false;

     QTextStream in(&file);

     int index = 0;
     while (!in.atEnd()) {
         QString line = in.readLine();

         if(index > 0)
         {
             HSET hset;
             QString sDate;

             if(line.indexOf("--") == 0)
                 continue;

             if(!parseRawDataLine(line, hset, sDate))
             {
                 file.close();
                 return false;
             }

             QPair<QString, HSET>* pair = new QPair<QString, HSET>();
             pair->first = sDate;
             pair->second = hset;

             result.prepend(pair);
         }

         index++;
     }

     file.close();

     return true;
}

bool parseRawDataLine(const QString& line, HSET& result, QString& date)
{
    QStringList numbers = line.split(",", QString::SkipEmptyParts);

    if(numbers.size() != 7)
        return false;

    date = numbers[0];

    result = 0;
    for(int index = 1; index < 7; index++)
    {
        QString sNumber = numbers.at(index);

        bool success = false;
        HSET divShift = sNumber.toULongLong(&success, 10);
        if(!success)
        {
            return false;
        }
        HSET divHSet = HONE << (divShift - HONE);
        result |= divHSet;
    }

    return true;
}

void bottomUp(QList<QPair<QString, HSET>*>& list) {
    typedef QPair<QString, HSET>* Pair1;

    for(int index1 = 0; index1 < list.size() / 2; ++index1) {
        int index2 = list.size() - 1 - index1;

        Pair1 temp = list.at(index1);
        list[index1] = list.at(index2);
        list[index2] = temp;
    }
}

void subserie(const HSERIE& original, int starTop, int lastsLen, HSERIE& result)
{
    for(int index1 = 0; index1 < lastsLen; index1++)
    {
        int index2 = starTop - index1;
        result.prepend(original.at(index2));
    }
}

void subserie(const QList<QPair<QString, HSET>*>& original, int starTop, int lastsLen, HSERIE& result)
{
    for(int index1 = 0; index1 < lastsLen; index1++)
    {
        int index2 = starTop - index1;
        QPair<QString, HSET>* pair = original.at(index2);
        result.prepend(pair->second);
    }
}

void subserieWithoutLastsRemoved(const HSERIE& original, int lastsRemovedLen, HSERIE& result)
{
    for(int index = 0; index < original.size() - lastsRemovedLen; index++)
        result.append(original.at(index));
}

void serieLast(const HSERIE& original, HSERIE& history, HSET& last)
{
    for(int index = 0; index < original.size() -1; index++)
    {
        history.append(original.at(index));
    }

    last = original.at(original.size() - 1);
}

void filterIntersectionCount(const HINTERSECTIONS_COUNT& in, HINTERSECTIONS_COUNT& out)
{
    for(int index = 0; index < in.size(); index++)
    {
        IntersectionCount* pIcIn = in.at(index);
        if(pIcIn->count >= FILTER_INTERSECTIONSCOUNT_COUNT_MIN)
        {
            IntersectionCount *pIcOut = new IntersectionCount(pIcIn->intersection, pIcIn->count);
            out.append(pIcOut);
        }
    }

    qSort(out.begin(), out.end(), compareIntersectionCount);
}

void intersectionCount(const HSERIE& serie, HINTERSECTIONS_COUNT& intersectionsCount)
{
    for(int index1 = 0; index1 < serie.size() - (HSET_INTERSECTION_LEN + 1); index1++)
    {
        int index2 = serie.size() - 1 - index1;
        int index3 = index2 - 1;

        HSET hsetReference = serie.at(index2);
        HSERIE history;
        subserie(serie, index3, HSET_INTERSECTION_LEN, history);

        size_t stat[HSET_LEN] = {};
        intersect(hsetReference, history, HSET_LEN, stat);

        addIntersection(stat, intersectionsCount);
    }

    qSort(intersectionsCount.begin(), intersectionsCount.end(), compareIntersectionCount);
}

void addIntersection(HSTAT const intersection, HINTERSECTIONS_COUNT& intersectionsCount)
{
    for(int index = 0; index < intersectionsCount.size(); index++)
    {
        IntersectionCount *intersectionCount = intersectionsCount.at(index);
        if(equals(intersection, intersectionCount->intersection))
        {
            intersectionCount->count++;
            return;
        }
     }

    IntersectionCount *intersectionCount = new IntersectionCount(intersection, 1);
    intersectionsCount.append(intersectionCount);
}

bool compareIntersectionCount(const IntersectionCount* x, const IntersectionCount* y)
{
    for(int index = 0; index < HSET_LEN; index++)
    {
        size_t noX = x->intersection[index];
        size_t noY = y->intersection[index];

        if(noX == noY)
        {
            continue;
        }
        else if(noX < noY)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return true;
}

QString dateCompact(const QString date)
{
    QStringList parts = date.split("-", QString::SkipEmptyParts);

    if(parts.size() != 3)
        return QString();

    QString result = QString("%1%2%3").arg(parts[0], parts[1], parts[2]);

    return result;
}

void clear(HINTERSECTIONS_COUNT& intersectionsCount)
{
    for(int index = 0; index < intersectionsCount.size(); index++)
    {
        IntersectionCount *intersectionCount = intersectionsCount.at(index);
        delete intersectionCount;
        intersectionCount = 0;
        intersectionsCount[index] = 0;
    }

    intersectionsCount.clear();
}

void clear(PLAYING_SETS& playingSets)
{
    for(int index = 0; index < playingSets.size(); index++)
    {
        PlayingSet *playingSet = playingSets.at(index);
        delete playingSet;
        playingSet = 0;
        playingSets[index] = 0;
    }

    playingSets.clear();
}

void initRand()
{
    QTime time = QTime::currentTime();
    int initCounter = (time.second() + 1) * 10;

    for(int index = 0; index < initCounter; index++)
        qrand();
}

bool hserieLoad(const QString &fileName, HSERIE &serie)
{
    QFile pFile(fileName);
    if(!pFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    bool success = true;
    QTextStream textStream(&pFile);
    while(!textStream.atEnd())
    {
        QString sLine = textStream.readLine().trimmed();
        if(sLine.length() == 0)
            continue;

        int indexPoint = sLine.indexOf(".");
        if(indexPoint < 0 )
        {
            success = false;
            break;
        }

        QString sHset = sLine.mid(indexPoint + 1, sLine.length() - (indexPoint + 1));
        HSET hset = stringToHset(sHset);
        if(hset == 0)
        {
            success = false;
            break;
        }

        serie.append(hset);
    }

    pFile.close();

    return success;
}

void hsetCountingAdd(QMap<HSET, size_t>& counter, HSET hset, size_t count)
{
    if(counter.contains(hset))
        counter[hset] += count;
    else
        counter[hset] = count;
}

void hsetCountingToString(QMap<HSET, size_t>& counter, QString& result, QString separator)
{
    for(QMap<HSET, size_t>::const_iterator it = counter.begin(); it != counter.end(); it++)
    {
        HSET hset = it.key();
        size_t count = it.value();

        QString sHset;
        hsetToString(hset, false, sHset);

        QString sPair;
        sPair.sprintf("([%s], %lu)", qPrintable(sHset), count);

        result += sPair + separator;
    }
}

void statAdd(QMap<size_t, size_t>& stat, size_t key)
{
    if(stat.contains(key))
        stat[key]++;
    else
        stat[key] = 1;
}

void statAdd(QMap<size_t, size_t>& stat, size_t key, size_t value)
{
    if(stat.contains(key))
        stat[key] += value;
    else
        stat[key] = value;
}

void statAdd(QMap<size_t, size_t>& x, const QMap<size_t, size_t>& y)
{
    for(QMap<size_t, size_t>::const_iterator it = y.begin(); it != y.end(); it++)
    {
        size_t key = it.key();
        size_t value = it.value();

        if(x.contains(key))
            x[key] += value;
        else
            x[key] = value;
    }
}

void statAdjustment(const QMap<size_t, size_t>& stat, size_t intervals, QMap<size_t, size_t>& result)
{
    if(stat.size() == 0) {
        return;
    }

    if(intervals == 0) {
        return;
    }

    size_t min = stat.keys().first();
    size_t max = stat.keys().last();
    size_t step = (max - min) / intervals;

    for(size_t key1 : stat.keys()) {
        size_t val = stat[key1];
        if(key1 == min) {
            statAdd(result, key1, val);
        } else {
            size_t stepsFromMin = 0;
            if(step > 0) {
                stepsFromMin = (key1 - min) / step + 1;
            }
            size_t key2 = min + stepsFromMin * step;
            statAdd(result, key2, val);
        }
    }
}

size_t statInverval(const QMap<size_t, size_t>& stat, size_t intervals, size_t value)
{
    size_t min = stat.keys().first();
    size_t max = stat.keys().last();
    size_t step = (max - min) / intervals;

    size_t stepsFromMin = (value - min) / step + 1;
    size_t result = min + stepsFromMin * step;

    return result;
}

void statToString(const QMap<size_t, size_t> &stat, QString &result, QString separator)
{
    for(QMap<size_t, size_t>::const_iterator it = stat.begin(); it != stat.end(); it++)
    {
        size_t key = it.key();
        size_t value = it.value();
        QString sPair;
        sPair.sprintf("(%zu,%zu)", key, value);
        result += sPair + separator;
    }
}

QString statToString02(const QMap<size_t, size_t>& stat)
{
    QString result;
    statToString(stat, result);

    return result;
}

bool compareToSecond(const QPair<size_t, size_t>& x, const QPair<size_t, size_t>& y)
{
    return x.second < y.second;
}

void statToStringSortByValue(const QMap<size_t, size_t> &stat, QString &result, QString separator)
{
    QList<QPair<size_t, size_t>> list;
    for(QMap<size_t, size_t>::const_iterator it = stat.begin(); it != stat.end(); it++) {
        list.append(QPair<size_t, size_t>(it.key(), it.value()));
    }

    qSort(list.begin(), list.end(), compareToSecond);

    for(QList<QPair<size_t, size_t>>::const_iterator it = list.begin(); it != list.end(); ++it) {
        const QPair<size_t, size_t>& pair = *it;

        QString sPair;
        sPair.sprintf("(%zu,%zu)", pair.first, pair.second);
        result += sPair + separator;
    }
}

QString statToStringSortByValueDesc(const QMap<size_t, size_t> &stat)
{
    QList<QPair<size_t, size_t>> list;
    for(QMap<size_t, size_t>::const_iterator it = stat.begin(); it != stat.end(); it++) {
        list.append(QPair<size_t, size_t>(it.key(), it.value()));
    }

    qSort(list.begin(), list.end(), [](const QPair<size_t, size_t>& x, const QPair<size_t, size_t>& y)
    {
        return x.second > y.second;
    });

    QString result;
    for(QList<QPair<size_t, size_t>>::const_iterator it = list.begin(); it != list.end(); ++it) {
        const QPair<size_t, size_t>& pair = *it;

        QString sPair;
        sPair.sprintf("(%zu,%zu)", pair.first, pair.second);
        result += sPair;
    }

    return result;
}

QString statToStringLast5(const QMap<size_t, size_t> &stat, int lastsLen)
{
    QString result;

    QList<size_t> keysDesc = stat.keys();
    qSort(keysDesc.begin(), keysDesc.end(), qGreater<int>());

    int indexMax = MIN(lastsLen, keysDesc.size());
    int indexNewLine = -1;
    for(int index = 0; index < indexMax; ++index) {
        if(++indexNewLine % 8 == 0 && index > 0) {
            result += "_\n\t";
        }

        size_t key = keysDesc[index];
        result += QString("(%1,%2)").arg(key).arg(stat[key]);
    }

    return result;
}

size_t statKeyMax(const QMap<size_t, size_t> &stat)
{
    if(stat.isEmpty())
        return 0;

    return stat.keys().last();
}

void statToStringBoth(const QMap<size_t, size_t> &stat, QString &result)
{
    QList<QPair<size_t, size_t>> list1;
    for(QMap<size_t, size_t>::const_iterator it = stat.begin(); it != stat.end(); it++) {
        list1.append(QPair<size_t, size_t>(it.key(), it.value()));
    }

    QList<QPair<size_t, size_t>> list2(list1);

    qSort(list1.begin(), list1.end(), compareToSecond);

    for(int index = 0; index < list1.size(); ++index) {
        const QPair<size_t, size_t>& pair1 = list1[index];
        const QPair<size_t, size_t>& pair2 = list2[index];

        QString sPair;
        sPair.sprintf("(%4zu,%3zu)\t(%4zu,%3zu)\n", pair1.first, pair1.second, pair2.first, pair2.second);
        result += sPair;
    }
}

void genericStatToString(const QMap<int, int> &stat, QString& result)
{
    const QList<int>& keys = stat.keys();
    for(QList<int>::const_iterator it = keys.begin(); it != keys.end(); ++it)
    {
        int key = *it;
        int value = stat[key];

        QString sPair;
        sPair.sprintf("(%d, %d)", key, value);

        result += sPair + "\n";
    }
}

bool hserieReadAll(const QString& dirPath, QList<QPair<QString, HSERIE*>*>& series, QString& log)
{
    bool ok = true;

    QDir dirParent(dirPath);
    QStringList dirChildren = dirParent.entryList(QDir::Dirs, QDir::Name);

    for(QStringList::const_iterator it1 = dirChildren.begin(); it1 != dirChildren.end(); ++it1)
    {
        QString sDirCrtName = *it1;

        bool dirIsNo = false;
        sDirCrtName.toUInt(&dirIsNo);
        if(!dirIsNo)
            continue;

        QString sDirCrtPath = QDir::cleanPath(dirParent.absolutePath() + QDir::separator() + sDirCrtName);
        QDir dirCrt(sDirCrtPath);

        QStringList files = dirCrt.entryList(QDir::Files, QDir::Name);
        for(QStringList::const_iterator it2 = files.begin(); it2 != files.end(); ++it2)
        {
            QString sFileName = *it2;
            if(sFileName.compare(FILE_RAWDATA) == 0)
            {
                QString sFilePath = QDir::cleanPath(dirCrt.absolutePath() + QDir::separator() + sFileName);

                HSERIE serieFromFile;
                QString sLastDate;
                if(readRawData(sFilePath, serieFromFile, sLastDate))
                {
                    HSERIE* serieToAdd = new HSERIE(serieFromFile);
                    QPair<QString, HSERIE*>* pair = new QPair<QString, HSERIE*>(sLastDate, serieToAdd);
                    series.append(pair);
                }
                else
                {
                    ok = false;

                    QString sErr;
                    sErr.sprintf("Cannot open file: %s\n", qPrintable(sFilePath));
                    log += sErr;
                }
            }
        }
    }

    return ok;
}

void hserieSplitByCount(const QList<QPair<QString, HSET>*>& original, int count, QList<QPair<QString, HSERIE*>*>& series)
{
    typedef QPair<QString, HSET>* Pair1;
    typedef QPair<QString, HSERIE*>* Pair2;
    for(int index = count - 1; index < original.size(); ++index) {
        Pair1 pair1 = original.at(index);
        Pair2 pair2 = new QPair<QString, HSERIE*>();

        pair2->first = pair1->first;

        HSERIE* serie2 = new HSERIE();
        pair2->second = serie2;
        subserie(original, index, count, *serie2);

        series.append(pair2);
    }
}

void seriesToString(const QList<QPair<QString, HSERIE*>*>& series, QString& result)
{
    for(QList<QPair<QString, HSERIE*>*>::const_iterator it = series.begin(); it != series.end(); ++it)
    {
        QPair<QString, HSERIE*>* pair = *it;

        result += pair->first + "\n";
        result += QString(20, '=') + "\n";

        QString sSerie;
        hserieToString(*pair->second, false, sSerie);

        result += sSerie;
        result += QString(20, '=') + "\n";
    }
}

void clear(QList<QPair<QString, HSERIE*>*>& series)
{
    for(QList<QPair<QString, HSERIE*>*>::const_iterator it = series.begin(); it != series.end(); ++it)
    {
        delete *it;
    }
    series.clear();
}

void hsetAndNeighboursStatToString(HSET hset, const HSERIE& serie, QString& result)
{
    QString sHset;
    hsetToString(hset, false, sHset);
    result += sHset + "\n";

    size_t stat[HSET_LEN] = {};
    intersect(hset, serie, HSET_LEN, stat);

    QString sStat;
    hStatToString(stat, sStat);
    QString sStat2;
    sStat2.sprintf("+[%s]\n", qPrintable(sStat));
    result += sStat2;
    result += QString(20, '-') + "\n";

    HSERIE neighbours;
    generateNeighbours(hset, neighbours);
    qSort(neighbours);

    QList<QPair<HSTAT, size_t>*> neighboursIntersection;
    intersectNeighbours(neighbours, serie, neighboursIntersection);

    QString sNI;
    neighboursIntersectionToString(neighboursIntersection, sNI);
    result += sNI;
    result += QString(20, '=') + "\n";

    for(QList<QPair<HSTAT, size_t>*>::iterator it = neighboursIntersection.begin(); it != neighboursIntersection.end(); ++it)
    {
        delete *it;
    }
    neighboursIntersection.clear();
}

void readHistoryFiles22(const QString& dir, QStringList& result)
{
    QDir dirParent(dir);
    QStringList dirChildren = dirParent.entryList(QDir::Dirs, QDir::Name);

    for(QStringList::const_iterator it1 = dirChildren.begin(); it1 != dirChildren.end(); ++it1)
    {
        QString sDirCrtName = *it1;

        QRegExp rxDir("[0-9]*");
        if(!rxDir.exactMatch(sDirCrtName)) {
            continue;
        }

        QString sDirCrtPath = QDir::cleanPath(dirParent.absolutePath() + QDir::separator() + sDirCrtName);
        QDir dirCrt(sDirCrtPath);

        QStringList files = dirCrt.entryList(QDir::Files, QDir::Name);
        for(QStringList::const_iterator it2 = files.begin(); it2 != files.end(); ++it2)
        {
            QString sFileName = *it2;

            QRegExp rxFile("H*.22.txt");
            rxFile.setPatternSyntax(QRegExp::Wildcard);
            if(rxFile.exactMatch(sFileName)) {
                QString sFilePath = QDir::cleanPath(dirCrt.absolutePath() + QDir::separator() + sFileName);
                result.append(sFilePath);
            }
        }
    }
}

bool hStatFromString(const QString& in, HSTAT result)
{
    QString sStat = in;

    if(in.indexOf("[") == 0) {
        sStat = in.mid(1, in.length() - 2);
    }

    QStringList numbers = sStat.split(",");
    if(numbers.length() != HSET_LEN) {
        return false;
    }

    for(int index = 0; index < numbers.length(); ++index) {
        bool ok = false;
        result[index] = numbers[index].toUInt(&ok);

        if(!ok) {
            return false;
        }
    }

    return true;
}

void indexesToString(const QList<int>& list, QString& result, const char separator)
{
    for(QList<int>::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        result += QString("%1%2").arg(*it).arg(separator);
    }
}

bool indexesFromString(const QString& text, QList<int>& result)
{
    QStringList sIndexes = text.split(",");

    for(QStringList::const_iterator it = sIndexes.begin(); it != sIndexes.end(); ++it)
    {
        QString sIndex = *it;

        bool ok = false;
        int index = sIndex.toInt(&ok);
        if(!ok)
            return false;

        result.append(index);
    }

    return true;
}

QString seriesToString01(const HSERIES& series, HSET last)
{
    QString result;

    QMap<size_t, size_t> statSize;
    size_t statLastSeries[HSET_LEN] = {};

    size_t sizeTotal = 0;
    QString sSseriesWithLastLen;

    for(const HSERIE* crt : series) {
        sizeTotal += crt->size();
        statAdd(statSize, crt->size());

        size_t statLastSerie[HSET_LEN] = {};
        intersect2(last, *crt, statLastSerie);
        hstatAdd(statLastSeries, statLastSerie);

        if(statLastSerie[HSET_LEN - 2] > 0) {
            sSseriesWithLastLen += QString("%1,").arg(crt->size());
        }
    }

    QMap<size_t, size_t> statSizeAdjust;
    statAdjustment(statSize, 20, statSizeAdjust);

    QString sStatSize;
    statToString(statSizeAdjust, sStatSize);
    result += QString("statSize=%1/%2. %3\n").arg(sizeTotal).arg(series.size()).arg(sStatSize);

    QString sStatLast;
    hStatToString(statLastSeries, sStatLast, '|');
    result += QString("statLast=[%1]\n").arg(sStatLast);

    if(!sSseriesWithLastLen.isEmpty()) {
        result += QString("serieSizeWithLast=%1\n").arg(sSseriesWithLastLen);
    }

    return result;
}

QString seriePairToString01(const HSERIE_PAIR& serie)
{
    QString result;

    for(auto crt : serie) {
        result += QString("(%1,%2)").arg(crt.first).arg(crt.second);
    }

    return result;
}

void statSeriePair(const HSERIE_PAIR& serie, QMap<size_t, size_t>& stat)
{
    for(auto crt : serie) {
        statAdd(stat, crt.second);
    }
}

QString sp5HitToString01(const HSERIE_PAIR& sp5, HSET last)
{
    QString result;

    for(auto crt : sp5) {
        if(hsetContains(last, crt.first)) {
            result += QString("%1,").arg(crt.second);
        }
    }

    return result;
}

size_t serie6MaxPair5(const HSERIE& serie)
{
    HSERIE many5;
    generateSerieMinus1(serie, HSET_LEN, many5);

    HSERIE_PAIR sp;
    generateSeriePair(many5, sp);

    QMap<size_t, size_t> spStat;
    statSeriePair(sp, spStat);

    size_t result = spStat.keys().last();

    return result;
}

void initStat(size_t stat[HSET_LEN])
{
    for(int index = 0; index < HSET_LEN; ++index) {
        stat[index] = 0;
    }
}

QString blockToString(HSET blocks[BLOCK_COUNT])
{
    QString result;

    for(int index = 0; index < BLOCK_COUNT; index++)
    {
        QString sLine;
        hsetToString(blocks[index], false, sLine);
       result += (sLine + "\n");
    }

    return result;
}

QString hSerieWithLast(const HSERIE& serie, HSET last)
{
    size_t stat[HSET_LEN] = {};
    intersect2(last, serie, stat);

    QString sStat;
    hStatToString(stat, sStat, '|');

    QString result;
    result.sprintf("%'d. [%s]\n", serie.size(), qPrintable(sStat));

    return result;
}

QString serie6ToSerie5TraceLastToString(const HSERIE& serie, HSET last, int showLastLen)
{
    size_t stat[HSET_LEN] = {};
    intersect2(last, serie, stat);

    QString sStat;
    hStatToString(stat, sStat, '|');

    HSERIE many5;
    generateSerieMinus1(serie, HSET_LEN, many5);

    HSERIE_PAIR sp;
    generateSeriePair(many5, sp);

    QMap<size_t, size_t> spStat;
    statSeriePair(sp, spStat);

    QString result = QString("serie5\n%1\n[%2]\n[%3]\n").arg(statToStringLast5(spStat, showLastLen)).arg(sStat).arg(sp5HitToString01(sp, last));

    return result;
}
