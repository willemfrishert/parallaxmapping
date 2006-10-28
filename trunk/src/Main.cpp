#include "CMyRenderer.h"

#include "CMyUiEvents.h"
//#include "TImageHandler.h"

//CONSTANTS
const int KWindowPositionX = 50;
const int KWindowPositionY = 50;
const int KWindowWidth = 800;
const int KWindowHeight = 600;

class A
{
public:
	int s;
};

void initContext(int argc, char **argv)
{

	glutInit( &argc, argv );
	
	//Init Window
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition( KWindowPositionX, KWindowPositionY );
	glutInitWindowSize( KWindowWidth, KWindowHeight ); //800,600);
	glutCreateWindow("Parallax Mapping");

	//Set some Glut functions
	glutKeyboardFunc( ProcessNormalKeysWithUi );
	glutSpecialFunc(  ProcessCursorKeysWithUi );
	glutMouseFunc(    ProcessMouseEventWithUi );
	glutMotionFunc(   ProcessMouseMotionEventWithUi );

	glutDisplayFunc(  RenderSceneWithRenderer );
	//glutIdleFunc(     RenderSceneWithRenderer );
	glutReshapeFunc(  ResizeSceneWithRenderer );
	glutTimerFunc(20, RenderSceneWithRendererTimerFunc, 20);

#ifdef _WIN32
	GLenum err = glewInit();
#endif	
}


/**
* Main method.
* This method is called first an it will do the following:
*  -# Creates the renderer and ui objects
*  -# Initializes Glut
*  -# Calls renderer->CreateMesh to create a CMesh
*  -# Starts the glutMainLoop()
* @param argc amount of arguments for glutInit().
* @param argv arguments for glutInit().
* @return 0
*/
int main(int argc, char **argv)
{
	//Initialize GL using GLUT functions
	initContext(argc, argv);

	//Init renderer
	CMyRenderer* renderer = new CMyRenderer();
	CMyUiEvents* ui = new CMyUiEvents( renderer );

	renderer->CreateScene();

	//glutFullScreen();

	//START GL loop
	glutMainLoop();

	delete ui;
	delete renderer;
	return 0;
}
