//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.c
// Author(s):	Emily Comes (emily.comes)
// Project:		Project 3
// Course:		CS230S22
//
// Copyright (C) 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Matrix2D.h"
#include "Vector2D.h"
#include <stddef.h>

#define _USE_MATH_DEFINES
#include <math.h>

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// This function sets the matrix equal to the identity matrix.
void Matrix2DIdentity(Matrix2D* pResult)
{
  if (pResult == NULL || pResult->m == NULL) // do nothing if any pointers are null
    return;
  for (char row = 0; row < 3; ++row)
    for (char col = 0; col < 3; ++col)
      pResult->m[row][col] = 1.f * (row == col);
}

// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
// (NOTE: Care must be taken when pResult = pMtx.)
void Matrix2DTranspose(Matrix2D* pResult, Matrix2D* pMtx)
{
  if (pResult == NULL || pResult->m == NULL || pMtx == NULL || pMtx->m == NULL) // do nothing if any pointers are null
    return;

  if (pResult != pMtx) // overwrite contents of pResult does not point to the same data as pMtx
    for (char row = 0; row < 3; ++row)
      for (char col = 0; col < 3; ++col)
        pResult->m[row][col] = pMtx->m[row][col];

  float temp = pMtx->m[0][1];
  pResult->m[0][1] = pMtx->m[1][0];
  pResult->m[1][0] = temp;

  temp = pMtx->m[0][2];
  pResult->m[0][2] = pMtx->m[2][0];
  pResult->m[2][0] = temp;

  temp = pMtx->m[1][2];
  pResult->m[1][2] = pMtx->m[2][1];
  pResult->m[2][1] = temp;
}

// This function multiplies Mtx0 with Mtx1 and saves the result in Result.
// Result = Mtx0*Mtx1
// (NOTE: Care must be taken when pResult = either pMtx0 or pMtx1.)
void Matrix2DConcat(Matrix2D* pResult, Matrix2D* pMtx0, Matrix2D* pMtx1)
{
  if (pResult == NULL || pResult->m == NULL || pMtx0 == NULL || pMtx0->m == NULL || pMtx1 == NULL || pMtx1->m == NULL) // do nothing if any pointers are null
    return;

  Matrix2D tempMtx;
  for (char row = 0; row < 3; ++row)
    for (char col = 0; col < 3; ++col)
      tempMtx.m[row][col] = pMtx0->m[row][0] * pMtx1->m[0][col] + pMtx0->m[row][1] * pMtx1->m[1][col] + pMtx0->m[row][2] * pMtx1->m[2][col];

  for (char row = 0; row < 3; ++row)
    for (char col = 0; col < 3; ++col)
      pResult->m[row][col] = tempMtx.m[row][col];
}

// This function creates a translation matrix from x & y and saves it in Result.
// (Hint: This function must initialize all matrix values.)
void Matrix2DTranslate(Matrix2D* pResult, float x, float y)
{
  if (pResult == NULL || pResult->m == NULL) // do nothing if any pointers are null
    return;
  Matrix2DIdentity(pResult);
  pResult->m[0][2] = x;
  pResult->m[1][2] = y;
}

// This function creates a scaling matrix from x & y and saves it in Result.
// (Hint: This function must initialize all matrix values.)
void Matrix2DScale(Matrix2D* pResult, float x, float y)
{
  if (pResult == NULL || pResult->m == NULL) // do nothing if any pointers are null
    return;
  Matrix2DIdentity(pResult);
  pResult->m[0][0] = x;
  pResult->m[1][1] = y;
}

// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
// (Hint: This function must initialize all matrix values.)
// Converting from degrees to radians can be performed as follows:
//	 radians = (angle * M_PI) / 180.f
// M_PI is defined in "math.h", which may be included as follows:
//   #define _USE_MATH_DEFINES
//   #include <math.h>
void Matrix2DRotDeg(Matrix2D* pResult, float angle)
{
  if (pResult == NULL || pResult->m == NULL || angle == 0.f || angle == 360.f) // do nothing if any pointers are null or angle is stupid
    return;
  Matrix2DRotRad(pResult, (angle * (float)M_PI) / 180.f);
}

// This matrix creates a rotation matrix from "Angle" whose value is in radians.
// (Hint: This function must initialize all matrix values.)
void Matrix2DRotRad(Matrix2D* pResult, float angle)
{
  if (pResult == NULL || pResult->m == NULL || angle == 0.f) // do nothing if any pointers are null or angle is stupid
    return;
  Matrix2DIdentity(pResult);
  pResult->m[0][0] = pResult->m[1][1] = cosf(angle);
  pResult->m[1][0] = sinf(angle);
  pResult->m[0][1] = -1.f * pResult->m[1][0]; // avoids calculating sine of the angle again by just copying the already calculated value and then negating it
}

// This function multiplies the matrix Mtx with the vector Vec and saves the result in Result.
// Result = Mtx * Vec.
// (NOTE: Care must be taken when pResult = pVec.)
// (NOTE: Remember to treat the implied w component as 1.)
void Matrix2DMultVec(Vector2D* pResult, Matrix2D* pMtx, Vector2D* pVec)
{
  if (pResult == NULL || pMtx == NULL || pMtx->m == NULL || pVec == NULL) // do nothing if any pointers are null
    return;
  Vector2DSet(pResult, pVec->x + pMtx->m[0][2], pVec->y + pMtx->m[1][2]);
}
