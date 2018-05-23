/*
 *  ofxBulletCone.cpp
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/24/11.
 *
 */

#include "ofxBulletCone.h"

//--------------------------------------------------------------
ofxBulletCone::ofxBulletCone() : ofxBulletRigidBody() {
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
    // shape passed in externally, so not responsible for deleteing pointer
    _bColShapeCreatedInternally = false;
}

//--------------------------------------------------------------
void ofxBulletCone::create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc, float a_mass, float a_radius, float a_height ) {
	btTransform tr=ofGetBtTransformFromVec3f( a_loc );
	create(a_world, tr, a_mass, a_radius, a_height);
}

//--------------------------------------------------------------
void ofxBulletCone::create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc, glm::quat a_rot, float a_mass, float a_radius, float a_height ) {
	btTransform tr	= ofGetBtTransformFromVec3f( a_loc );
	tr.setRotation( btQuaternion( a_rot.x, a_rot.y, a_rot.z, a_rot.w ));

	create( a_world, tr, a_mass, a_radius, a_height );
}

//--------------------------------------------------------------
void ofxBulletCone::create( btDiscreteDynamicsWorld* a_world, btTransform &a_bt_tr, float a_mass, float a_radius, float a_height ) {
	if(!_bInited || _shape == NULL) {
		ofxBulletRigidBody::create( a_world, (btCollisionShape*)new btConeShape(a_radius, a_height ), a_bt_tr, a_mass );
	} else {
		ofxBulletRigidBody::create( a_world, _shape, a_bt_tr, a_mass );
	}
	createInternalUserData();
}

//--------------------------------------------------------------
void ofxBulletCone::removeShape() {
    if(_bColShapeCreatedInternally) {
        if(_shape) {
            delete (btConeShape *)_shape;
            _shape = NULL;
        }
    }
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

//--------------------------------------------------------------
void ofxBulletCone::draw() {
	if(!_bCreated || _rigidBody == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBulletCone :: draw : must call create() first and add() after");
		return;
	}
	transformGL();
    ofPushMatrix();
    ofRotateDeg(180, 1, 0, 0);
    ofDrawCone( 0, 0, 0, getRadius(), getHeight() );
    ofPopMatrix();
    restoreTransformGL();
}




