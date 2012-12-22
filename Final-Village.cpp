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
Angel::mat4 projmat;  //projection matrix

void m_glewInitAndVersion(void) {
	fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
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

vec4 eye = vec4( 0, 10, 20, 1 );
vec4 at = vec4( 0, 0, 0, 1 );

GLuint carPro;
void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glDepthMask( GL_TRUE );

	glUseProgram( carPro );
	glUniformMatrix4fv(glGetUniformLocation( carPro, "Projection" ), 1, GL_TRUE, projmat);  //pass to shader
	glUniformMatrix4fv(glGetUniformLocation( carPro, "ModelView" ), 1, GL_TRUE, modelViewStack.top());
	
	glPolygonMode(GL_FRONT, GL_FILL);
	traverse( &nodes[Dude] );  //begin traverse the tree
	glutSwapBuffers();
}


void initNodes(){
	carPro = InitShader( "carPro.v", "carPro.f" );
	GLubyte *dudeTex = NULL; 

	Model *dude = new Model("data/al.obj", 
		&modelViewStack, 
		vec4( 0.0, 0.0, 0.0, 0 ), vec3(0,-30,0), vec3(1.0, 1.0, 1.0),
		carPro, false, dudeTex);
	nodes[Dude] = Node(dude, NULL, NULL);
}


void init()
{	

	Angel::mat4 modelView = Angel::identity_mat();  //tmp
	projmat = Angel::Perspective( 60.0, 1, 1, 200 ); 
	//modelView = modelView * LookAt( eye, at, vec4(0,1,0,1) );
	modelViewStack.push( modelView );  //stack

	glClearColor(0.5, 0.8, 0.9, 1.0);
	glClearDepth( 1.0f );
	glShadeModel( GL_FLAT );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
	
	initNodes();
	//loadTexture();
	//loadModel();
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
	glutDisplayFunc( display );
	//glutIdleFunc(idle);
	//glutKeyboardFunc( keyBoard );
	//glutSpecialFunc( funcKey );

	glutMainLoop();
	return 0;
}