#include "CMesh.h"

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
	//glEnableClientState( GL_TEXTURE_COORD_ARRAY );  
	
	// Enable special attributes: Tangent and Binormal arrays
	//glEnableVertexAttribArray( this->iVertexTangentLoc );
	//glEnableVertexAttribArray( this->iVertexBinormalLoc );
	
	// Passing the information to openGL using pointers to the actual arrays
	glVertexPointer( 3, GL_FLOAT, sizeof(TVertex), &(this->iVertices[0].iPosition.x()) );
	glNormalPointer( GL_FLOAT, sizeof(TVertex), &(this->iVertices[0].iNormal.x()) );
	//glTexCoordPointer( 2, GL_FLOAT, sizeof(TVertex), &(this->iVertices[0].iTexCoord));	
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
	//glDisableClientState( GL_TEXTURE_COORD_ARRAY );

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