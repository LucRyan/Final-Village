// Final-Village.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Models.h"
#include "myCamera.h"

#include "Skybox.h"

// The enumerations to make a clear tree.
enum {
	Sky = 0,
	Terrain = 1,
	Car = 2,
	Grass = 3,
	Dude = 4,
	NumNodes,
	Quit
};

// Several Global variables
stack<Angel::mat4> modelViewStack; // the Model view stack
Node  nodes[NumNodes]; // The tree struct
Angel::mat4 projmat;  //projection matrix
GLuint glutPro, arrayPro, skyPro, carPro, cubeMapPro; // Several shader programs 
myCamera camera( vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), 50 ); // initialize the camera
int switchCam = 2;


// Car Related Parameters and Variables.
// I need put the car as global variables so that it can drive with the camera
Model *car ;
Angel::vec4 carPosition(300.0, 0.0, 400.0,1.0);  //the terrain's high is -2.0
Angel::vec4 carForward(0.0,0.0,-1.0,0.0) ; // the minimum step of the car's move
float carTurn = 2.0; // the minimum angle of car's turn
float carRotation = 0.0; // store the car's rotation

// Functions Declaration 
void cam();


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


void idle() {
	//skyRotate += 0.05;
	glutPostRedisplay();
}

void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glDepthMask( GL_TRUE );

	glUseProgram( glutPro );
	glUniformMatrix4fv(glGetUniformLocation( glutPro, "Projection" ), 1, GL_TRUE, projmat);  //pass to shader
	glUniformMatrix4fv(glGetUniformLocation( glutPro, "ModelView" ), 1, GL_TRUE, modelViewStack.top());

	glUseProgram( arrayPro );
	glUniformMatrix4fv(glGetUniformLocation( arrayPro, "Projection" ), 1, GL_TRUE, projmat);  //pass to shader
	glUniformMatrix4fv(glGetUniformLocation( arrayPro, "ModelView" ), 1, GL_TRUE, modelViewStack.top());

	glUseProgram( skyPro );
	glUniformMatrix4fv(glGetUniformLocation( skyPro, "Projection" ), 1, GL_TRUE, projmat);  //pass to shader
	glUniformMatrix4fv(glGetUniformLocation( skyPro, "ModelView" ), 1, GL_TRUE, modelViewStack.top());

	glUseProgram( cubeMapPro );
	glUniformMatrix4fv(glGetUniformLocation( cubeMapPro, "Projection" ), 1, GL_TRUE, projmat);  //pass to shader
	glUniformMatrix4fv(glGetUniformLocation( cubeMapPro, "ModelView" ), 1, GL_TRUE, modelViewStack.top());

	glUseProgram( carPro );
	glUniformMatrix4fv(glGetUniformLocation( carPro, "Projection" ), 1, GL_TRUE, projmat);  //pass to shader
	glUniformMatrix4fv(glGetUniformLocation( carPro, "ModelView" ), 1, GL_TRUE, modelViewStack.top());

	glPolygonMode(GL_FRONT, GL_FILL);
	
	cam();
	traverse( &nodes[Sky] );  //begin traverse the tree
	glutSwapBuffers();
}


void initNodes(){
	GLubyte *noneTex =  glmReadPPM("data/textures/grass.ppm", 64, 64);

	Model *skybox = new Skybox("data/sphere.obj", "data/textures/sky.ppm", 512, 512,
		&modelViewStack,
		skyPro, (GLuint)10 ,&camera);

	Model *terrain = new Model("data/HOUSTON.obj", 
		&modelViewStack, 
		vec4( 0.0, 83.0, 0.0, 0 ), vec3(-90,0,0), vec3(1000.0, 1000.0, 1000.0),
		arrayPro, false, noneTex);

	Model *grass = new Model("data/center.obj", 
		&modelViewStack, 
		vec4( 0.0, -3.0, 0.0, 0 ), vec3(0,0,0), vec3(1000.0, 1.0, 1000.0),
		arrayPro, true, noneTex);

	car = new Model("data/sr5.obj", 
		&modelViewStack, 
		carPosition, vec3(-90.0, -180.0 + carRotation, 0.0), vec3(2.0, 2.0, 2.0),
		carPro, true, 21);

	Model *dude = new Model("data/al.obj", 
		&modelViewStack, 
		vec4( 300.0, 1.0, 380.0, 0 ), vec3(0,0,0), vec3(2.0, 2.0, 2.0),
		carPro, false, noneTex);

	nodes[Sky] = Node(skybox, &nodes[Terrain], NULL);
	nodes[Terrain] = Node(terrain, &nodes[Grass], NULL);
	nodes[Grass] = Node(grass, &nodes[Dude], NULL);
	nodes[Dude] = Node(dude, &nodes[Car], NULL);
	nodes[Car] = Node(car, NULL, NULL);
}

void cam() {

	if ( switchCam == 1 ) {
		camera.setEye(Angel::vec4(carPosition.x + sin(carRotation/180*PI)* 0.5, 
			carPosition.y + 0.6, carPosition.z + cos(carRotation/180*PI)* 0.5, 1.0 ));
	}
	if ( switchCam == 2 ) {
		camera.setEye(Angel::vec4(carPosition.x + sin(carRotation/180*PI)*5.0,
			carPosition.y + 1.0, carPosition.z + cos(carRotation/180*PI)*5.0, 1.0 ));
	}
	modelViewStack.top() = camera.getLookAt();
}


