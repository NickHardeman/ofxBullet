/*
 *  ofxBulletCylinder.cpp
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/23/11.
 *
 */

#include "ofxBulletCylinder.h"

//--------------------------------------------------------------
ofxBulletCylinder::ofxBulletCylinder() : ofxBulletRigidBody() {
	_type = OFX_BULLET_CYLINDER_SHAPE;
}

//--------------------------------------------------------------
ofxBulletCylinder::~ofxBulletCylinder() {
	
}

//--------------------------------------------------------------
void ofxBulletCylinder::init(float a_radius, float a_height) {
	_shape		= (btCollisionShape*)ofBtGetCylinderCollisionShape(a_radius, a_height);
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletCylinder::init( btCylinderShape* a_colShape ) {
	_shape		= (btCollisionShape*)a_colShape;
	_bInited	= true;
    // shape passed in externally, so not responsible for deleteing pointer
    _bColShapeCreatedInternally = false;
}

//--------------------------------------------------------------
void ofxBulletCylinder::create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc, float a_mass, float a_radius, float a_height ) {
	btTransform tr=ofGetBtTransformFromVec3f( a_loc );
	create(a_world, tr, a_mass, a_radius, a_height);
}

//--------------------------------------------------------------
void ofxBulletCylinder::create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc, glm::quat a_rot, float a_mass, float a_radius, float a_height ) {
	btTransform tr	= ofGetBtTransformFromVec3f( a_loc );
	tr.setRotation( btQuaternion(a_rot.x, a_rot.y, a_rot.z, a_rot.w ));
	
	create( a_world, tr, a_mass, a_radius, a_height );
}

//--------------------------------------------------------------
void ofxBulletCylinder::create( btDiscreteDynamicsWorld* a_world, btTransform &a_bt_tr, float a_mass, float a_radius, float a_height) {
	if(!_bInited || _shape == NULL) {
		ofxBulletRigidBody::create( a_world, (btCollisionShape*)ofBtGetCylinderCollisionShape(a_radius, a_height), a_bt_tr, a_mass );
	} else {
		ofxBulletRigidBody::create( a_world, _shape, a_bt_tr, a_mass );
	}
	createInternalUserData();
}

//--------------------------------------------------------------
void ofxBulletCylinder::removeShape() {
    if(_bColShapeCreatedInternally) {
        if(_shape) {
            delete (btCylinderShape *)_shape;
            _shape = NULL;
        }
    }
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
//	int upAxis			= ((btCylinderShape*)_rigidBody->getCollisionShape())->getUpAxis();
//	float halfHeight	= ((btCylinderShape*)_rigidBody->getCollisionShape())->getHalfExtentsWithMargin()[upAxis];
	
	transformGL();
    ofDrawCylinder( 0, 0, 0, getRadius(), getHeight() );
    restoreTransformGL();
}





