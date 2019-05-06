#include "hspeedlow.h"


bool parseLine(const char* line, HSET& hset, string& date)
{
    hset = 0;
    unsigned int no[HSET_LEN] = {};
    char sDate[32] = {};
    bool ok = (sscanf(line, "%10s,%u,%u,%u,%u,%u,%u", sDate, &no[0], &no[1], &no[2], &no[3], &no[4], &no[5]) == (HSET_LEN + 1));

    if(!ok) {
        printf("!parseLine(%s)\n", line);
        return false;
    }

    date = sDate;

    for(int index = 0; index < HSET_LEN; index++)
    {
        if(no[index] > 0)
        {
			unsigned __int64 diff = no[index] - HONE;
            HSET div = (HONE << diff);
            hset |= div;
        }
    }

    return true;
}

bool readRawData(const string& filePath, HSERIE& serie, string& lastDate)
{
    FILE* pFile = fopen(filePath.data(), "r");
    if (pFile == NULL)
    {
        printf("Cannot open file: %s\n", filePath.data());
        return false;
    }

     int index = 0;
     bool ok = true;
     while (!feof(pFile)) {
         char sLine[64] = {};
         fscanf(pFile, "%s\n", sLine);

         if(index > 0)
         {
             if(strlen(sLine) == 0) {
                 continue;
             }

             if(strncmp(sLine, "--", 2) == 0){
                 continue;
             }


             HSET hset = 0;
             string sDate;

             if(!parseLine(sLine, hset, sDate))
             {
                 ok = false;
             }

             serie.push_front(hset);

             if(lastDate.empty())
                 lastDate = sDate;
         }

         index++;
     }

     fclose(pFile);

     return ok;
}

bool readSerieFull(HSERIE& serie, string& lastDate)
{
    return readRawData("rawdata.txt", serie, lastDate);
}

bool readSerieWithoutLast(HSERIE& history, HSET&last, string& lastDate)
{
    HSERIE original;
    bool ok = readSerieFull(original, lastDate);

    if(ok)
    {
        serieLast(original, history, last);
    }

    return ok;
}

string hsetToString(const HSET hset)
{
    string result;
    for(HSET index = 0; index < HDIV_MAX; index++)
    {
        HSET mask = HONE << index;
        HSET indexIs1 = hset & mask;
        if(indexIs1)
        {
            if(result.length() > 0)
                result += ",";

            char sNo[8] = {};
            sprintf(sNo, "%02lu", index + 1);
            result += sNo;
        }
    }

    return result;
}

string hserieToString(const HSERIE& serie)
{
    string result;
    unsigned long index = 0;
    for(auto hset : serie)
    {
        char sIndex[16] = {};
        sprintf(sIndex, "%02lu. ", index++);

        result += sIndex + hsetToString(hset) + "\n";
    }

    return result;
}

string statToStringKeyVal(const size_t* stat, unsigned int size)
{
    string result;

    for(unsigned int index = 0; index < size; ++index) {
        if(index > 0 && index % 5 == 0) {
            result += "\n";
        }
        char sPair[32] = {};
        sprintf(sPair, "(%u,%lu)", index, stat[index]);
        result += sPair;
    }
    result += "\n";

    return result;
}

string statToStringLine(const size_t* stat, unsigned int size)
{
    string result("[");

    for(unsigned int index = 0; index < size; ++index) {
        if(index > 0 ) {
            result += "|";
        }

        char sNo[16] = {};
        sprintf(sNo, "%lu", stat[index]);
        result += sNo;
    }
    result += "]";

    return result;
}

string statToStringLine(const size_t stat[HSET_LEN])
{
    return statToStringLine(stat, HSET_LEN);
}

string toStringFormatS2(const char* format, const string& s1, const string& s2)
{
    char sText[SPEEDLOW_SPRINTF_LEN] = {};

    sprintf(sText, format, s1.data(), s2.data());
    string result(sText);

    return result;
}

string toStringFormatUL2(const char* format, const size_t n1, const size_t n2)
{
    char sText[SPEEDLOW_SPRINTF_LEN] = {};

    sprintf(sText, format, n1, n2);
    string result(sText);

    return result;
}

string statToStringSortByValueDesc(const map<size_t, size_t> &stat)
{
    deque<pair<size_t, size_t>> list;
    for(map<size_t, size_t>::const_iterator it = stat.begin(); it != stat.end(); it++) {
        list.push_back(pair<size_t, size_t>(it->first, it->second));
    }

    sort(list.begin(), list.end(), [](const pair<size_t, size_t>& x, const pair<size_t, size_t>& y)
    {
        return x.second > y.second;
    });

    string result;
    for(deque<pair<size_t, size_t>>::const_iterator it = list.begin(); it != list.end(); ++it) {
        const pair<size_t, size_t>& pair1 = *it;

        result += toStringFormatUL2("(%lu,%lu)", pair1.first, pair1.second);
    }

    return result;
}

vector<size_t> keysFromMapDesc(const map<size_t, size_t> &stat)
{
    vector<size_t> result;

    for(auto it : stat) {
        result.push_back(it.first);
    }

    sort(result.begin(), result.end(), [](size_t x, size_t y){
        return x > y;
    });

    return result;
}

string statToStringLast5(const map<size_t, size_t> &stat, int lastsLen)
{
   vector<size_t> keysDesc = keysFromMapDesc(stat);

   string result;
    int indexMax = MIN(lastsLen, (int)keysDesc.size());
    int indexNewLine = -1;
    for(int index = 0; index < indexMax; ++index) {
        if(++indexNewLine % 8 == 0 && index > 0) {
            result += "_\n\t";
        }

        size_t key = keysDesc[index];
        size_t value = stat.find(key)->second;
        result += toStringFormatUL2("(%lu,%lu)", key, value);
    }

    return result;
}

string hdivToString(HDIV hdiv)
{
    return hsetToString(hdiv);
}

string hlimitToString(HDIV limit[HSET_LEN])
{
    string result = "[";
    for(int index = 0; index < HSET_LEN; index++)
    {
        if(index > 0) {
            result += ",";
        }
        result += hdivToString(limit[index]);
    }
    result += "]";

    return result;
}
