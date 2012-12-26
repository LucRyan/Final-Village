// Final-Village.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Models.h"
#include "myCamera.h"

#include "Skybox.h"

enum {
	Sky = 0,
	Dude = 1,
	NumNodes,
	Quit
};

stack<Angel::mat4> modelViewStack; 
Node  nodes[NumNodes];
Angel::mat4 projmat;  //projection matrix
myCamera camera( Angel::vec4(0.0, 0.0, 1.0, 0.0), Angel::vec4(0.0, 1.0, 0.0, 0.0), 50 );
GLuint glutPro, arrayPro, skyPro, carPro, cubeMapPro; 
GLMmodel *carMod, *dudeMod;
Angel::vec4 carPosition(0.0, 0.0,50.0,1.0);  //the terrain's high is -2.0
Angel::vec4 carForward(0.0,0.0,-1.0,0.0) ;
float carRotation = 0.0;
float carTurn = 3.0;

GLuint buffer;
GLuint vao;
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

extern point4 distant_light_position;
extern color4 distant_light_ambient;
extern color4 distant_light_diffuse;
extern color4 distant_light_specular;

void cam();
void car();


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

	//glUseProgram( arrayPro );
	//glUniformMatrix4fv(glGetUniformLocation( arrayPro, "Projection" ), 1, GL_TRUE, projmat);  //pass to shader
	//glUniformMatrix4fv(glGetUniformLocation( arrayPro, "ModelView" ), 1, GL_TRUE, modelViewStack.top());

	glUseProgram( skyPro );
	glUniformMatrix4fv(glGetUniformLocation( skyPro, "Projection" ), 1, GL_TRUE, projmat);  //pass to shader
	glUniformMatrix4fv(glGetUniformLocation( skyPro, "ModelView" ), 1, GL_TRUE, modelViewStack.top());

	//glUseProgram( cubeMapPro );
	//glUniformMatrix4fv(glGetUniformLocation( cubeMapPro, "Projection" ), 1, GL_TRUE, projmat);  //pass to shader
	//glUniformMatrix4fv(glGetUniformLocation( cubeMapPro, "ModelView" ), 1, GL_TRUE, modelViewStack.top());

	glUseProgram( carPro );
	glUniformMatrix4fv(glGetUniformLocation( carPro, "Projection" ), 1, GL_TRUE, projmat);  //pass to shader
	glUniformMatrix4fv(glGetUniformLocation( carPro, "ModelView" ), 1, GL_TRUE, modelViewStack.top());

	glPolygonMode(GL_FRONT, GL_FILL);
	
	cam();
	traverse( &nodes[Sky] );  //begin traverse the tree
	car();
	glutSwapBuffers();
}


void initNodes(){

	Model *skybox = new Skybox("data/sphere.obj", "data/textures/sky.ppm", 512, 512,
		&modelViewStack,
		skyPro, (GLuint)10 ,&camera);

	GLubyte *dudeTex = NULL; 
	Model *dude = new Model("data/al.obj", 
		&modelViewStack, 
		vec4( 0.0, 1.0, 0.0, 0 ), vec3(0,-30,0), vec3(5.0, 5.0, 5.0),
		carPro, false, dudeTex);
	nodes[Sky] = Node(skybox, &nodes[Dude], NULL);
	nodes[Dude] = Node(dude, NULL, NULL);
}

void cam() {

	camera.setEye(Angel::vec4(carPosition.x + sin(carRotation/180*PI)*0.4, 
			carPosition.y + 0.8, carPosition.z + cos(carRotation/180*PI)*0.4, 1.0 ));

	modelViewStack.top() = camera.getLookAt();
	Print(modelViewStack.top());
}


void init()
{	
	glutPro = InitShader( "glutPro.v", "glutPro.f" );
	arrayPro = InitShader( "arrayPro.v", "arrayPro.f" );
	skyPro = InitShader( "skyPro.v", "skyPro.f" );
	carPro = InitShader( "carPro.v", "carPro.f" );
	cubeMapPro = InitShader( "cubeMap.v", "cubeMap.f" );

	Angel::mat4 modelView = Angel::identity_mat();  //tmp
	projmat = Angel::Perspective( 60.0, 1, 1, 200 ); 
	modelViewStack.push( modelView );  //stack

	glClearColor(0.5, 0.8, 0.9, 1.0);
	glClearDepth( 1.0f );
	glShadeModel( GL_FLAT );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );

	carMod=glmReadOBJ("data/porsche.obj");
	glmUnitize(carMod);
	glmFacetNormals(carMod);
	glmVertexNormals(carMod, 90.0);
	glmLinearTexture(carMod);
	glmLoadGroupsInVBO(carMod);

	initNodes();
	//loadModel();
}


