// Final-Village.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Models.h"
#include "myCamera.h"
#include "Controls.h"
#include "Skybox.h"
#include "SphereModel.h"
#include "IndoorModel.h"

// The enumerations to make a clear tree.
enum {
	Sky = 0,
	Terrain = 1,
	Car = 2,
	Grass = 3,
	Earth = 4,
	Dude = 5,
	Room = 6,
	Bed = 7,
	Rose = 8,
	Dragon,
	ReflectBall,
	NumNodes,
};

// Several Global variables
stack<Angel::mat4> modelViewStack; // the Model view stack
Node  nodes[NumNodes]; // The tree struct
Angel::mat4 projmat;  //projection matrix
GLuint glutPro, arrayPro, skyPro, carPro, cubeMapPro; // Several shader programs 
myCamera camera( vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), 50 ); // initialize the camera
int switchCam = 2; // Camera state.
bool switcher = true;// the switcher in room.
Model *room, *bed, *rose, *dragon, *reflectBall; // the indoor model.
bool indoorFlag = false; //Use to define the camera in the room.
GLfloat roomY = -30.0;

// Car Related Parameters and Variables.
// I need put the car as global variables so that it can drive with the camera
Model *car; // Car Model
Angel::vec4 carPosition(270.0, 0.0, 440.0,1.0);  //the terrain's high is 0.0
Angel::vec4 carForward(0.0,0.0,-1.0,0.0) ; // the minimum step of the car's move
float carTurn = 2.0; // the minimum angle of car's turn
float carRotation = 0.0; // store the car's rotation

// Functions Declaration 
void cam(); // Camera function, press 'V' to switch different Viewpoints.


void m_glewInitAndVersion(void) {
	fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}