void init()
{	
	glutPro = InitShader( "glutPro.v", "glutPro.f" );
	arrayPro = InitShader( "arrayPro.v", "arrayPro.f" );
	skyPro = InitShader( "skyPro.v", "skyPro.f" );
	carPro = InitShader( "carPro.v", "carPro.f" );
	cubeMapPro = InitShader( "cubeMap.v", "cubeMap.f" );

	Angel::mat4 modelView = Angel::identity_mat();  //tmp
	projmat = Angel::Perspective( 45.0, 1, 1, 500 ); 
	modelViewStack.push( modelView );  //stack

	glClearColor(0.5, 0.8, 0.9, 1.0);
	glClearDepth( 1.0f );
	glShadeModel( GL_FLAT );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );

	initNodes();
}


void reshape( int width, int height ) {
	glViewport( 0, 0, (GLsizei) width, (GLsizei) height );
	projmat = Angel::Perspective( 45.0, width/height, 1, 500 ); 
}


void funcKey( int key, int x, int y ) {
   if( switchCam != 0 ) {
		switch( key ) {
		case GLUT_KEY_UP: // move forward		
			carPosition += carForward;
			car->setTraslate(carPosition);
			break;
		case GLUT_KEY_DOWN: // move backward
			carPosition -= carForward;
			car->setTraslate(carPosition);
			break;
		case GLUT_KEY_LEFT:
			carRotation += carTurn;
			camera.yawLeft( carTurn );
			carForward = Angel::vec4(-sin(carRotation/180*PI), 0, -cos(carRotation/180*PI), 0);
			car->setRotate(vec3(-90.0, -180.0 + carRotation, 0.0));
			break;
		case GLUT_KEY_RIGHT:
			carRotation -= carTurn;
			camera.yawRight( carTurn );
			carForward = Angel::vec4(-sin(carRotation/180*PI), 0, -cos(carRotation/180*PI), 0);
			car->setRotate(vec3(-90.0, -180.0 + carRotation, 0.0));
			break;
		}
	}else {
	   switch( key ) {
	   default:
		   break;
	   }
   }
	glutPostRedisplay();
}

void keyBoard( unsigned char key, int x, int y ) {
	if ( switchCam == 0 ) {  //fly Cam
		switch(key) {
		case 'w': // MOVE Forward
			camera.moveForward();
			break;
		case 's': // MOVE Backward
			camera.moveBackward();
			break;
		case 'a': // MOVE Left
			camera.moveLeft();
			break;
		case 'd': // MOVE Right
			camera.moveRight();
			break;
		case 'q': // Turn Left
			camera.yawLeft();
			break;
		case 'e': // Turn Right
			camera.yawRight();
			break;
		case 'x': // pitch up
			camera.pitchUp();
			break;
		case 'X': // pitch down
			camera.pitchDown();
			break;
		case 'c': // yaw counter-clockwise in the un plane
			camera.rollRight();
			break;
		case 'C': // yaw clockwise in the un plane
			camera.rollLeft();
			break;
		case 033:  // Escape key
			exit( EXIT_SUCCESS );
		default:
			break;
		}
	} else {
		switch(key) {
		case 'w': // move forward		
			carPosition += carForward;
			car->setTraslate(carPosition);
			break;
		case 's': // move backward
			carPosition -= carForward;
			car->setTraslate(carPosition);
			break;
		case 'a':
			carRotation += carTurn;
			camera.yawLeft( carTurn );
			carForward = Angel::vec4(-sin(carRotation/180*PI), 0, -cos(carRotation/180*PI), 0);
			car->setRotate(vec3(-90.0, -180.0 + carRotation, 0.0));
			break;
		case 'd':
			carRotation -= carTurn;
			camera.yawRight( carTurn );
			carForward = Angel::vec4(-sin(carRotation/180*PI), 0, -cos(carRotation/180*PI), 0);
			car->setRotate(vec3(-90.0, -180.0 + carRotation, 0.0));
			break;
		case 'v': // switch cam
		case 'V':  // inCar/followCar
			if ( switchCam == 1)
				switchCam = 2;
			else
				switchCam = 1;
			break;
		case 'f':
		case 'F':
			if ( switchCam == 0)
				switchCam = 2;
			else
				switchCam = 0;
			break;
		case 033:  // Escape key
		case 'q': case 'Q':
			exit( EXIT_SUCCESS );
		}
	}
	glutPostRedisplay();
}


int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( SCREENWIDTH, SCREENHEIGHT );
	glutInitWindowPosition( 300, 0 );
	glutCreateWindow( "Village" );
	m_glewInitAndVersion();
	init();

	glutReshapeFunc( reshape );
	glutDisplayFunc( display );
	//glutIdleFunc(idle);
	glutKeyboardFunc( keyBoard );
	glutSpecialFunc( funcKey );

	glutMainLoop();
	return 0;
}