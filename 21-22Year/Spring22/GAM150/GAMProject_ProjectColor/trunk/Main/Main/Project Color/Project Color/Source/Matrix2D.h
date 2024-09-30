//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 3
// Course:		CS230S22
//
// Copyright (C) 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Vector2D Vector2D;

//------------------------------------------------------------------------------
// Public Macros:
//------------------------------------------------------------------------------

#define Matrix2DRowCol(pMtx, row, col) (pMtx)->m[(row)][(col)]

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Frequently, vector and matrix structures are declared publicly.  While an
// interface is typically provided with these structures, these are two common
// exceptions to the coding best practice of "hiding information".

// WARNING: Do not modify the declaration of this structure, as it is a part
// of the public interface.

typedef struct Matrix2D
{
  // The matrix is stored in row-major format, meaning the consecutive
  //   elements of rows are contiguous.
  //   m[row][column]
  float m[3][3];
} Matrix2D;

//------------------------------------------------------------------------------
// Public Function Declarations:
//------------------------------------------------------------------------------

// This function sets the matrix equal to the identity matrix.
void Matrix2DIdentity(Matrix2D* pResult);

// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
// (NOTE: Care must be taken when pResult = pMtx.)
void Matrix2DTranspose(Matrix2D* pResult, Matrix2D* pMtx);

// This function multiplies Mtx0 with Mtx1 and saves the result in Result.
// Result = Mtx0*Mtx1
// (NOTE: Care must be taken when pResult = either pMtx0 or pMtx1.)
void Matrix2DConcat(Matrix2D* pResult, Matrix2D* pMtx0, Matrix2D* pMtx1);

// This function creates a translation matrix from x & y and saves it in Result.
// (Hint: This function must initialize all matrix values.)
void Matrix2DTranslate(Matrix2D* pResult, float x, float y);

// This function creates a scaling matrix from x & y and saves it in Result.
// (Hint: This function must initialize all matrix values.)
void Matrix2DScale(Matrix2D* pResult, float x, float y);

// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
// (Hint: This function must initialize all matrix values.)
// Converting from degrees to radians can be performed as follows:
//	 radians = (angle * M_PI) / 180.0f
// M_PI is defined in "math.h", which may be included as follows:
//   #define _USE_MATH_DEFINES
//   #include <math.h>
void Matrix2DRotDeg(Matrix2D* pResult, float angle);

// This matrix creates a rotation matrix from "Angle" whose value is in radians.
// (Hint: This function must initialize all matrix values.)
void Matrix2DRotRad(Matrix2D* pResult, float angle);

// This function multiplies the matrix Mtx with the vector Vec and saves the result in Result.
// Result = Mtx * Vec.
// (NOTE: Care must be taken when pResult = pVec.)
// (NOTE: Remember to treat the implied w component as 1.)
void Matrix2DMultVec(Vector2D* pResult, Matrix2D* pMtx, Vector2D* pVec);
