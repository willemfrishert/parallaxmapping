#pragma once

#include "Basic.h"
#include "TVertex.h"

class CMesh;
class ShaderAttributeObject;

/**
 * @description Convenience class to iterate through the triangles
 * of a mesh
 */
class TTriangleIterator
{
public:
	TTriangleIterator(CMesh* aMesh);
	bool getNextTriangle(GLuint* aIndices);

private:
	GLuint iCurrentTriangle[3];
	CMesh* iMesh;
};

class CMesh
{
  //PUBLIC METHODS
public:
	CMesh();
	~CMesh(void);

	void draw();

	void addVertex(const TVertex& aVertex, int aIndex = -1);
	void addTriangleIndices(const int& aIndex1, const int& aIndex2, const int& aIndex3);
	
	TVertex& getVertex(int aIndex = -1);
	int getIndicesCount() const;
	void getTriangleIndices(const GLuint* aTriangleIndices, GLuint* aVertexIndices) const;

	void setBinormalAttributeObject(ShaderAttributeObject* aBinormal);
	void setTangentAttributeObject(ShaderAttributeObject* aTangent);

	void createInverseTBNMatrices();
  
	//PRIVATE METHODS
private:
	void createInverseTBNMatrix(TVertex& aVertex0, const TVertex& aVertex1, 
		const TVertex& aVertex2);

	bool biasToZero(float& aValue);

  //PUBLIC VARIABLES
public:
    
  //PRIVATE VARIABLES
private:
	vector<TVertex> iVertices;
	vector<GLuint> iIndices;
	int iIndicesCount;
	
	/**
	 * The vertex attributes shader location, 
	 * for tangent and binormal
	 */
	ShaderAttributeObject* iVertexTangentAttribObject;
	ShaderAttributeObject* iVertexBinormalAttribObject;
};

/**
 * @param aVertex the vertex to be added
 * @param aIndex the index where the vertex should be added
 */
inline
void CMesh::addVertex(const TVertex& aVertex, int aIndex)
{
	if ( aIndex < 0)
	{
		this->iVertices.push_back( aVertex );
	}
	else if( aIndex < static_cast<int>( this->iVertices.size() ) )
	{
		this->iVertices[ aIndex ] = aVertex;
	}
}

/**
 * @param aIndex the index of the vertex
 * @return the vertex if the index is between bounds; otherwise the
 */
inline
TVertex& CMesh::getVertex(int aIndex)
{
	return this->iVertices.at( aIndex );
}

/**
 * @return the number of triangle indices in the mesh
 */
inline
int CMesh::getIndicesCount() const
{
	return this->iIndicesCount;
}
/**
 * @param aTriangleIndices the indices that correspond to the triangle description: input
 * @param aVertexIndices the indices of the vertices that describe the triangle: output
 */
inline
void CMesh::getTriangleIndices(const GLuint* aTriangleIndices, GLuint* aVertexIndices) const
{
	GLuint count = static_cast<GLuint>( this->getIndicesCount() );
	assert( (aTriangleIndices[0] < count) &&
		(aTriangleIndices[1] < count) &&
		(aTriangleIndices[2] < count));

	aVertexIndices[ 0 ] = this->iIndices[ aTriangleIndices[0] ];
	aVertexIndices[ 1 ] = this->iIndices[ aTriangleIndices[1] ];
	aVertexIndices[ 2 ] = this->iIndices[ aTriangleIndices[2] ];
}


/**
 * @param aIndex1 the first index of the triangle
 * @param aIndex2 the second index of the triangle
 * @param aIndex3 the third index of the triangle
 */
inline
void CMesh::addTriangleIndices(const int& aIndex1, const int& aIndex2, const int& aIndex3)
{
	this->iIndices.push_back( aIndex1 );
	this->iIndices.push_back( aIndex2 );
	this->iIndices.push_back( aIndex3 );

	this->iIndicesCount+=3;
}

/**
 * @description bias the value if it is between [-0.001f, 0.001f]
 * @param aValue the value to be eventually biased
 * @return true if the value was biased to zero; false otherwise
 */
inline
bool CMesh::biasToZero(float& aValue)
{
	if ( fabs( aValue ) < 0.001f )
	{
		aValue = 0.0f;
		return true;	
	}
	
	return false;
}