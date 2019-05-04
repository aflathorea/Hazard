#ifndef HSPEEDLOW_H
#define HSPEEDLOW_H

#include <cstring>
#include <string>
#include <cstdio>
#include "hspeedhigh.h"


#define SPEEDLOW_SPRINTF_LEN 1024


bool parseLine(const char* line, HSET& hset, string& date);
bool readRawData(const string& filePath, HSERIE& serie, string& lastDate);
bool readSerieFull(HSERIE& serie, string& lastDate);
bool readSerieWithoutLast(HSERIE& history, HSET&last, string& lastDate);
string hsetToString(const HSET hset);
string hserieToString(const HSERIE& serie);
string statToStringKeyVal(const size_t* stat, unsigned int size);
string statToStringLine(const size_t* stat, unsigned int size);
string statToStringLine(const size_t stat[HSET_LEN]);
string toStringFormatS2(const char* format, const string& s1, const string& s2);
string toStringFormatUL2(const char* format, const size_t n1, const size_t n2);
string statToStringSortByValueDesc(const map<size_t, size_t> &stat);
string statToStringLast5(const map<size_t, size_t> &stat, int lastsLen);
vector<size_t> keysFromMapDesc(const map<size_t, size_t> &stat);
string hdivToString(HDIV hdiv);
string hlimitToString(HDIV limit[HSET_LEN]);

#endif // HSPEEDLOW_H
