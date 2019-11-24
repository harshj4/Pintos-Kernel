#ifndef _FIXED_POINT_H_
#define _FIXED_POINT_H_

#include <stdint.h>

#define Fixed int32_t
#define scale 14
#define Precision 16384
// const int FractionMask = 0xffffffff >> (32-scale);
// const int WholeMask = (-1) ^ FractionMask;
#define FractionMask (0xffffffff >> (32-scale))
#define WholeMask ((-1) ^ FractionMask)

Fixed InttoFixed(int x);
int FixedtoInt(Fixed x);
#define FractionPart(x) ((x)&FractionMask)
#define WholePart(x) ((x)&WholeMask)

#define Add(x,y) (FixedtoInt(InttoFixed(x) + InttoFixed(y)))
#define Sub(x,y) (FixedtoInt(InttoFixed(x) - InttoFixed(y)))

Fixed Multiply(Fixed x,Fixed y);
//TODO: check for correct term
Fixed Divide(Fixed x,Fixed y);

#endif //_FIXED_POINT_H_
