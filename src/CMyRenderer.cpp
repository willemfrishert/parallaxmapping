//INCLUDES
#include "CMyRenderer.h"
#include "TVertex.h"
#include "3ds.h"
#include "tga.h"

//USE THIS FILE


//CONSTANTS
const int KTextureWidth = 512; ///< Texture max size
const int KTextureHeight= 512; ///< Texture max size

//Aspect ratio common values: 4/3(TV), 1.66(Cartoons), 16/9(HDTV), 2.35(Epic Movies)
const float KPerspectiveAspectRatio = float(4/3);
const float KPerspectiveAngle = float(45);

//VIEW FRUSTUM
const float KZNear	= 10.0f;
const float KZFar	= 600.0f;

//INIT STATIC DATA
CMyRenderer* CMyRenderer::iCurrentRenderer = 0;

//CONSTRUCTORS
//
//Default constructor
CMyRenderer::CMyRenderer()
: iFrame(0)
, iPreviousFrame(0)
, iPreviousTime(0)
, angle(0)
, textureMapId(0)
, heightMapId(0)
, tangentAttributeObject(0)
, binormalAttributeObject(NULL)
, textureMapUniformObject(NULL)
, heightMapUniformObject(NULL)	
, iXRotation( 0 )
, iYRotation( 0 )
, iOldXRotation( 0 )
, iOldYRotation( 0 )
, iXPosition( 0 )
, iYPosition( 0 )
, iZoom( 10 )
{
	InitMain();
	CMyRenderer::iCurrentRenderer = this;
}


//
//DESTRUCTOR releases the used heap
//
CMyRenderer::~CMyRenderer()
{
}

void CMyRenderer::InitShaders()
{
	iVertexShader = new ShaderObject(GL_VERTEX_SHADER, "shader/parallaxmap.vert");
	iFragmentShader = new ShaderObject(GL_FRAGMENT_SHADER, "shader/parallaxmap.frag");

	iShaderProgram = new ShaderProgram();
	
	iShaderProgram->attachShader( *iVertexShader);
	iShaderProgram->attachShader( *iFragmentShader );

	tangentAttributeObject = new ShaderAttributeObject("tangent");
	binormalAttributeObject = new ShaderAttributeObject("binormal");
	tbnNormalAttributeObject = new ShaderAttributeObject("tbnNormal");

	textureMapUniformObject = new ShaderUniformValue<int>();
	textureMapUniformObject->setName("textureMap");
	textureMapUniformObject->setValue( 0 );
	
	heightMapUniformObject = new ShaderUniformValue<int>();
	heightMapUniformObject->setName("heightMap");
	heightMapUniformObject->setValue( 1 );

	normalMapUniformObject = new ShaderUniformValue<int>();
	normalMapUniformObject->setName("normalMap");
	normalMapUniformObject->setValue( 2 );

	iShaderProgram->addAttributeObject( tangentAttributeObject );
	iShaderProgram->addAttributeObject( binormalAttributeObject );
	iShaderProgram->addAttributeObject( tbnNormalAttributeObject );
	iShaderProgram->addUniformObject( textureMapUniformObject );
	iShaderProgram->addUniformObject( heightMapUniformObject );
	iShaderProgram->addUniformObject( normalMapUniformObject );

	iShaderProgram->buildProgram();

	ResetMultitexturing();
}

void CMyRenderer::LoadTextures()
{
	rockwallTextures[0] = loadTGATexture("textures/rockwall_rot.tga");
	rockwallTextures[1] = loadTGATexture("textures/rockwall_height_rot.tga");
	
	// generate normal map using ATI's TGAtoDOT3 functions
	rockwallTextures[2] = GenerateDOT3( heightMapId );

	textureMapId	= rockwallTextures[0];
	heightMapId		= rockwallTextures[1];
	normalMapId		= rockwallTextures[2];

}

//
/// Init function for the constructors
void CMyRenderer::InitMain()
{
	//Init view
	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE); // Enable the back face culling
	glEnable(GL_DEPTH_TEST); // Enable the depth test (z-buffer)

	textureMapId = loadTGATexture("textures/rockwall_rot.tga");
	heightMapId = loadTGATexture("textures/rockwall_height_rot.tga");
	
	// generate normal map using ATI's TGAtoDOT3 functions
	normalMapId = GenerateDOT3( heightMapId );

	InitShaders();
}

/**
 */
