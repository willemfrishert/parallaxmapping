//INCLUDES
#include "CMyRenderer.h"
#include "TVertex.h"
#include "3ds.h"
#include "tga.h"
#include "BumpMapping.h"
#include "TextureMapping.h"

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
, iXRotation( 0 )
, iYRotation( 0 )
, iOldXRotation( 0 )
, iOldYRotation( 0 )
, iXPosition( 0 )
, iYPosition( 0 )
, iZoom( -25 )
, iFreezeLights( false )
, iMappingIndex( 1 )
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
	iParallaxMapping = new ParallaxMapping();

	iMappings[ 0 ] = iParallaxMapping;
	iMappings[ 1 ] = new BumpMapping();
	//iMappings[ 2 ] = new TextureMapping();
}

void CMyRenderer::LoadTextures()
{
	// ##################### ROCK WALL ###########################
	rockwallTextures[0] = loadTGATexture("textures/rockwall_rot.tga");
	rockwallTextures[1] = loadTGATexture("textures/rockwall_height_rot.tga");
	
	// generate normal map using ATI's TGAtoDOT3 functions
	rockwallTextures[2] = GenerateDOT3( rockwallTextures[1] );

	// ##################### ROCKS ############################
	rocksTextures[0] = loadTGATexture("textures/rocks.tga");
	rocksTextures[1] = loadTGATexture("textures/rocks_height.tga");

	// generate normal map using ATI's TGAtoDOT3 functions
	rocksTextures[2] = GenerateDOT3( rocksTextures[1] );

	// ##################### RED BRICKS ############################
	redbricksTextures[0] = loadTGATexture("textures/redbricks.tga");
	redbricksTextures[1] = loadTGATexture("textures/redbricks_height.tga");

	// generate normal map using ATI's TGAtoDOT3 functions
	redbricksTextures[2] = GenerateDOT3( redbricksTextures[1] );

	// ##################### SPRITES ############################
	sprite[0] = loadTGATexture("textures/particlered.tga");
	sprite[1] = loadTGATexture("textures/particleblue.tga");
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

	// Load all the necessary textures
	LoadTextures();

	InitShaders();

	InitLights();
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
	
	this->teapot = modelLoader.CreateUsingATI("3ds/teapot.3ds");
	this->wall = modelLoader.CreateUsingATI("3ds/wall.3ds");
	this->column = modelLoader.CreateUsingATI("3ds/column.3ds");

	//this->mesh->createInverseTBNMatrices();
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
	static GLfloat quadratic[3] = { 1.0f, 0.0f, 0.0f };
	glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, quadratic );

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

void CMyRenderer::ComputeLightPosition( float* position, float radius, float alpha, 
									   float cosineFreq, float height )
{
	alpha = (alpha * (2 * 3.1459)) / 360.0f;
	position[0] = radius * sinf( alpha );
	position[1] = height * cosf( cosineFreq * alpha );
	position[2] = radius * cosf( alpha );
	position[3] = 1.0f;
}

void CMyRenderer::InitLights()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	diffuse0[0] = 1;
	diffuse0[1] = 1;
	diffuse0[2] = 1;
	diffuse0[3] = 1.0f;

	diffuse1[0] = 0.5f;
	diffuse1[1] = 0.5f;
	diffuse1[2] = 0.2f;
	diffuse1[3] = 1.0f;

	diffuse2[0] = 0.6f;
	diffuse2[1] = 0.6f;
	diffuse2[2] = 1.0f;
	diffuse2[3] = 1.0f;

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
}

void CMyRenderer::ComputeLightsPositions()
{
	static float alpha1 = 0.0f;
	static float alpha2 = 240.0f;

	//ComputeLightPosition(light0Position, 10.0f, alpha0, 0.0f, 0.0f);
	light0Position[0] = 0.0;
	light0Position[1] = 7.0;
	light0Position[2] = 7.0;
	light0Position[3] = 1.0f;

	ComputeLightPosition(light1Position, 9.0f, alpha1, 2.0f, 3.0f);
	ComputeLightPosition(light2Position, 18.0f, alpha2, 2.0f, 10.0f);

	/* freeze or not to freeze.... :P */
	if ( ! iFreezeLights )
	{
		alpha1 -= 2.0f;
		alpha2 += 2.0f;

		if ( alpha1 < 0.0f)
		{
			alpha1 += 360.0f;
		}

		if ( alpha2 > 360.0f)
		{
			alpha2 -= 360.0f;
		}
	}
}