void initNodes(){

	// Initialize the skybox
	Model *skybox = new Skybox("data/sphere.obj", "data/textures/sky.ppm", 512, 512, // Object, textures 
		&modelViewStack, // Model view stack
		skyPro, (GLfloat)10.0 ,&camera); // shader program, rotate value and camera object

	// Initialize the Terrain.
	Model *terrain = new Model("data/HOUSTON.obj", 
		&modelViewStack, arrayPro,
		vec4( 0.0, 83.0, 0.0, 0 ), vec3(-90,0,0), vec3(1000.0, 1000.0, 1000.0));

	// Initialize the Grass Field.
	Model *grass = new Model("data/center.obj", 
		&modelViewStack, arrayPro,
		vec4( 0.0, -3.0, 0.0, 0 ), vec3(0,0,0), vec3(1000.0, 1.0, 1000.0),
		true, "data/textures/grass.ppm", 64, 64);

	// Initialize the Car.
	car = new Model("data/sr5.obj", 
		&modelViewStack, 
		carPosition, vec3(-90.0, -180.0 + carRotation, 0.0), vec3(2.0, 2.0, 2.0),
		carPro, true, 21);

	// Initialize the Dude.
	Model *dude = new Model("data/al.obj", 
		&modelViewStack, carPro,
		vec4( 275.0, 0, 350.0, 0 ), vec3(0,-1,0), vec3(1.0, 1.0, 1.0));

	//Initialize the Earth.
	Model *earth = new SphereModel("data/textures/Earth_Front.ppm", "data/textures/Earth_Back.ppm",
								   "data/textures/Earth_Left.ppm", "data/textures/Earth_Right.ppm",
								   "data/textures/Earth_Upper.ppm", "data/textures/Earth_Below.ppm",
								   512,512,
								   &modelViewStack, cubeMapPro, 1,
								   true, false,
								   vec4( 270.0, 3.0, 330.0, 0.0 ), vec3(0,0,200), 5.0);

	//Initialize the Room.
	room = new IndoorModel("data/cube.obj", 
						   &modelViewStack, arrayPro,
						   vec4( 295.0, roomY + 8, 350.0, 0 ), vec3(90,0,0), vec3(15.0, 15.0, 10.0),
						   true, "data/textures/wall.ppm", 64, 64 );
	
	//Initialize the Bed.
	bed = new IndoorModel("data/MIDEVIL.obj", 
						  &modelViewStack, carPro,
						  vec4( 302.0, roomY + 3, 347.0, 0 ), vec3(-90,0,0), vec3(10.0, 10.0, 10.0),
						  true, "data/textures/wall.ppm", 64, 64 );
	
	//Initialize the Rose.
	rose = new IndoorModel("data/rose+vase.obj", 
		&modelViewStack, carPro,
		vec4( 300.0, roomY +  1, 343.0, 0 ), vec3(0,0,0), vec3(2.0, 2.0, 2.0),
		true, "data/textures/wall.ppm", 64, 64 );
	
	//Initialize the Dragon.
	dragon = new IndoorModel("data/dragon.obj", 
		&modelViewStack, carPro,
		vec4( 300.0, roomY - 1, 354.0, 0 ), vec3(-90,90,0), vec3(3.0, 3.0, 3.0),
		true, "data/textures/wall.ppm", 64, 64 );
	
	//Initialize the Ball.
	reflectBall = new SphereModel("data/textures/front.ppm", "data/textures/back.ppm",
							      "data/textures/left.ppm", "data/textures/right.ppm",
								  "data/textures/top.ppm", "data/textures/bottom.ppm",
								  512,512,
								  &modelViewStack, cubeMapPro, 2,
								  false, true,
								  vec4( 284.0, roomY + 1.0, 339.0, 0.0 ), vec3(0,-45,180), 3.0);

	// Build the tree.
	nodes[Sky] = Node(skybox, &nodes[Terrain], NULL);
	nodes[Terrain] = Node(terrain, &nodes[Grass], NULL);
	nodes[Grass] = Node(grass, &nodes[Dude], NULL);
	nodes[Dude] = Node(dude, &nodes[Earth], NULL);
	nodes[Earth] = Node(earth, &nodes[Room], NULL);
	nodes[Room] =  Node(room, NULL, &nodes[Bed]);
	nodes[Bed] =  Node(bed, NULL, &nodes[Dragon]);
	nodes[Dragon] =  Node(dragon, &nodes[Rose], NULL );
	nodes[Rose] =  Node(rose, NULL, &nodes[ReflectBall]);
	nodes[ReflectBall] =  Node(reflectBall, &nodes[Car], NULL);
	// Always put the car in last position, 
	// for rendering the transparent object successfully.
	nodes[Car] = Node(car, NULL, NULL); 
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
	room->updateSwitcher(switcher);
	bed->updateSwitcher(switcher);
	dragon->updateSwitcher(switcher);
	rose->updateSwitcher(switcher);
	reflectBall->updateSwitcher(switcher);
	if(camera.getEye().x > 265 && camera.getEye().x < 275 &&
	   camera.getEye().y > -2 && camera.getEye().y < 8 &&
	   camera.getEye().z > 325 && camera.getEye().z < 335){
			switchCam = 0;
			camera.setEye(vec4( 285.0, roomY, 365.0, 1 ));
			indoorFlag = true;
	};

	if( indoorFlag == true &&
		camera.getEye().x > 280 && camera.getEye().x < 310 &&
		camera.getEye().y > roomY - 2 && camera.getEye().y < roomY + 18 &&
		camera.getEye().z > 335 && camera.getEye().z < 366){
	}else if (indoorFlag == true){
			camera.setEye(vec4( 270.0, 0, 340.0, 1 ));
			indoorFlag = false;
	};

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




void reshape( int width, int height ) {
	glViewport( 0, 0, (GLsizei) width, (GLsizei) height );
	projmat = Angel::Perspective( 45.0, width/height, 1, 500 ); 
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
	glutIdleFunc(idle);
	glutKeyboardFunc( keyBoard );
	glutSpecialFunc( funcKey );

	glutMainLoop();
	return 0;
}