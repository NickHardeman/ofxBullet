/*
 *  ofxBulletCone.cpp
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/24/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#include "ofxBulletCone.h"
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

//--------------------------------------------------------------
ofxBulletCone::ofxBulletCone() {
	_type = OFX_BULLET_CONE_SHAPE;
}

//--------------------------------------------------------------
ofxBulletCone::~ofxBulletCone() {
	
}

//--------------------------------------------------------------
void ofxBulletCone::init(float $radius, float $height) {
	_shape		= (btCollisionShape*)new btConeShape( $radius, $height );
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletCone::init( btConeShape* $colShape ) {
	_shape		= (btCollisionShape*)$colShape;
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletCone::create( btDiscreteDynamicsWorld* $world, ofVec3f $loc, float $mass, float $radius, float $height ) {
	create($world, ofGetBtTransformFromVec3f( $loc ), $mass, $radius, $height);
}

//--------------------------------------------------------------
void ofxBulletCone::create( btDiscreteDynamicsWorld* $world, ofVec3f $loc, ofQuaternion $rot, float $mass, float $radius, float $height ) {
	btTransform tr	= ofGetBtTransformFromVec3f( $loc );
	tr.setRotation( btQuaternion(btVector3($rot.x(), $rot.y(), $rot.z()), $rot.w()) );
	
	create( $world, tr, $mass, $radius, $height );
}

//--------------------------------------------------------------
void ofxBulletCone::create( btDiscreteDynamicsWorld* $world, btTransform $bt_tr, float $mass, float $radius, float $height ) {
	if(!_bInited || _shape == NULL) {
		ofxBulletBaseShape::create( $world, (btCollisionShape*)new btConeShape($radius, $height ), $bt_tr, $mass );
	} else {
		ofxBulletBaseShape::create( $world, _shape, $bt_tr, $mass );
	}
	setData( new ofxBulletUserData() );
}

//--------------------------------------------------------------
float ofxBulletCone::getHeight() {
	return (float)((btConeShape*)_rigidBody->getCollisionShape())->getHeight();
}

//--------------------------------------------------------------
float ofxBulletCone::getRadius() {
	return (float)((btConeShape*)_rigidBody->getCollisionShape())->getRadius();
}

//--------------------------------------------------------------
int ofxBulletCone::getUpIndex() {
	return ((btConeShape*)_rigidBody->getCollisionShape())->getConeUpIndex();
}

// TODO: VBO drawing, no GLUT //
//--------------------------------------------------------------
void ofxBulletCone::draw() {
	if(!_bCreated || _rigidBody == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBulletCone :: draw : must call create() first and add() after");
		return;
	}
	btScalar	m[16];
	ofGetOpenGLMatrixFromRigidBody( _rigidBody, m );
	glPushMatrix(); 
	glMultMatrixf( m );
	
	glPushMatrix();
	int upIndex		= getUpIndex();
	float radius	= getRadius();
	float height	= getHeight();
	switch (upIndex) {
		case 0:
			glRotatef(90.0, 0.0, 1.0, 0.0);
			break;
		case 1:
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			break;
		case 2:
			break;
		default:
		{
		}
	};
	glTranslatef(0.0, 0.0, -0.5*height);
	glutSolidCone(radius, height, 10, 10);
	
	glPopMatrix();
	glPopMatrix();
}




