#include "TextureMapping.h"


TextureMapping::TextureMapping()
{
	iVertexShader = new ShaderObject(GL_VERTEX_SHADER, "shader/texturemap.vert");
	iFragmentShader = new ShaderObject(GL_FRAGMENT_SHADER, "shader/texturemap.frag");

	iShaderProgram = new ShaderProgram();

	iShaderProgram->attachShader( *iVertexShader);
	iShaderProgram->attachShader( *iFragmentShader );

	iTextureMapUniformObject = new ShaderUniformValue<int>();
	iTextureMapUniformObject->setName("textureMap");
	iTextureMapUniformObject->setValue( 0 );

	iShaderProgram->addUniformObject( iTextureMapUniformObject );

	iShaderProgram->buildProgram();
}
TextureMapping::~TextureMapping()
{
	delete iShaderProgram;
	delete iVertexShader;
	delete iFragmentShader;
	delete iTextureMapUniformObject;
}

/**
 * NOTE: Call first setBiasValue
 * @param aObject
 * @param aTextureIds
 */
void TextureMapping::setup(CMesh* aObject, GLuint* aTextureIds)
{	
	resetMultitexturing(aTextureIds);
}

void TextureMapping::enable()
{
	this->iShaderProgram->useProgram();
}

void TextureMapping::disable()
{
	this->iShaderProgram->disableProgram();
}

void TextureMapping::resetMultitexturing(GLuint* aTextureIds)
{
	// initialize multitexturing
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, aTextureIds[0]);
}