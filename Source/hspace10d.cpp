#include "hspace10d.h"

HSpace10D::HSpace10D()
    : mSize(0)
{
    mSpace = new HSET[0];
}

HSpace10D::HSpace10D(unsigned int size)
    : mSize(size)
{
    mSpace = new HSET[size];
    for(unsigned int index = 0; index < size; ++index) {
        mSpace[index] = 0;
    }
}

HSpace10D::HSpace10D(const HSpace10D& original)
    : mSize(original.mSize)
{
    mSpace = new HSET[mSize];
    for(int index = 0; index < mSize; ++index) {
        mSpace[index] = original.mSpace[index];
    }
}

HSpace10D& HSpace10D::operator= (const HSpace10D& original)
{
    delete[] mSpace;

    mSize = original.mSize;
    mSpace = new HSET[mSize];
    for(int index = 0; index < mSize; ++index) {
        mSpace[index] = original.mSpace[index];
    }

    return *this;
}

HSpace10D::HSpace10D (HSpace10D&& original)
    : mSize(original.mSize), mSpace(original.mSpace)
{
    original.mSpace = nullptr;
    original.mSize = 0;
}

HSpace10D& HSpace10D::operator= (HSpace10D&& original)
{
    delete[] mSpace;

    mSize = original.mSize;
    mSpace = original.mSpace;

    original.mSize = 0;
    original.mSpace = nullptr;

    return *this;
}

HSpace10D::~HSpace10D()
{
    delete[] mSpace;
}

HSpace10D HSpace10D::operator& (const HSpace10D& visitor) const
{
    unsigned int size = MIN(this->mSize, visitor.mSize);

    HSpace10D result(size);
    for(unsigned int index = 0; index < size; ++index)
    {
        result.mSpace[index] = this->mSpace[index] & visitor.mSpace[index];
    }

    return result;
}

HSpace10D& HSpace10D::operator&= (const HSpace10D& visitor)
{
    unsigned int size = MIN(this->mSize, visitor.mSize);

    for(unsigned int index = 0; index < size; ++index)
    {
        this->mSpace[index] &= visitor.mSpace[index];
    }

    return *this;
}

HSpace10D HSpace10D::operator| (const HSpace10D& visitor) const
{
    unsigned int size = MIN(this->mSize, visitor.mSize);

    HSpace10D result(size);
    for(unsigned int index = 0; index < size; ++index)
    {
        result.mSpace[index] = this->mSpace[index] | visitor.mSpace[index];
    }

    return result;
}

HSpace10D& HSpace10D::operator|= (const HSpace10D& visitor)
{
    unsigned int size = MIN(this->mSize, visitor.mSize);

    for(unsigned int index = 0; index < size; ++index)
    {
        this->mSpace[index] |= visitor.mSpace[index];
    }

    return *this;
}

void HSpace10D::fillAnd(HSET* space, unsigned int size, unsigned int start, unsigned int numbers[HSET_LEN])
{
    for(unsigned int index1 = 0; index1 < HSET_LEN; index1++) {
        unsigned int line = numbers[index1] - 1;
        HSET& cell = space[start + line];
        for(unsigned int index2 = 0; index2 < HSET_LEN; index2++) {
            unsigned int column = numbers[index2] - 1;
            cell |= (HONE << column);
        }
    }
}

void HSpace10D::fillOr(HSET* space, unsigned int size, unsigned int start, unsigned int numbers[HSET_LEN])
{
    for(unsigned int index1 = 0; index1 < HSET_LEN; index1++) {
        unsigned int line = numbers[index1] - 1;
        HSET& cell = space[start + line];
        for(unsigned int index2 = 0; index2 < HDIV_MAX; index2++) {
            cell |= (HONE << index2);
        }
    }
}

size_t HSpace10D::count1() const
{
    size_t result = 0;

    for(unsigned int index = 0; index < mSize; ++index)
    {
        result += hsetBitCount(mSpace[index]);
    }

    return result;
}

// HSpace10D(HDIV_MAX*HDIV_MAX)
void HSpace10D::fillAndOr(HSET hset)
{
    unsigned int numbers[HSET_LEN] = {};
    hsetToArray(hset, numbers);

    fillAnd(mSpace, HDIV_MAX, 0, numbers);
    fillOr(mSpace, HDIV_MAX, HDIV_MAX, numbers);
}

// HSpace10D(HDIV_MAX*HDIV_MAX)
void HSpace10D::fillOrAnd(HSET hset)
{
    unsigned int numbers[HSET_LEN] = {};
    hsetToArray(hset, numbers);

    fillOr(mSpace, HDIV_MAX, 0, numbers);
    fillAnd(mSpace, HDIV_MAX, HDIV_MAX, numbers);
}

// HSpace10D(HDIV_MAX)
void HSpace10D::fillAnd(HSET hset)
{
    unsigned int numbers[HSET_LEN] = {};
    hsetToArray(hset, numbers);

    fillAnd(mSpace, HDIV_MAX, 0, numbers);
}

// HSpace10D(HDIV_MAX)
void HSpace10D::fillOr(HSET hset)
{
    unsigned int numbers[HSET_LEN] = {};
    hsetToArray(hset, numbers);

    fillOr(mSpace, HDIV_MAX, 0, numbers);
}
