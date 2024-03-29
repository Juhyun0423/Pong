#include "LUtil.cpp"


void runMainLoop( int val );


int main( int argc, char* args[] )
{
    std::cout<<"Press q to end game"<<std::endl;
	//Initialize FreeGLUT
	glutInit( &argc, args );

	//Create Double Buffered Window
	glutInitDisplayMode( GLUT_DOUBLE );
	glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	glutCreateWindow( "Pong" );

	//Do post window/context creation initialization
	if( !initGL() )
	{
		printf( "Unable to initialize graphics library!\n" );
		return 1;
	}

    //Set keyboard handler
    glutKeyboardFunc( handleKeys );

	//Set rendering function
	glutDisplayFunc( render );

	//Set main loop
	glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, 0 );

	//Start GLUT main loop
	glutMainLoop();

	return 0;
}

void runMainLoop( int val )
{
    //Frame logic
    update(1);
    render();

    //Run frame one more time
    glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, val );
}
