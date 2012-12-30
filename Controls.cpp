#include "stdafx.h"
#include "Models.h"
#include "Controls.h"
#include "myCamera.h"

extern int switchCam;
extern Model *car ;
extern vec4 carPosition; 
extern vec4 carForward ; // the minimum step of the car's move
extern float carTurn; // the minimum angle of car's turn
extern float carRotation ; // store the car's rotation
extern myCamera camera; // initialize the camera
extern bool switcher;

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
		case 't':
			if(switcher == false)
				switcher = true;
			else
				switcher = false;
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