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
, textureId(0)
, bumpMapId(0)
, tangentAttributeObject(0)
, binormaltAttributeObject(NULL)
, textureUniformObject(NULL)
, bumpMapUniformObject(NULL)	
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
	iVertexShader = new ShaderObject(GL_VERTEX_SHADER, "shader/bumpmap.vert");
	iFragmentShader = new ShaderObject(GL_FRAGMENT_SHADER, "shader/bumpmap.frag");

	iShaderProgram = new ShaderProgram();
	
	iShaderProgram->attachShader( *iVertexShader);
	iShaderProgram->attachShader( *iFragmentShader );

	tangentAttributeObject = new ShaderAttributeObject("tangent");
	binormaltAttributeObject = new ShaderAttributeObject("binormal");

	textureUniformObject = new ShaderUniformValue<int>();
	textureUniformObject->setName("decalTex");
	textureUniformObject->setValue( 0 );
	
	bumpMapUniformObject = new ShaderUniformValue<int>();
	bumpMapUniformObject->setName("bumpTex");
	bumpMapUniformObject->setValue( 1 );

	iShaderProgram->addAttributeObject( tangentAttributeObject );
	iShaderProgram->addAttributeObject( binormaltAttributeObject );
	iShaderProgram->addUniformObject( textureUniformObject );
	iShaderProgram->addUniformObject( bumpMapUniformObject );

	iShaderProgram->buildProgram();

	// initialize multitexturing
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, textureId);

	glActiveTexture(GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_2D, bumpMapId);
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

	textureId = loadTGATexture("textures/bricks.tga");
	bumpMapId = loadTGATexture("textures/bricks_bump.tga");

	InitShaders();
}




/** \brief Method that creates a mesh
*
*   Creates an instance of a desired mesh type
*/
void CMyRenderer::CreateScene()
{
	Load3ds modelLoader;
	
	this->mesh = modelLoader.Create("3ds/torus.3ds");

	this->mesh->createInverseTBNMatrices();

	this->mesh->setBinormalAttributeObject( binormaltAttributeObject );
	this->mesh->setTangentAttributeObject( tangentAttributeObject );
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

	FramesPerSec();

	DrawText();

	iShaderProgram->useProgram();

	//glPolygonMode ( GL_FRONT, GL_FILL );
	//glPointSize( 2.0f );
	//glDisable(GL_LIGHTING);
	GLfloat diffuse[4] = {0.8f, 0, 0, 1.0};
	GLfloat specular[4] = {1, 1, 1, 1.0};
	GLfloat light_pos[4] = {0, 0, 1, 0};
	GLfloat shininess = 10;

	// since we are scaling (in our particular case, uniform scaling) 
	// the object, the normals need to be rescaled
	glEnable(GL_RESCALE_NORMAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	//glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	//glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, bumpMapId);

	glPushMatrix();
	glTranslatef(0, 0, -15);
	//glRotatef(angle, 0, 1, 0);
	glScalef(0.1f, 0.1f, 0.1f);
	this->mesh->draw();
	glPopMatrix();

	angle += 0.5f;
	if (angle > 360)
	{
		angle -= 360;
	}

	iShaderProgram->disableProgram();

	glutSwapBuffers();
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




