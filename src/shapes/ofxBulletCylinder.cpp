/*
 *  ofxBulletCylinder.cpp
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/23/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#include "ofxBulletCylinder.h"

//--------------------------------------------------------------
ofxBulletCylinder::ofxBulletCylinder() {
	_type = OFX_BULLET_CYLINDER_SHAPE;
}

//--------------------------------------------------------------
ofxBulletCylinder::~ofxBulletCylinder() {
	
}

//--------------------------------------------------------------
void ofxBulletCylinder::init(float $radius, float $height) {
	_shape		= (btCollisionShape*)ofBtGetCylinderCollisionShape($radius, $height);
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletCylinder::init( btCylinderShape* $colShape ) {
	_shape		= (btCollisionShape*)$colShape;
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletCylinder::create( btDiscreteDynamicsWorld* $world, ofVec3f $loc, float $mass, float $radius, float $height ) {
	create($world, ofGetBtTransformFromVec3f( $loc ), $mass, $radius, $height);
}

//--------------------------------------------------------------
void ofxBulletCylinder::create( btDiscreteDynamicsWorld* $world, ofVec3f $loc, ofQuaternion $rot, float $mass, float $radius, float $height ) {
	btTransform tr	= ofGetBtTransformFromVec3f( $loc );
	tr.setRotation( btQuaternion(btVector3($rot.x(), $rot.y(), $rot.z()), $rot.w()) );
	
	create( $world, tr, $mass, $radius, $height );
}

//--------------------------------------------------------------
void ofxBulletCylinder::create( btDiscreteDynamicsWorld* $world, btTransform $bt_tr, float $mass, float $radius, float $height) {
	if(!_bInited || _shape == NULL) {
		ofxBulletBaseShape::create( $world, (btCollisionShape*)ofBtGetCylinderCollisionShape($radius, $height), $bt_tr, $mass );
	} else {
		ofxBulletBaseShape::create( $world, _shape, $bt_tr, $mass );
	}
	setData( new ofxBulletUserData() );
}

//--------------------------------------------------------------
float ofxBulletCylinder::getRadius() {
	return ((btCylinderShape*)_rigidBody->getCollisionShape())->getRadius();
}

//--------------------------------------------------------------
float ofxBulletCylinder::getHeight() {
	return ((btCylinderShape*)_rigidBody->getCollisionShape())->getHalfExtentsWithMargin().getY()*2.f; 
}


//--------------------------------------------------------------
void ofxBulletCylinder::draw() {
	if(!_bCreated || _rigidBody == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBulletCylinder :: draw : must call create() first and add() after");
		return;
	}
	int upAxis			= ((btCylinderShape*)_rigidBody->getCollisionShape())->getUpAxis();
	float halfHeight	= ((btCylinderShape*)_rigidBody->getCollisionShape())->getHalfExtentsWithMargin()[upAxis];
	
	btScalar	m[16];
	ofGetOpenGLMatrixFromRigidBody( _rigidBody, m );
	glPushMatrix(); 
	glMultMatrixf( m );
	drawCylinder( getRadius(), halfHeight, upAxis );
	glPopMatrix();
}

// from the Bullet examples files //
//--------------------------------------------------------------
void ofxBulletCylinder::drawCylinder(float radius, float halfHeight, int upAxis) {
	glPushMatrix();
	
	switch (upAxis) {
		case 0:
			glRotatef(-90.0, 0.0, 1.0, 0.0);
			glTranslatef(0.0, 0.0, -halfHeight);
			break;
		case 1:
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.0, -halfHeight);
			break;
		case 2:
			glTranslatef(0.0, 0.0, -halfHeight);
			break;
		default: {
			btAssert(0);
		}
			
	}
	
	GLUquadricObj *quadObj = gluNewQuadric();
	
	//The gluCylinder subroutine draws a cylinder that is oriented along the z axis. 
	//The base of the cylinder is placed at z = 0; the top of the cylinder is placed at z=height. 
	//Like a sphere, the cylinder is subdivided around the z axis into slices and along the z axis into stacks.
	
	gluQuadricDrawStyle(quadObj, (GLenum)GLU_FILL);
	gluQuadricNormals(quadObj, (GLenum)GLU_SMOOTH);
	
	gluDisk(quadObj,0,radius,15, 10);
	//glutCylinder(const bool solid=true, const GLdouble radius=1.0, const GLdouble height=1.0, const GLint slices=15, const GLint stacks=10, const GLint loops=5)
	
	gluCylinder(quadObj, radius, radius, 2.f*halfHeight, 15, 10);
	glTranslatef(0.0f, 0.0f, 2.f*halfHeight);
	glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);
	gluDisk(quadObj,0.f,radius,15, 10);
	
	glPopMatrix();
	gluDeleteQuadric(quadObj);
}




