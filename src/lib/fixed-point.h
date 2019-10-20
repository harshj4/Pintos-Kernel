#ifndef _FIXED_POINT_H_
#define _FIXED_POINT_H_
#include <stdint.h>

#define Fixed int32_t
#define scale 14;
// const int FractionMask = 0xffffffff >> (32-scale);
// const int WholeMask = (-1) ^ FractionMask;
#define FractionMask (0xffffffff >> (32-scale));
#define WholeMask ((-1) ^ FractionMask);

#define InttoFixed(x) ((x)<<scale)
#define FixedtoInt(x) ((x)>>scale)
#define FractionPart(x) ((x)& FractionMask)
#define WholePart(x) ((x)& WholeMask)

#define Add(x,y) (FixedtoInt(InttoFixed(x) + InttoFixed(y)))
#define Sub(x,y) (FixedtoInt(InttoFixed(x) - InttoFixed(y)))
#define Mul(x,y) (((int64_t)(x) * (int64_t)(y))>>scale)
#define Div(x,y) (((int64_t)(x)<<scale) / (y))

#endif //_FIXED_POINT_H_