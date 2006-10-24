#include "TVertex.h"

TVertex::TVertex(void)
{
}

TVertex::TVertex(const TVector3<float>& aNormal, const TVector3<float>& aPosition, 
				 const TVector2<float>& aTexCoord)
: iNormal( aNormal )
, iPosition( aPosition )
, iTexCoord( aTexCoord )
{
}

TVertex::TVertex(const TVector3<float>& aNormal, const TVector3<float>& aPosition, const TVector2<float>& aTexCoord, 
				 const TVector3<float>& aBinormal, const TVector3<float>& aTangent)
: iNormal( aNormal )
, iPosition( aPosition )
, iBinormal( aBinormal )
, iTexCoord( aTexCoord )
, iTangent( aTangent )
{
}

TVertex::~TVertex(void)
{
}
