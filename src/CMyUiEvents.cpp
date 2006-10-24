
//INCLUDE FILES
#include "CMyUiEvents.h"

//CONSTANTS
#ifdef USE_RADIANS
const float KAngleChangeAmount = 0.001f;
#elif defined USE_DEGREES
const float KAngleChangeAmount = 0.05f;
#endif

//INIT STATIC DATA
CMyUiEvents* CMyUiEvents::iCurrentUi = 0;

//Constructor, saves a pointer to the renderer
CMyUiEvents::CMyUiEvents( CMyRenderer* aRenderer )
{
	iRenderer = aRenderer;
	CMyUiEvents::iCurrentUi = this;
}

//Destructor
CMyUiEvents::~CMyUiEvents()
{
}

//Used to handle the keyboard input (ASCII Characters)
void CMyUiEvents::ProcessNormalKeys(unsigned char key, int x, int y)
{

}

// Used to handle the keyboard input (not ASCII Characters)
void CMyUiEvents::ProcessCursorKeys(int key, int x, int y)
{
	switch (key)
	{
		//MODIFY X
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
		//MODIFY Y
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
		//MODIFY Z
	case GLUT_KEY_PAGE_UP:
		break;
	case GLUT_KEY_PAGE_DOWN:
		break;
		//RESET Base angle as well
	case GLUT_KEY_HOME:
		break;
		//FREEZE
	default:
		break;
	}
}

void CMyUiEvents::ProcessMouseEvent( int button, int state, int x, int y )
{
	switch (button)
	{
		//Set point of rotation
	case GLUT_LEFT_BUTTON:
		if(GLUT_DOWN == state)
		{
			iMouseButtonDown = EMouseDownLeft;
			iMouseY = y;
		}
		else
		{
			iMouseButtonDown = EMouseUp;
		}
		break;
		//Set mesh position
	case GLUT_RIGHT_BUTTON:
		if(GLUT_UP == state)
		{
			iMouseButtonDown = EMouseDownRight;
			//Wait until change is ok
			//while(iRenderer->TransformInProcess());
			//TVector3 mp = iRenderer->MeshPosition();
			//iRenderer->SetMeshPosition( x, y, mp.Z() );
		}
		else
		{
			iMouseButtonDown = EMouseUp;
		}
		break;
	default:
		break;
	}

}

void CMyUiEvents::ProcessMouseMotionEvent( int x, int y )
{
	if( EMouseDownLeft == iMouseButtonDown)
	{
		iMouseY = y;
	}
}






//EXTERNAL FUNCTIONS TO USE GLUT CALLBACKS
void ProcessNormalKeysWithUi( unsigned char key, int x, int y )
{
	if (CMyUiEvents::iCurrentUi != 0 )
		CMyUiEvents::iCurrentUi->ProcessNormalKeys( key, x, y  );
}

void ProcessCursorKeysWithUi( int key, int x, int y )
{
	if (CMyUiEvents::iCurrentUi != 0 )
		CMyUiEvents::iCurrentUi->ProcessCursorKeys( key, x, y );
}
void ProcessMouseEventWithUi( int button, int state, int x, int y)
{
	if (CMyUiEvents::iCurrentUi != 0 )
		CMyUiEvents::iCurrentUi->ProcessMouseEvent( button, state, x, y );
}

void ProcessMouseMotionEventWithUi( int x, int y )
{
	if (CMyUiEvents::iCurrentUi != 0 )
		CMyUiEvents::iCurrentUi->ProcessMouseMotionEvent( x, y );
}

