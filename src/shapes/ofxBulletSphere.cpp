/*
 *  ofxBulletSphere.cpp
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/18/11.
 *
 */

#include "ofxBulletSphere.h"

//--------------------------------------------------------------
ofxBulletSphere::ofxBulletSphere() : ofxBulletRigidBody() {
	_type = OFX_BULLET_SPHERE_SHAPE;
}

//--------------------------------------------------------------
ofxBulletSphere::~ofxBulletSphere() {
	
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
void ofxBulletSphere::create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc, float a_mass, float a_radius ) {
	btTransform tr=ofGetBtTransformFromVec3f( a_loc );
	create(a_world, tr, a_mass, a_radius);
}

//--------------------------------------------------------------
void ofxBulletSphere::create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc, glm::quat a_rot, float a_mass, float a_radius ) {
	btTransform tr	= ofGetBtTransformFromVec3f( a_loc );
	tr.setRotation( btQuaternion( a_rot.x, a_rot.y, a_rot.z, a_rot.w ));
	
	create( a_world, tr, a_mass, a_radius );
}

//--------------------------------------------------------------
void ofxBulletSphere::create( btDiscreteDynamicsWorld* a_world, btTransform &a_bt_tr, float a_mass, float a_radius ) {
	if(!_bInited || _shape == NULL) {
		ofxBulletRigidBody::create( a_world, (btCollisionShape*)new btSphereShape( a_radius ), a_bt_tr, a_mass );
	} else {
		ofxBulletRigidBody::create( a_world, _shape, a_bt_tr, a_mass );
	}
	createInternalUserData();
}

//--------------------------------------------------------------
void ofxBulletSphere::removeShape() {
    if(_bColShapeCreatedInternally) {
        if(_shape) {
            delete (btSphereShape *)_shape;
            _shape = NULL;
        }
    }
}

//--------------------------------------------------------------
void ofxBulletSphere::draw() {
	if(!_bCreated || _rigidBody == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBulletSphere :: draw : must call create() first and add() after");
		return;
	}
	transformGL();
    ofDrawSphere( 0, 0, 0, getRadius() );
    restoreTransformGL();
}

//--------------------------------------------------------------
float ofxBulletSphere::getRadius() {
	return ((btSphereShape*)_rigidBody->getCollisionShape())->getRadius();
}



