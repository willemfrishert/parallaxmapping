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

	// initialize multitexturing
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, textureMapId);

	glActiveTexture(GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_2D, heightMapId);

	glActiveTexture(GL_TEXTURE2 );
	glBindTexture(GL_TEXTURE_2D, normalMapId);
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

	textureMapId = loadTGATexture("textures/rockwall.tga");
	heightMapId = loadTGATexture("textures/rockwall_height.tga");
	normalMapId = loadTGATexture("textures/rockwallDOT3.tga");

	InitShaders();
}




/** \brief Method that creates a mesh
*
*   Creates an instance of a desired mesh type
*/
void CMyRenderer::CreateScene()
{
	Load3ds modelLoader;
	
	this->mesh = modelLoader.Create("3ds/plane.3ds");

	this->mesh->createInverseTBNMatrices();

	this->mesh->setBinormalAttributeObject( binormalAttributeObject );
	this->mesh->setTangentAttributeObject( tangentAttributeObject );
	this->mesh->setTBNNormalAttributeObject( tbnNormalAttributeObject );
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

	//glRotatef(this->iXRotation, 1, 0, 0);
	//glRotatef(this->iYRotation, 0, 1, 0);

	FramesPerSec();

	DrawText();

	iShaderProgram->useProgram();

	//glPolygonMode ( GL_FRONT, GL_FILL );
	//glPointSize( 2.0f );
	//glDisable(GL_LIGHTING);
	GLfloat diffuse[4] = {0.8f, 0, 0, 1.0};
	GLfloat specular[4] = {1, 1, 1, 1.0};
	GLfloat light_pos[4] = {0.0f, 0.0f, 1.0f, 0};
	GLfloat shininess = 10;

	// since we are scaling (in our particular case, uniform scaling) 
	// the object, the normals need to be rescaled
	//glEnable(GL_RESCALE_NORMAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glPushMatrix();
		//glRotatef(angle, 0, 0, 1);
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
		glTranslatef(0, 0, -30);
		glRotatef(this->iXRotation, 1, 0, 0);
		glRotatef(this->iYRotation, 0, 1, 0);
		//glScalef(0.1f, 0.1f, 0.1f);
		this->mesh->draw();
	}
	glPopMatrix();

	angle += 1.5f;
	if (angle > 360)
	{
		angle -= 360;
	}

	iShaderProgram->disableProgram();

	glutSwapBuffers();
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

//-----------------------------
//resize the window
//-----------------------------
void CMyRenderer::ResizeScene(const int aWidth, const int aHeight)
{
	glViewport( 0, 0, aWidth, aHeight );

	// Projection transformation
	glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations
	glLoadIdentity(); // We initialize the projection matrix as identity
	gluPerspective(60.0, (GLfloat)aWidth/(GLfloat)aHeight, 1.5, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

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




