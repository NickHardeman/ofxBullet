//
//  ofxBulletRigidBody.cpp
//  ofxBullet
//
//  Created by Elias Zananiri on 2014-11-12.
//
//

#include "ofxBulletRigidBody.h"

//--------------------------------------------------------------
ofxBulletRigidBody::ofxBulletRigidBody() : ofxBulletBaseShape() {
	_world			= NULL;
	_rigidBody		= NULL;
	_shape			= NULL;
	_bColShapeCreatedInternally = true;
}

//--------------------------------------------------------------
ofxBulletRigidBody::~ofxBulletRigidBody() {
	// Call remove() explicitly so that the version in this subclass gets called.
	remove();
}

// you can call this function directly if you wish to use the same collision object for multiple rigid bodies,
// which will increase performance //
//--------------------------------------------------------------
void ofxBulletRigidBody::create( btDiscreteDynamicsWorld* a_world, btCollisionShape* a_colShape, btTransform &a_bt_tr, float a_mass ) {
	if(a_world == NULL) {
		ofLog(OF_LOG_ERROR, "ofxBulletSphere :: create : a_world param is NULL");
		return;
	}
	_mass			= a_mass;
	_world			= a_world;
    
    _shape          = a_colShape;
	
	_rigidBody		= ofGetBtRigidBodyFromCollisionShape( a_colShape, a_bt_tr, a_mass);
	setCreated(_rigidBody);
	
	setProperties(.4, .75);
	setDamping( .25 );
}

//--------------------------------------------------------------
void ofxBulletRigidBody::add() {
	_bAdded = true;
	_world->addRigidBody( _rigidBody );
}

//--------------------------------------------------------------
void ofxBulletRigidBody::add(short group, short mask) {
    _bAdded = true;
    _world->addRigidBody( _rigidBody, group, mask );
}

//--------------------------------------------------------------
void ofxBulletRigidBody::remove() {
	setData(NULL);
	removeRigidBody();
    removeShape();
	setRemoved();
}

//--------------------------------------------------------------
void ofxBulletRigidBody::removeShape() {
    if(_bColShapeCreatedInternally) {
        if(_shape) {
            delete _shape;
            _shape = NULL;
        }
    }
	_bInited = false;
}

//--------------------------------------------------------------
void ofxBulletRigidBody::removeRigidBody() {
	if(_rigidBody != NULL) {
		if(_world != NULL && _bAdded) {
			_world->removeRigidBody(_rigidBody);
			_bAdded = false;
		}
		if (_rigidBody->getMotionState()) {
			delete _rigidBody->getMotionState();
			_rigidBody->setMotionState(NULL);
		}

		delete _rigidBody;
		_rigidBody = NULL;
	}
}

// GETTERS //

//--------------------------------------------------------------
btRigidBody* ofxBulletRigidBody::getRigidBody() {
	return _rigidBody;
}

//--------------------------------------------------------------
btCollisionShape* ofxBulletRigidBody::getCollisionShape() const {
	return _shape;
}

//--------------------------------------------------------------
bool ofxBulletRigidBody::isCollisionShapeInternal() {
    return _bColShapeCreatedInternally;
}

//--------------------------------------------------------------
float ofxBulletRigidBody::getMass() const {
	return _mass;
}

//--------------------------------------------------------------
void ofxBulletRigidBody::getOpenGLMatrix( btScalar* a_m ) {
	ofGetOpenGLMatrixFromRigidBody( _rigidBody, a_m );
}

//--------------------------------------------------------------
float ofxBulletRigidBody::getDamping() const {
	return (float)_rigidBody->getLinearDamping();
}

//--------------------------------------------------------------
float ofxBulletRigidBody::getAngularDamping() const {
	return (float)_rigidBody->getAngularDamping();
}

// SETTERS, may be called after create() //

//--------------------------------------------------------------
void ofxBulletRigidBody::setDamping( float a_linear_damp ) {
	setDamping( a_linear_damp, getAngularDamping() );
}

//--------------------------------------------------------------
void ofxBulletRigidBody::setAngularDamping( float a_angular_damp ) {
	setDamping( getDamping(), a_angular_damp );
}

//--------------------------------------------------------------
void ofxBulletRigidBody::setDamping( float a_linear_damp, float a_angular_damp ) {
	_rigidBody->setDamping(a_linear_damp, a_angular_damp);
}

// FORCES //

//--------------------------------------------------------------
void ofxBulletRigidBody::applyForce( const glm::vec3& a_frc, const glm::vec3& a_rel_pos ) {
	_rigidBody->applyForce( btVector3(a_frc.x, a_frc.y, a_frc.z), btVector3(a_rel_pos.x, a_rel_pos.y, a_rel_pos.z) );
}

//--------------------------------------------------------------
void ofxBulletRigidBody::applyForce( const btVector3& a_frc, const btVector3& a_rel_pos ) {
	_rigidBody->applyForce( a_frc, a_rel_pos );
}

//--------------------------------------------------------------
void ofxBulletRigidBody::applyCentralForce( const glm::vec3& a_frc ) {
	applyCentralForce( btVector3(a_frc.x, a_frc.y, a_frc.z) );
}
void ofxBulletRigidBody::applyCentralForce( float a_x, float a_y, float a_z ) {
	applyCentralForce( btVector3(a_x, a_y, a_z) );
}
void ofxBulletRigidBody::applyCentralForce( const btVector3& a_frc ) {
	_rigidBody->applyCentralForce( a_frc );
}

//--------------------------------------------------------------
void ofxBulletRigidBody::applyTorque( const glm::vec3& a_torque ) {
	applyTorque( btVector3(a_torque.x, a_torque.y, a_torque.z) );
}
void ofxBulletRigidBody::applyTorque( float a_x, float a_y, float a_z ) {
	applyTorque( btVector3( a_x, a_y, a_z ) );
}
void ofxBulletRigidBody::applyTorque( const btVector3& a_torque ) {
	_rigidBody->applyTorque( a_torque );
}

//--------------------------------------------------------------
void ofxBulletRigidBody::transformGL() {
    btScalar	ATTRIBUTE_ALIGNED16(m[16]);
	getOpenGLMatrix(m);
    ofPushMatrix();
    ofMultMatrix( ofMatrix4x4(m) );
}
