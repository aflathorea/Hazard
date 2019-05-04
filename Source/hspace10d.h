#ifndef HSPACE10D_H
#define HSPACE10D_H

#include "hspeedhigh.h"

class HSpace10D
{
private:
    unsigned int mSize;
    HSET* mSpace;

public:
    HSpace10D();
    HSpace10D(unsigned int size);
    HSpace10D(const HSpace10D& original);
    HSpace10D& operator= (const HSpace10D& original);
    HSpace10D (HSpace10D&& original);
    HSpace10D& operator= (HSpace10D&& original);
    ~HSpace10D();

private:
    void fillAnd(HSET* space, unsigned int size, unsigned int start, unsigned int numbers[HSET_LEN]);
    void fillOr(HSET* space, unsigned int size, unsigned int start, unsigned int numbers[HSET_LEN]);

public:
    HSpace10D operator& (const HSpace10D& visitor) const;
    HSpace10D& operator&= (const HSpace10D& visitor);
    HSpace10D operator| (const HSpace10D& visitor) const;
    HSpace10D& operator|= (const HSpace10D& visitor);

public:
    size_t count1() const;

public:
    void fillAndOr(HSET hset);
    void fillOrAnd(HSET hset);
    void fillAnd(HSET hset);
    void fillOr(HSET hset);
};

#endif // HSPACE10D_H
