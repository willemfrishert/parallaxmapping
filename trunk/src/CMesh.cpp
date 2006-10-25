#include "CMesh.h"

#define ROUNDOFF(val) val

CMesh::CMesh()
: iIndicesCount( 0 )
{
}

CMesh::~CMesh(void)
{
}

/**
 * @param aLocation
 * @description sets the binormal location on the shader
 */
void CMesh::setBinormalLocation(GLuint aLocation)
{
	this->iVertexBinormalLoc = aLocation;
}

/**
 * @param aLocation
 * @description sets the tangent location on the shader
 */
void CMesh::setTangentLocation(GLuint aLocation)
{
	this->iVertexTangentLoc = aLocation;
}

/**
 *
 */
void CMesh::draw()
{
#ifndef GRAPHICS_DEBUG
	// Enable a vertex position, normal and texture coords arrays, respectively
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );  
	
	// Enable special attributes: Tangent and Binormal arrays
	//glEnableVertexAttribArray( this->iVertexTangentLoc );
	//glEnableVertexAttribArray( this->iVertexBinormalLoc );
	
	// Passing the information to openGL using pointers to the actual arrays
	glVertexPointer( 3, GL_FLOAT, sizeof(TVertex), &(this->iVertices[0].iPosition.x()) );
	glNormalPointer( GL_FLOAT, sizeof(TVertex), &(this->iVertices[0].iNormal.x()) );
	glTexCoordPointer( 2, GL_FLOAT, sizeof(TVertex), &(this->iVertices[0].iTexCoord.x()));

	//glVertexAttribPointer(this->iVertexTangentLoc, 3, GL_FLOAT, 0, sizeof(TVertex), 
	//	&(this->iVertices[0].iTangent));
	//glVertexAttribPointer(this->iVertexBinormalLoc, 3, GL_FLOAT, 0, sizeof(TVertex), 
	//	&(this->iVertices[0].iTangent));

	// the actual drawing
	glDrawElements(GL_TRIANGLES, this->iIndicesCount, GL_UNSIGNED_INT, &(this->iIndices.front()));

	//glDisableVertexAttribArray( this->iVertexBinormalLoc );
	//glDisableVertexAttribArray( this->iVertexTangentLoc );
	glDisableClientState( GL_VERTEX_ARRAY );	
	glDisableClientState( GL_NORMAL_ARRAY );	
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

#else
	TVertex v;
	TVector3<float> vec, norm;
	glLineWidth( 1.0f );
	glBegin(GL_TRIANGLES);
		for (int i = 0; i < this->iIndicesCount; i++)
		{
			v = this->iVertices[ this->iIndices[ i ] ];
			vec = v.iPosition;
			norm = v.iNormal;
			glVertex3f(vec.x(), vec.y(), vec.z());
			glNormal3f(norm.x(), norm.y(), norm.z());
		}
	glEnd();

	glDisable(GL_LIGHTING);
	glColor3f(1, 0, 1);
	glLineWidth( 2.0f );
	float len = 10;
	glBegin(GL_LINES);
		for (int i = 0; i < this->iVertices.size(); i++)
		{
			v = this->iVertices[ i ];
			vec = v.iPosition;
			norm = v.iNormal;
			glVertex3f(vec.x(), vec.y(), vec.z());
			glVertex3f(vec.x() + norm.x() * len, vec.y() + norm.y() * len, vec.z() + norm.z() * len);
		}
	glEnd();
#endif
}

/************************************************************************/
/* CONVENIENCE CLASS TTRIANGLEITERATOR                                  */
/************************************************************************/
TTriangleIterator::TTriangleIterator(CMesh* aMesh)
: iMesh( aMesh )
{
	this->iCurrentTriangle[0] = 0;
	this->iCurrentTriangle[1] = 1;
	this->iCurrentTriangle[2] = 2;
}

bool TTriangleIterator::getNextTriangle(GLuint* aIndices)
{
	if (this->iCurrentTriangle[2] < static_cast<GLuint>( this->iMesh->getIndicesCount() ))
	{
		this->iMesh->getTriangleIndices(this->iCurrentTriangle, aIndices);

		this->iCurrentTriangle[0]++;
		this->iCurrentTriangle[1]++;
		this->iCurrentTriangle[2]++;

		return true;
	}

	return false;
}

/**
 * @param aVertices
 * @param aTextureCoordinates
 * @param aInverseNormal
 * @param aInverseBinormal
 * @param aInverseTangent
 */
void CMesh::CreateInverseTBNMatrix(const TVector3<float> aVertices[3], const TVector2<float> aTextureCoordinates[3],
								   TVector3<float>& aInverseNormal, TVector3<float>& aInverseBinormal, TVector3<float>& aInverseTangent)
{
	TVector3<float> v0v1 = aVertices[1] - aVertices[0];
	TVector3<float> v0v2 = aVertices[2] - aVertices[0];

	float c0c1_T = aTextureCoordinates[1].x() - aTextureCoordinates[0].x();
	float c0c1_B = aTextureCoordinates[1].y() - aTextureCoordinates[0].y();

	float c0c2_T = aTextureCoordinates[2].x() - aTextureCoordinates[0].x();
	float c0c2_B = aTextureCoordinates[2].y() - aTextureCoordinates[0].y();

	float denominator = c0c1_T*c0c2_B-c0c2_T*c0c1_B;

	if ( ROUNDOFF(denominator) )
	{
		aInverseTangent = TVector3<float>(1.0f, 0.0f, 0.0f);
		aInverseBinormal= TVector3<float>(0.0f, 1.0f, 0.0f);
		aInverseNormal  = TVector3<float>(0.0f, 0.0f, 1.0f);
	}
	else
	{
		float fScale1=1.0f/denominator;

		TVector3<float> T = TVector3<float>( (c0c2_B  * v0v1.x() - c0c1_B * v0v2.x()) * fScale1,
			(c0c2_B  * v0v1.y() - c0c1_B * v0v2.y()) * fScale1,
			(c0c2_B  * v0v1.z() - c0c1_B * v0v2.z()) * fScale1 );

		TVector3<float> B = TVector3<float>( (-c0c2_T * v0v1.x() + c0c1_T * v0v2.x()) * fScale1,
			(-c0c2_T * v0v1.y() + c0c1_T * v0v2.y()) * fScale1,
			(-c0c2_T * v0v1.z() + c0c1_T * v0v2.z()) * fScale1 );

		TVector3<float> N = T^B;

		float fScale2 = 1.0f / ( (T.x() * B.y() * N.z() - T.z() * B.y() * N.x()) +
			(B.x() * N.y() * T.z() - B.z() * N.y() * T.x()) +
			(N.x() * T.y() * B.z() - N.z() * T.y() * B.x()) );

		aInverseTangent.set(  (B^N).x() * fScale2,
			((-N)^T).x() * fScale2,
			(T^B).x() * fScale2 );

		aInverseBinormal.set( ((-B)^N).y() * fScale2,
			(N^T).y() * fScale2,
			((-T)^N).y() * fScale2 );

		aInverseNormal.set(   (B^N).z() * fScale2,
			((-N)^T).z() * fScale2,
			(T^B).z() * fScale2 );
	}
}