void reshape( int width, int height ) {
	glViewport( 0, 0, (GLsizei) width, (GLsizei) height );
}


void funcKey( int key, int x, int y ) {
/*	if( switchCam == 0 ) */
		switch( key ) {
		case GLUT_KEY_UP: // move forward		
			carPosition += carForward;
			break;
		case GLUT_KEY_DOWN: // move backward
			carPosition -= carForward;
			break;
		case GLUT_KEY_LEFT:
			carRotation += carTurn;
			camera.yawLeft( carTurn );
			carForward = Angel::vec4(-sin(carRotation/180*PI), 0, -cos(carRotation/180*PI), 0);
			break;
		case GLUT_KEY_RIGHT:
			carRotation -= carTurn;
			camera.yawRight( carTurn );
			carForward = Angel::vec4(-sin(carRotation/180*PI), 0, -cos(carRotation/180*PI), 0);
			break;
		}
	glutPostRedisplay();
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

	glutReshapeFunc( reshape );
	glutDisplayFunc( display );
	glutIdleFunc(idle);
	//glutKeyboardFunc( keyBoard );
	glutSpecialFunc( funcKey );

	glutMainLoop();
	return 0;
}

void car() {
	glUseProgram( carPro );

	GLMgroup* group = carMod->groups;
	GLMmaterial* material;

	for ( int i = 0; i < carMod->numgroups; ++i ) {
		glBindVertexArray( carMod->vao[i] );
		glBindBuffer( GL_ARRAY_BUFFER, carMod->vbo[i] );
		GLuint vPosition = glGetAttribLocation( carPro, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0) );
		GLuint vNormal = glGetAttribLocation( carPro, "vNormal" );
		glEnableVertexAttribArray( vNormal );
		glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(GLfloat)*4*group->numtriangles*3) );

		material = &carMod->materials[group->material];
		Angel::vec4 ambient( material->ambient[0], material->ambient[1], material->ambient[2], material->ambient[3]);
		Angel::vec4 diffuse( material->diffuse[0], material->diffuse[1], material->diffuse[2], material->diffuse[3]);
		Angel::vec4 specular( material->specular[0], material->specular[1], material->specular[2], material->specular[3]);
		glUniform4fv( glGetUniformLocation(carPro, "AmbientProduct"), 1, 
			distant_light_ambient * ambient );
		glUniform4fv( glGetUniformLocation(carPro, "DiffuseProduct"), 1, 
			distant_light_diffuse * diffuse );
		glUniform4fv( glGetUniformLocation(carPro, "SpecularProduct"), 1, 
			distant_light_specular * specular );
		glUniform1f( glGetUniformLocation(carPro, "Shininess"), 
			material->shininess );
		glUniform4fv( glGetUniformLocation(carPro, "LightPosition"), 1, distant_light_position );
		glUniform1f( glGetUniformLocation(carPro, "alpha"), 1.0 ); 

		modelViewStack.push(modelViewStack.top());
		modelViewStack.top() *= Angel::Translate( carPosition );
		modelViewStack.top() *= Angel::RotateY( -180.0 + carRotation );
		modelViewStack.top() *= Angel::Scale( 2.0, 2.0, 2.0 );
		glUniformMatrix4fv( glGetUniformLocation(carPro, "NormalMatrix"), 1, GL_TRUE,
			Angel::RotateY( -180.0 + carRotation )*Angel::Scale( 1.0/2.0, 1.0/2.0, 1.0/2.0 ) );
		glUniformMatrix4fv(glGetUniformLocation( carPro, "ModelView" ), 1, GL_TRUE, modelViewStack.top());
		modelViewStack.pop();
		glDrawArrays( GL_TRIANGLES, 0, group->numtriangles*3 );	
		group = group->next;
	}
}

