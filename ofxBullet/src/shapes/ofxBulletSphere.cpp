/*
 *  ofxBulletSphere.cpp
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/18/11.
 *
 */

#include "ofxBulletSphere.h"

//--------------------------------------------------------------
ofxBulletSphere::ofxBulletSphere() {
	_type = OFX_BULLET_SPHERE_SHAPE;
}

//--------------------------------------------------------------
ofxBulletSphere::~ofxBulletSphere() {
	//destroy();
}

//--------------------------------------------------------------
void ofxBulletSphere::init(float a_radius) {
	_shape		= (btCollisionShape*)new btSphereShape( a_radius );
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletSphere::init( btSphereShape* a_colShape ) {
	_shape		= (btCollisionShape*)a_colShape;
	_bInited	= true;
    // shape passed in externally, so not responsible for deleteing pointer
    _bColShapeCreatedInternally = false;
}

//--------------------------------------------------------------
void ofxBulletSphere::create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, float a_mass, float a_radius ) {
	create(a_world, ofGetBtTransformFromVec3f( a_loc ), a_mass, a_radius);
}

//--------------------------------------------------------------
void ofxBulletSphere::create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, ofQuaternion a_rot, float a_mass, float a_radius ) {
	btTransform tr	= ofGetBtTransformFromVec3f( a_loc );
	tr.setRotation( btQuaternion(btVector3(a_rot.x(), a_rot.y(), a_rot.z()), a_rot.w()) );
	
	create( a_world, tr, a_mass, a_radius );
}

//--------------------------------------------------------------
void ofxBulletSphere::create( btDiscreteDynamicsWorld* a_world, btTransform const&  a_bt_tr, float a_mass, float a_radius ) {
	if(!_bInited || _shape == NULL) {
		ofxBulletBaseShape::create( a_world, (btCollisionShape*)new btSphereShape( a_radius ), a_bt_tr, a_mass );
	} else {
		ofxBulletBaseShape::create( a_world, _shape, a_bt_tr, a_mass );
	}
	createInternalUserData();
}

//--------------------------------------------------------------
void ofxBulletSphere::draw() {
	if(!_bCreated || _rigidBody == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBulletSphere :: draw : must call create() first and add() after");
		return;
	}
	transformGL();
    ofDrawSphere( 0, 0, 0, getRadius() );
    restoreTramsformGL();
}

//--------------------------------------------------------------
float ofxBulletSphere::getRadius() {
	return ((btSphereShape*)_rigidBody->getCollisionShape())->getRadius();
}