void CMyRenderer::ResetMultitexturing()
{
	// initialize multitexturing
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, textureMapId);

	glActiveTexture(GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_2D, heightMapId);

	glActiveTexture(GL_TEXTURE2 );
	glBindTexture(GL_TEXTURE_2D, normalMapId);
}

void CMyRenderer::ReadPixel(GLubyte *image, GLubyte *pix, int x, int y, int width)
{
	pix[0] = *(image+width*3*y+x*3+0);
	pix[1] = *(image+width*3*y+x*3+1);
	pix[2] = *(image+width*3*y+x*3+2);	
}

void CMyRenderer::WritePixel(GLubyte *image, GLubyte *pix, int x, int y, int width)
{
	*(image+width*3*y+x*3+0) = pix[0];
	*(image+width*3*y+x*3+1) = pix[1];
	*(image+width*3*y+x*3+2) = pix[2];
}

GLuint CMyRenderer::GenerateDOT3( GLuint aHeightMapId )
{		
	glBindTexture( GL_TEXTURE_2D, aHeightMapId );

	GLenum target = GL_TEXTURE_2D;
	GLenum format = GL_RGB;
	GLenum type = GL_UNSIGNED_BYTE;	

	GLint level;	
	glGetTexParameteriv( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, &level);
	GLint internalFormat;
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat );
	GLint border;
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_BORDER, &border );

	GLint width;
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width );
	GLint height;
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height );


	GLubyte* srcImage = new GLubyte[width*height*3];
	GLubyte* dstImage = new GLubyte[width*height*3];	

	glGetTexImage( GL_TEXTURE_2D, 0, format, type, srcImage);

	float dX, dY, nX, nY, nZ, oolen;
	GLubyte pix[3];
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// Do Y Sobel filter
			ReadPixel(srcImage, pix, (x-1+width) % width, (y+1) % height, width);
			dY  = ((float) pix[0]) / 255.0f * -1.0f;

			ReadPixel(srcImage, pix,   x   % width, (y+1) % height, width);
			dY += ((float) pix[0]) / 255.0f * -2.0f;

			ReadPixel(srcImage, pix, (x+1) % width, (y+1) % height, width);
			dY += ((float) pix[0]) / 255.0f * -1.0f;

			ReadPixel(srcImage, pix, (x-1+width) % width, (y-1+height) % height, width);
			dY += ((float) pix[0]) / 255.0f *  1.0f;

			ReadPixel(srcImage, pix,   x   % width, (y-1+height) % height, width);
			dY += ((float) pix[0]) / 255.0f *  2.0f;

			ReadPixel(srcImage, pix, (x+1) % width, (y-1+height) % height, width);
			dY += ((float) pix[0]) / 255.0f *  1.0f;

			// Do X Sobel filter
			ReadPixel(srcImage, pix, (x-1+width) % width, (y-1+height) % height, width);
			dX  = ((float) pix[0]) / 255.0f * -1.0f;

			ReadPixel(srcImage, pix, (x-1+width) % width,   y   % height, width);
			dX += ((float) pix[0]) / 255.0f * -2.0f;

			ReadPixel(srcImage, pix, (x-1+width) % width, (y+1) % height, width);
			dX += ((float) pix[0]) / 255.0f * -1.0f;

			ReadPixel(srcImage, pix, (x+1) % width, (y-1+height) % height, width);
			dX += ((float) pix[0]) / 255.0f *  1.0f;

			ReadPixel(srcImage, pix, (x+1) % width, y % height, width);
			dX += ((float) pix[0]) / 255.0f *  2.0f;

			ReadPixel(srcImage, pix, (x+1) % width, (y+1) % height, width);
			dX += ((float) pix[0]) / 255.0f *  1.0f;


			// Cross Product of components of gradient reduces to
			nX = -dX;
			nY = -dY;
			nZ = 1;

			// Normalize
			oolen = 1.0f/((float) sqrt(nX*nX + nY*nY + nZ*nZ));
			nX *= oolen;
			nY *= oolen;
			nZ *= oolen;

			pix[0] = (GLubyte) ((nX+1.0f) / 2.0f * 255.0f);
			pix[1] = (GLubyte) ((nY+1.0f) / 2.0f * 255.0f);
			pix[2] = (GLubyte) ((nZ+1.0f) / 2.0f * 255.0f);

			WritePixel(dstImage, pix, x, y, width);
		}
	}

	GLuint normalMapId = 0;
	/* generate texture */
	glGenTextures (1, &normalMapId);
	glBindTexture (GL_TEXTURE_2D, normalMapId);

	/* setup some parameters for texture filters and mipmapping */
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);/*GL_CLAMP, GL_REPEAT*/
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexImage2D (GL_TEXTURE_2D, level, internalFormat,
		width, height, border, format,
		GL_UNSIGNED_BYTE, dstImage );

	gluBuild2DMipmaps (GL_TEXTURE_2D, internalFormat,
		width, height,
		format, GL_UNSIGNED_BYTE, dstImage);	

	delete[] srcImage;
	delete[] dstImage;

	return normalMapId;
} 




