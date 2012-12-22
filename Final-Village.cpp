// Final-Village.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Models.h"



enum {
	Dude = 0,
	NumNodes,
	Quit
};


stack<Angel::mat4> modelViewStack; 
Node  nodes[NumNodes];


void init()
{	

	glClearColor(0.5, 0.8, 0.9, 1.0);
	glClearDepth( 1.0f );
	glShadeModel( GL_FLAT );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );

	//buildTree();
	//loadTexture();
	//loadModel();
}

void
	traverse( Node* node )
{
	if ( node == NULL ) { return; }

	modelViewStack.push( modelViewStack.top() );

	node->_model->render();

	if ( node->_child ) { traverse( node->_child ); }

	modelViewStack.pop();

	if ( node->_sibling ) { traverse( node->_sibling ); }
}

void initNodes(){
	GLuint carPro = InitShader( "carPro.v", "carPro.f" );
	GLubyte *dudeTex; 
	Model *dude = new Model("data/al.obj", 
					   &modelViewStack, 
					   vec4( 6.0, -0.8, -8.0, 0 ), vec3(0,-30,0), vec3(1.0, 1.0, 1.0),
					   carPro, false, dudeTex);
	nodes[Dude] = Node(dude, NULL, NULL);
}

void m_glewInitAndVersion(void) {
	fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( SCREENWIDTH, SCREENHEIGHT );
	glutInitWindowPosition( 0, 0 );
	glutCreateWindow( "Village" );
	m_glewInitAndVersion();
	init();

	//glutReshapeFunc( reshape );
	//glutDisplayFunc( display );
	//glutIdleFunc(idle);
	//glutKeyboardFunc( keyBoard );
	//glutSpecialFunc( funcKey );

	glutMainLoop();
	return 0;
}