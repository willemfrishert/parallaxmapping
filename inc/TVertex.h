/**
 * @description NOTE: the iNormal, iBinormal and iTangent can be used either
 * to store the normal, binormal and tangent, or the inverse normal, inverse binormal
 * and inverse tangent. This will, of course, depend on the mapping technique we're
 * using.
 * 
 * @file TVertex.h
 * @author João Pedro Jorge
 */
#pragma once

#include "TVector3.h"
#include "TVector2.h"

class TVertex
{
public:
	TVertex(void);
	TVertex(const TVector3<float>& aNormal, const TVector3<float>& aPosition, const TVector2<float>& aTexCoord);
	TVertex(const TVector3<float>& aNormal, const TVector3<float>& aPosition, const TVector2<float>& aTexCoord, 
		const TVector3<float>& aBinormal, const TVector3<float>& aTangent);

	~TVertex(void);

	//PRIVATE DATA
public:
	TVector3<float> iPosition;

	TVector3<float> iNormal;

	TVector2<float> iTexCoord;

	TVector3<float> iBinormal;

	TVector3<float> iTangent;
};
