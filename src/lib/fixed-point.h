#ifndef _FIXED_POINT_H_
#define _FIXED_POINT_H_

#include <stdint.h>

#define Fixed int32_t
#define scale 14
#define Precision 16384
// const int FractionMask = 0xffffffff >> (32-scale);
// const int WholeMask = (-1) ^ FractionMask;
#define FractionMask (0xffffffff >> (32-scale))
#define WholeMask (((unsigned)-1) ^ FractionMask)

#define FractionPart(x) ((x)&FractionMask)
#define WholePart(x) ((x)&WholeMask)

// #define Add(x,y) (FixedtoInt(InttoFixed(x) + InttoFixed(y)))
// #define Sub(x,y) (FixedtoInt(InttoFixed(x) - InttoFixed(y)))

#define I2F InttoFixed
#define F2I FixedtoInt

Fixed InttoFixed(int x);
int FixedtoInt(Fixed x);

/*Adds fixed-point x and integer n and returns a fixed-point number.*/
Fixed Add(Fixed x, int n);

/*Subtracts fixed-point x and integer n and returns a fixed-point number.*/
Fixed Subtract(Fixed x, int n);

/*Multiplies two fixed-point numbers x and y and returns a fixed-point number.
Not necesslar ary if one of the two numbers is a regular integer*/
Fixed Multiply(Fixed x,Fixed y);

/*Divides two fixed-point numbers x and y and returns a fixed-point number.
Not necesslar ary if one of the two numbers is a regular integer*/
Fixed Divide(Fixed x,Fixed y);



#endif //_FIXED_POINT_H_