/** \brief Method that creates a mesh
 *
 *   Creates an instance of a desired mesh type
 */
void CMyRenderer::CreateScene()
{
	Load3ds modelLoader;
	
	//this->mesh = modelLoader.Create("3ds/plane.3ds");
	this->mesh = modelLoader.CreateUsingATI("3ds/teapot1.3ds");
	this->wall = modelLoader.CreateUsingATI("3ds/wall.3ds");

	//this->mesh->createInverseTBNMatrices();

	// Set the TBN matrix attributes to the meshes
	this->mesh->setBinormalAttributeObject( binormalAttributeObject );
	this->mesh->setTangentAttributeObject( tangentAttributeObject );
	this->mesh->setTBNNormalAttributeObject( tbnNormalAttributeObject );

	this->wall->setBinormalAttributeObject( binormalAttributeObject );
	this->wall->setTangentAttributeObject( tangentAttributeObject );
	this->wall->setTBNNormalAttributeObject( tbnNormalAttributeObject );
}


/** \brief Method that calculates iFrames per second
*
*	Method that uses glut to calculate iFrames per second. It update the iFpsCountString every  1000 calculation.
*	This method is to uncertain to do any scientific conclusiions but give you a chanse to evaluate the
*	speed of you code.<br>
*	The more math you implement the slower the iFrame rate should be, but not to slowe.
*
*/
void CMyRenderer::FramesPerSec()
{
	iFrame++;
	iCurrentTime = glutGet(GLUT_ELAPSED_TIME);

	if ( iCurrentTime - iPreviousTime > 1000 )
	{
		//secure sprintf_s: (required by visualstudio 2005)
#ifdef _WIN32
		//sprintf_s(iFpsCountString,"FPS:%4.2f", iFrame*1000.0/( iCurrentTime - iPreviousTime ));
		sprintf_s  (iFpsCountString,"FPS:%4.2f", iFrame*1000.0/( iCurrentTime - iPreviousTime ));
#else
		sprintf(iFpsCountString,"FPS:%4.2f", iFrame*1000.0/( iCurrentTime - iPreviousTime ) );
#endif
		iPreviousTime = iCurrentTime;
		iFrame = 0;
	}

}

/** \brief Method that draws text to the glut window
*
*	This method draws text to the glut window at the postion x = -0.85 and y = -0.95
*	If you want to print you own text, you might want to change the method to something like
*	DrawText(int x, int y, cons char *string)
*
*/
void CMyRenderer::DrawText() const
{
	float x( -0.9f ), y( -0.9f );
	
	//configure the transforms for 2D
	glMatrixMode( GL_PROJECTION );

	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -1.0f, 1.0f, -1.0f, 1.0f);

	glDisable(GL_LIGHTING);
	glDisable( GL_TEXTURE_2D );
	glDisable(GL_DEPTH_TEST);

	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(x, y);

	for (int i = 0, len = static_cast<int>( strlen(iFpsCountString) ); i < len; i++)
	{
		//glutBitmapCharacter(GLUT_BITMAP_9_BY_15, iFpsCountString[i] );
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, iFpsCountString[i] );
	}
	//glFlush();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
}


// ********** GL DRAWING METHODs *******


// *************	RENDERING METHODS *********** /

/** \brief Method that specifies how the screen is rendered
*/

void CMyRenderer::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw Text
	FramesPerSec();
	

	iShaderProgram->useProgram();

	//glPolygonMode ( GL_FRONT, GL_FILL );
	//glPointSize( 2.0f );
	//glDisable(GL_LIGHTING);
	GLfloat diffuse[4] = {0.8f, 0, 0, 1.0};
	GLfloat specular[4] = {1, 1, 1, 1.0};
	GLfloat light_pos[4] = {1.5f, 0.0f, 1.0f, 0};
	GLfloat shininess = 10;

	// since we are scaling (in our particular case, uniform scaling) 
	// the object, the normals need to be rescaled
	//glEnable(GL_RESCALE_NORMAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glPushMatrix();
		glRotatef(angle, 0, 0, 1);
		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	//glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	//glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, heightMapId);

	glPushMatrix();
	{
		glTranslatef(0, 0, -20);
		glRotatef(this->iXRotation, 1, 0, 0);
		glRotatef(this->iYRotation, 0, 1, 0);
		this->mesh->draw();

		drawRoom();
	}
	glPopMatrix();

	//angle += 1.5f;
	if (angle > 360)
	{
		angle -= 360;
	}

	iShaderProgram->disableProgram();
	
	DrawText();
	
	glutSwapBuffers();
}