void CMyRenderer::drawPointSprite( GLuint spriteId, GLfloat spriteSize )
{
	static GLfloat quadratic[3] = { 0.0f, 0.0f, 0.005f };
	glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, quadratic );

	glDisable(GL_LIGHTING);
	glActiveTexture( GL_TEXTURE1 );
	glDisable(GL_TEXTURE_2D);
	glActiveTexture( GL_TEXTURE2 );
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0 );
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spriteId);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor3f(1, 1, 1);

	glPointSize( spriteSize );
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

	glEnable(GL_POINT_SPRITE);
	glBegin(GL_POINTS);
		glVertex3f(0, 0, 0);
	glEnd();
	glDisable(GL_POINT_SPRITE);
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
}

/** 
 *\brief Method that specifies how the screen is rendered
 */
void CMyRenderer::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw Text
	FramesPerSec();

	glPushMatrix();
	{
		glTranslatef(0, 0, iZoom);
		
		// Head Light
		glPushMatrix();
			glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
		glPopMatrix();
		
		glRotatef(this->iXRotation, 1, 0, 0);
		glRotatef(this->iYRotation, 0, 1, 0);

		ComputeLightsPositions();

		/************************************************************************/
		/* LIGHTS !!!!                                                          */
		/************************************************************************/
		glPushMatrix();
			glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
		glPopMatrix();
		
		glPushMatrix();
			glLightfv(GL_LIGHT2, GL_POSITION, light2Position);
		glPopMatrix();

		//iParallaxMapping->enable();
		//iBumpMapping->enable();
		iMappings[ iMappingIndex ]->enable();

		glEnable(GL_TEXTURE_2D);

		// Change texture and draw the teapot
		iParallaxMapping->setBiasValue( 0.2f );
		//iParallaxMapping->setup(this->teapot, rocksTextures);
		iMappings[ iMappingIndex ]->setup(this->teapot, rocksTextures);
		this->teapot->draw();
		
		// Change texture and draw the room walls
		iParallaxMapping->setBiasValue( 0.05f );
		//iParallaxMapping->setup(this->wall, rockwallTextures);
		iMappings[ iMappingIndex ]->setup(this->wall, rockwallTextures);
		drawRoom();
		
		// Change texture and draw the column
		iParallaxMapping->setBiasValue( 0.06f );
		//iParallaxMapping->setup(this->column, redbricksTextures);
		iMappings[ iMappingIndex ]->setup(this->column, redbricksTextures);
		this->column->draw();

		//iParallaxMapping->disable();
		//iBumpMapping->disable();
		iMappings[ iMappingIndex ]->disable();

		glPushMatrix();
			glTranslatef(light1Position[0], light1Position[1], light1Position[2]);
			drawPointSprite( sprite[0] ,32.0f );
		glPopMatrix();
		glPushMatrix();
			glTranslatef(light2Position[0], light2Position[1], light2Position[2]);
			drawPointSprite( sprite[1], 16.0f );
		glPopMatrix();
	}
	glPopMatrix();
	
	DrawText();
	
	glutSwapBuffers();
}

/**
 * @description draws the cave room
 */
void CMyRenderer::drawRoom()
{
	float delta = 19.9f;

	// back wall
	glPushMatrix();
		glTranslatef(0, 0, -delta);
		this->wall->draw();
	glPopMatrix();

	// front wall
	glPushMatrix();
		glTranslatef(0, 0, delta);
		glRotatef(180, 0, 1, 0);
		this->wall->draw();
	glPopMatrix();

	// left wall
	glPushMatrix();
		glTranslatef(-delta, 0, 0);
		glRotatef(90, 0, 1, 0);
		this->wall->draw();
	glPopMatrix();

	// right wall
	glPushMatrix();
		glTranslatef(delta, 0, 0);
		glRotatef(-90, 0, 1, 0);
		this->wall->draw();
	glPopMatrix();

	// top wall
	glPushMatrix();
		glTranslatef(0, delta, 0);
		glRotatef(90, 1, 0, 0);
		// so, to align the texture
		glRotatef(90, 0, 0, 1);
		this->wall->draw();
	glPopMatrix();

	// bottom wall
	glPushMatrix();
		glTranslatef(0, -delta, 0);
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
	if ( (aZoom > -33.0f ) && ( aZoom < -15.0f ) )
	{
		this->iZoom = aZoom;
	}
	
}
float CMyRenderer::GetZoom()
{
	return this->iZoom;
}

float CMyRenderer::GetScreenHeightInPixels()
{
	return this->iScreenHeight;
}

/**
 */
void CMyRenderer::ToggleMapping()
{
	iMappingIndex = (iMappingIndex + 1) % 2;
}

/**
 */
void CMyRenderer::ToggleLightMovement()
{
	this->iFreezeLights = ! this->iFreezeLights;
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




