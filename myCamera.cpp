#include "stdafx.h"
#include "myCamera.h"
#include <math.h>
//#include <iostream>

using namespace std;

myCamera::myCamera(void) {
	scaleVector = 0.5;  
	scaleAngle = 0.05;
	cosine = cos( scaleAngle );
	sine = sin( scaleAngle );

	n = Angel::normalize(vec4(0.0, 0.0, 10.0, 1.0));
	v = Angel::vec4(0.0, 1.0, 0.0, 1.0);  
	u = Angel::normalize(cross(v, n)); 

	distant = 2;
	eye = distant*n;
	//eye += Angel::vec4( 1, 1, 0, 0 );  //correct the bug version mat.h
}

myCamera::myCamera( vec4 &nn, vec4 &vv, int dis ) {
	scaleVector = 0.5;  
	scaleAngle = 0.05;
	cosine = cos( scaleAngle );
	sine = sin( scaleAngle );

	n = Angel::normalize( nn );
	v = Angel::normalize( vv );  
	u = Angel::normalize(cross(vv, nn)); 

	distant = dis;
	eye = distant*n;
	//eye += Angel::vec4( 1, 1, 0, 0 );  //correct the bug version mat.h
}

myCamera::~myCamera(void) {
}

void myCamera::moveForward() {
	eye -= scaleVector * n;
	eye.w = 1.0;
}

void myCamera::moveBackward() {
	eye += scaleVector * n;
	eye.w = 1.0;
}

void myCamera::moveLeft() {
	eye -= scaleVector * u;
	eye.w = 1.0;
}

void myCamera::moveRight() {
	eye += scaleVector * u;
	eye.w = 1.0;
}

void myCamera::pitchUp() {
	Angel::vec4 tmp0;
	tmp0 = v;  tmp0[3] = 0;
	v = cosine*tmp0 + sine*n; 
	n = -sine*tmp0 + cosine*n;
	v = Angel::normalize( v );
	n = Angel::normalize( n );
	//cout << n << endl;
}

void myCamera::pitchDown() {
	Angel::vec4 tmp0;
	tmp0 = v;  tmp0[3] = 0;
	v = cosine*tmp0 - sine*n;
	n = sine*tmp0 + cosine*n;
	v = Angel::normalize( v );
	n = Angel::normalize( n );
	//cout << n << endl;
}

void myCamera::yawLeft() {
	Angel::vec4 tmp0;
	tmp0 = u;  tmp0[3] = 0;
	u = cosine*tmp0 - sine*n;
	n = sine*tmp0 + cosine*n;
	u = Angel::normalize( u );
	n = Angel::normalize( n );
	//cout << n << endl;
}

void myCamera::yawLeft( float angle ) {
	Angel::vec4 tmp0;
	tmp0 = u;  tmp0[3] = 0;
	u = cos( angle/180*PI )*tmp0 - sin( angle/180*PI )*n;
	n = sin( angle/180*PI )*tmp0 + cos( angle/180*PI )*n;
	u = Angel::normalize( u );
	n = Angel::normalize( n );
}

void myCamera::yawRight() {
	Angel::vec4 tmp0, tmp1;
	tmp0 = u;  tmp0[3] = 0;
	u = cosine*tmp0 + sine*n;
	n = -sine*tmp0 + cosine*n;
	u = Angel::normalize( u );
	n = Angel::normalize( n );
	//cout << n << endl;
}

void myCamera::yawRight( float angle ) {
	Angel::vec4 tmp0, tmp1;
	tmp0 = u;  tmp0[3] = 0;
	u = cos( angle/180*PI )*tmp0 + sin( angle/180*PI )*n;
	n = -sin( angle/180*PI )*tmp0 + cos( angle/180*PI )*n;
	u = Angel::normalize( u );
	n = Angel::normalize( n );
}

void myCamera::rollLeft() {
	Angel::vec4 tmp0, tmp1;
	tmp0 = v;  tmp0[3] = 0;
	tmp1 = u;	tmp1[3] = 0;
	u = cosine*tmp1 - sine*tmp0;
	v = sine*tmp1 + cosine*tmp0;
	u = Angel::normalize( u );
	v = Angel::normalize( v );
}

void myCamera::rollRight() {
	Angel::vec4 tmp0, tmp1;
	tmp0 = v;  tmp0[3] = 0;
	tmp1 = u;	tmp1[3] = 0;
	u = cosine*tmp1 + sine*tmp0;
	v = -sine*tmp1 + cosine*tmp0;
	u = Angel::normalize( u );
	v = Angel::normalize( v );
}

void myCamera::setEye( Angel::vec4 &e ) {
	eye = e;
}

Angel::mat4 myCamera::getLookAt() {
	return Angel::LookAt(Angel::vec4( eye.x, eye.y, eye.z, 1 ), Angel::vec4( (eye-n).x, (eye-n).y, (eye-n).z, 1 ), v);
}

Angel::vec4 myCamera::getEye() {
	//cout << "Eye: " << eye << endl;
	return Angel::vec4( eye.x, eye.y, eye.z, 1 );
}

Angel::vec4 myCamera::getAt() {
	//cout << "At: " << eye-n << endl;
	return Angel::vec4( (eye-n).x, (eye-n).y, (eye-n).z, 1 );
}