/**
 * @description draws the cave room
 */
void CMyRenderer::drawRoom()
{
	// back wall
	glPushMatrix();
		glTranslatef(0, 0, -20);
		this->wall->draw();
	glPopMatrix();

	// front wall
	glPushMatrix();
		glTranslatef(0, 0, 20);
		glRotatef(180, 0, 1, 0);
		this->wall->draw();
	glPopMatrix();

	// left wall
	glPushMatrix();
		glTranslatef(-20, 0, 0);
		glRotatef(90, 0, 1, 0);
		this->wall->draw();
	glPopMatrix();

	// right wall
	glPushMatrix();
		glTranslatef(20, 0, 0);
		glRotatef(-90, 0, 1, 0);
		this->wall->draw();
	glPopMatrix();

	// top wall
	glPushMatrix();
		glTranslatef(0, 20, 0);
		glRotatef(90, 1, 0, 0);
		// so, to align the texture
		glRotatef(90, 0, 0, 1);
		this->wall->draw();
	glPopMatrix();

	// bottom wall
	glPushMatrix();
		glTranslatef(0, -20, 0);
		glRotatef(-90, 1, 0, 0);
		// so, to align the texture
		glRotatef(90, 0, 0, 1);
		this->wall->draw();
	glPopMatrix();
}

void CMyRenderer::SetXRotation(float aXRotation)
{
	this->iXRotation = aXRotation;
}

void CMyRenderer::SetYRotation(float aYRotation)
{
	this->iYRotation = aYRotation;
}

float CMyRenderer::GetXRotation()
{
	return this->iXRotation;
}

float CMyRenderer::GetYRotation()
{
	return this->iYRotation;
}

void CMyRenderer::SetOldXRotation(float aXRotation)
{
	this->iOldXRotation = aXRotation;
}

void CMyRenderer::SetOldYRotation(float aYRotation)
{
	this->iOldYRotation = aYRotation;
}

float CMyRenderer::GetOldXRotation()
{
	return this->iOldXRotation;
}

float CMyRenderer::GetOldYRotation()
{
	return this->iOldYRotation;
}

void CMyRenderer::SetXPosition(float aXPosition)
{
	this->iXPosition = aXPosition;
}

void CMyRenderer::SetYPosition(float aYPosition)
{
	this->iYPosition = aYPosition;
}

float CMyRenderer::GetXPosition()
{
	return this->iXPosition;
}

float CMyRenderer::GetYPosition()
{
	return this->iYPosition;
}

void CMyRenderer::SetZoom( float aZoom )
{
	this->iZoom = aZoom;
}

float CMyRenderer::GetScreenHeightInPixels()
{
	return this->iScreenHeight;
}

//-----------------------------
//resize the window
//-----------------------------
void CMyRenderer::ResizeScene(const int aWidth, const int aHeight)
{
	glViewport( 0, 0, aWidth, aHeight );

	// Projection transformation
	glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations
	glLoadIdentity(); // We initialize the projection matrix as identity
	gluPerspective(60.0, (GLfloat)aWidth/(GLfloat)aHeight, 1.5, 500.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	this->iScreenHeight = aHeight;
	// This command redraw the scene (it calls the same routine of glutDisplayFunc)
	//glutPostRedisplay ();
}

//EXTERNAL FUNCTIONS TO USE GLUT CALLBACKS
//-----------------------------------------------------
void RenderSceneWithRenderer()
{
	if (CMyRenderer::iCurrentRenderer != NULL)
		CMyRenderer::iCurrentRenderer->RenderScene();
}

void ResizeSceneWithRenderer( int aWidth, int aHeight )
{
	if (CMyRenderer::iCurrentRenderer != NULL)
		CMyRenderer::iCurrentRenderer->ResizeScene(aWidth, aHeight);
}

void RenderSceneWithRendererTimerFunc(int timeValue)
{
	glutPostRedisplay();
	glutTimerFunc(timeValue, RenderSceneWithRendererTimerFunc, timeValue);
}




