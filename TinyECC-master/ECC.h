/**
 * All new code in this distribution is Copyright 2005 by North Carolina
 * State University. All rights reserved. Redistribution and use in
 * source and binary forms are permitted provided that this entire
 * copyright notice is duplicated in all such copies, and that any
 * documentation, announcements, and other materials related to such
 * distribution and use acknowledge that the software was developed at
 * North Carolina State University, Raleigh, NC. No charge may be made
 * for copies, derivations, or distributions of this material without the
 * express written consent of the copyright holder. Neither the name of
 * the University nor the name of the author may be used to endorse or
 * promote products derived from this material without specific prior
 * written permission.
 *
 * IN NO EVENT SHALL THE NORTH CAROLINA STATE UNIVERSITY BE LIABLE TO ANY
 * PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
 * DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION,
 * EVEN IF THE NORTH CAROLINA STATE UNIVERSITY HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE. THE SOFTWARE PROVIDED HEREUNDER IS ON AN
 * "AS IS" BASIS, AND THE NORTH CAROLINA STATE UNIVERSITY HAS NO
 * OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 * MODIFICATIONS. "
 *
 */

/**
 * $Id: ECC.h,v 1.13 2007/11/02 22:36:39 aliu3 Exp $
 * Ecc.h
 * define the data structure for ECC operation
 */

#ifndef _ECC_H_
#define _ECC_H_

#include "NN.h"


#ifndef PROJECTIVE
#define AFFINE
#endif

#ifdef PROJECTIVE
//enable mixed projective coordinate addition
#define ADD_MIX
//enable repeated point doubling
#define REPEAT_DOUBLE
#endif


#ifdef SLIDING_WIN
//The size of sliding window, must be power of 2 (change this if you
//want to use other window size, for example: 2 or 4)
#define W_BITS 4

//basic mask used to generate mask array (you need to change this if
//you want to change window size)
//For example: if W_BITS is 2, BASIC_MASK must be 0x03;
//             if W_BITS is 4, BASIC_MASK must be 0x0f
//			   if W_BITS is 8, BASIC_MASK must be 0xff
#define BASIC_MASK 0xf

//number of windows in one digit, NUM_MASKS = NN_DIGIT_BITS/W_BITS
#define NUM_MASKS (NN_DIGIT_BITS/W_BITS)

//number of points for precomputed points, NN_POINTS = 2^W_BITS - 1
#define NUM_POINTS ((1 << W_BITS) - 1)

#endif

//the data structure define the elliptic curve
struct Curve
{
  // curve's coefficients
  NN_DIGIT a[NUMWORDS];
  NN_DIGIT b[NUMWORDS];
  
  //whether a is -3
  bool a_minus3;
  
  //whether a is zero
  bool a_zero;
  
  bool a_one;
};
typedef struct Curve Curve;

struct Point
{
  // point's coordinates
  NN_DIGIT x[NUMWORDS];
  NN_DIGIT y[NUMWORDS];
};
typedef struct Point Point;

struct ZCoordinate
{
  NN_DIGIT z[NUMWORDS];
};
typedef struct ZCoordinate ZCoordinate;

//all the parameters needed for elliptic curve operation
struct Params
{
    // prime modulus
    NN_DIGIT p[NUMWORDS];

    // Omega, p = 2^m -omega
    NN_DIGIT omega[NUMWORDS];

    // curve over which ECC will be performed
    Curve E;

    // base point, a point on E of order r
    Point G;

    // a positive, prime integer dividing the number of points on E
    NN_DIGIT r[NUMWORDS];

    // a positive prime integer, s.t. k = #E/r
//    NN_DIGIT k[NUMWORDS];
};
typedef struct Params Params;

struct ECCState{
  Params p;
  Barrett b;
  Point pBaseArray[NUM_POINTS];
  NN_DIGIT mask[NUM_MASKS];
};
typedef struct ECCState ECCState;

//all the parameters needed for elliptic curve operations of Tate Pairing
struct TPParams
{
  // prime modulus
  NN_DIGIT p[NUMWORDS];
  
  // curve over which ECC will be performed
  Curve E;
  
  // group order m
  NN_DIGIT m[NUMWORDS];
  
  // c = ((p^k)-1)/m
  NN_DIGIT c[NUMWORDS];
  
  // point P
  Point P;
};
typedef struct TPParams TPParams;

//structure for precomputed points and slope
struct PointSlope {
  bool dbl;  //TRUE for double, FALSE for add
  Point P;
  NN_DIGIT slope[NUMWORDS];
  struct PointSlope * next;
};
typedef struct PointSlope PointSlope;

#endif
