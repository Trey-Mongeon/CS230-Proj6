//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 3
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Matrix2D.h"
#include "DGL.h"
#define _USE_MATH_DEFINES
#include <math.h>

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// This function sets the matrix equal to the identity matrix.
void Matrix2DIdentity(Matrix2D* pResult)
{
	int i = 0;
		for (i; i < 4; ++i)
		{
			int j = 0;
			for (j; j < 4; ++j)
			{
				if (i == j)
				{
					pResult->m[i][j] = 1.0f;
				}
				else
				{
					pResult->m[i][j] = 0.0f;
				}
			}
		}
}

// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
// (NOTE: Care must be taken when pResult = pMtx.)
void Matrix2DTranspose(Matrix2D* pResult, const Matrix2D* pMtx)
{
	if (pMtx)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = i; j < 4; ++j)
			{
				float tempIndex = pMtx->m[i][j];

				pResult->m[i][j] = pMtx->m[j][i];

				pResult->m[j][i] = tempIndex;
			}
		}
	}
}

// This function multiplies Mtx0 with Mtx1 and saves the result in Result.
// Result = Mtx0*Mtx1
// (NOTE: Care must be taken when pResult = either pMtx0 or pMtx1.)
void Matrix2DConcat(Matrix2D* pResult, const Matrix2D* pMtx0, const Matrix2D* pMtx1)
{
	Matrix2D temp0 = *pMtx0;
	Matrix2D temp1 = *pMtx1;

	if (pMtx0 && pMtx1)
	{
		for (int i = 0; i < 4; ++i)
		{

			for (int j = 0; j < 4; ++j)
			{
				float summation = 0;

				for (int k = 0; k < 4; ++k)
				{
					summation += temp0.m[i][k] * temp1.m[k][j];
				}
					pResult->m[i][j] = summation;
			}
		}
	}
}

// This function creates a translation matrix from x & y and saves it in Result.
// (Hint: This function must initialize all matrix values.)
void Matrix2DTranslate(Matrix2D* pResult, float x, float y)
{
	Matrix2DIdentity(pResult);
	pResult->m[0][3] = x;
	pResult->m[1][3] = y;
}

// This function creates a scaling matrix from x & y and saves it in Result.
// (Hint: This function must initialize all matrix values.)
void Matrix2DScale(Matrix2D* pResult, float x, float y)
{
	Matrix2DIdentity(pResult);
	pResult->m[0][0] = x;
	pResult->m[1][1] = y;
}

// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
// (Hint: This function must initialize all matrix values.)
// Converting from degrees to radians can be performed as follows:
//	 radians = (angle * M_PI) / 180.0f
// M_PI is defined in "math.h", which may be included as follows:
//   #define _USE_MATH_DEFINES
//   #include <math.h>
void Matrix2DRotDeg(Matrix2D* pResult, float angle)
{
	Matrix2DIdentity(pResult);
	float rad = angle * (float)M_PI / 180;
	Matrix2DRotRad(pResult, rad);
}

// This matrix creates a rotation matrix from "Angle" whose value is in radians.
// (Hint: This function must initialize all matrix values.)
void Matrix2DRotRad(Matrix2D* pResult, float angle)
{
	Matrix2DIdentity(pResult);
	pResult->m[0][0] = cosf(angle);
	pResult->m[0][1] = -sinf(angle);
	pResult->m[1][0] = sinf(angle);
	pResult->m[1][1] = cosf(angle);
}

// This function multiplies the matrix Mtx with the vector Vec and saves the result in Result.
// Result = Mtx * Vec.
// (NOTE: Care must be taken when pResult = pVec.)
// (NOTE: Remember to treat the implied w component as 1.)
void Matrix2DMultVec(Vector2D* pResult, const Matrix2D* pMtx, const Vector2D* pVec)
{
	Vector2D tempVec;

	tempVec.x = pMtx->m[0][0] * pVec->x;
	tempVec.x += pMtx->m[0][1] * pVec->y;
	tempVec.x += pMtx->m[0][3];
		   
	tempVec.y = pMtx->m[1][0] * pVec->x;
	tempVec.y += pMtx->m[1][1] * pVec->y;	
	tempVec.y += pMtx->m[1][3];

		   
	pResult->x = tempVec.x;
	pResult->y = tempVec.y;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

