/*
 *  ofxBulletCone.cpp
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/24/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#include "ofxBulletCone.h"
#ifdef TARGET_LINUX
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

//--------------------------------------------------------------
ofxBulletCone::ofxBulletCone() {
	_type = OFX_BULLET_CONE_SHAPE;
}

//--------------------------------------------------------------
ofxBulletCone::~ofxBulletCone() {

}

//--------------------------------------------------------------
void ofxBulletCone::init(float a_radius, float a_height) {
	_shape		= (btCollisionShape*)new btConeShape( a_radius, a_height );
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletCone::init( btConeShape* a_colShape ) {
	_shape		= (btCollisionShape*)a_colShape;
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletCone::create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, float a_mass, float a_radius, float a_height ) {
	create(a_world, ofGetBtTransformFromVec3f( a_loc ), a_mass, a_radius, a_height);
}

//--------------------------------------------------------------
void ofxBulletCone::create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, ofQuaternion a_rot, float a_mass, float a_radius, float a_height ) {
	btTransform tr	= ofGetBtTransformFromVec3f( a_loc );
	tr.setRotation( btQuaternion(btVector3(a_rot.x(), a_rot.y(), a_rot.z()), a_rot.w()) );

	create( a_world, tr, a_mass, a_radius, a_height );
}

//--------------------------------------------------------------
void ofxBulletCone::create( btDiscreteDynamicsWorld* a_world, btTransform a_bt_tr, float a_mass, float a_radius, float a_height ) {
	if(!_bInited || _shape == NULL) {
		ofxBulletBaseShape::create( a_world, (btCollisionShape*)new btConeShape(a_radius, a_height ), a_bt_tr, a_mass );
	} else {
		ofxBulletBaseShape::create( a_world, _shape, a_bt_tr, a_mass );
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




