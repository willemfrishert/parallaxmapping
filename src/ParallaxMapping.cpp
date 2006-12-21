#include "ParallaxMapping.h"


ParallaxMapping::ParallaxMapping()
{
	iVertexShader = new ShaderObject(GL_VERTEX_SHADER, "shader/parallaxmap.vert");
	iFragmentShader = new ShaderObject(GL_FRAGMENT_SHADER, "shader/parallaxmap.frag");

	iShaderProgram = new ShaderProgram();

	iShaderProgram->attachShader( *iVertexShader);
	iShaderProgram->attachShader( *iFragmentShader );

	iTangentAttributeObject = new ShaderAttributeObject("tangent");
	iBinormalAttributeObject = new ShaderAttributeObject("binormal");
	iTbnNormalAttributeObject = new ShaderAttributeObject("tbnNormal");

	iTextureMapUniformObject = new ShaderUniformValue<int>();
	iTextureMapUniformObject->setName("textureMap");
	iTextureMapUniformObject->setValue( 0 );

	iHeightMapUniformObject = new ShaderUniformValue<int>();
	iHeightMapUniformObject->setName("heightMap");
	iHeightMapUniformObject->setValue( 1 );

	iNormalMapUniformObject = new ShaderUniformValue<int>();
	iNormalMapUniformObject->setName("normalMap");
	iNormalMapUniformObject->setValue( 2 );

	iParallaxBiasUniformObject = new ShaderUniformValue<float>();
	iParallaxBiasUniformObject->setName("parallaxBias");
	iParallaxBiasUniformObject->setValue( 0.05f );

	iShaderProgram->addAttributeObject( iTangentAttributeObject );
	iShaderProgram->addAttributeObject( iBinormalAttributeObject );
	iShaderProgram->addAttributeObject( iTbnNormalAttributeObject );
	iShaderProgram->addUniformObject( iTextureMapUniformObject );
	iShaderProgram->addUniformObject( iHeightMapUniformObject );
	iShaderProgram->addUniformObject( iNormalMapUniformObject );
	iShaderProgram->addUniformObject( iParallaxBiasUniformObject );

	iShaderProgram->buildProgram();
}
ParallaxMapping::~ParallaxMapping()
{
	delete iTangentAttributeObject;
	delete iTbnNormalAttributeObject;
	delete iBinormalAttributeObject;
	delete iShaderProgram;
	delete iVertexShader;
	delete iFragmentShader;
	delete iTextureMapUniformObject;
	delete iHeightMapUniformObject;
	delete iNormalMapUniformObject;
	delete iParallaxBiasUniformObject;
}

/**
 * NOTE: Call first setBiasValue
 * @param aObject
 * @param aTextureIds
 */
void ParallaxMapping::setup(CMesh* aObject, GLuint* aTextureIds)
{	
	iShaderProgram->updateProgramUniformObjects();

	aObject->setBinormalAttributeObject( iBinormalAttributeObject );
	aObject->setTangentAttributeObject( iTangentAttributeObject );
	aObject->setTBNNormalAttributeObject( iTbnNormalAttributeObject );
	
	resetMultitexturing(aTextureIds);
}

void ParallaxMapping::enable()
{
	this->iShaderProgram->useProgram();
}

void ParallaxMapping::disable()
{
	this->iShaderProgram->disableProgram();
}

void ParallaxMapping::resetMultitexturing(GLuint* aTextureIds)
{
	// initialize multitexturing
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, aTextureIds[0]);

	glActiveTexture(GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_2D, aTextureIds[1]);

	glActiveTexture(GL_TEXTURE2 );
	glBindTexture(GL_TEXTURE_2D, aTextureIds[2]);
}

void ParallaxMapping::setBiasValue(float value)
{
	iParallaxBiasUniformObject->setValue( value );
}