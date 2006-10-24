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
