/*
 *  ofxBulletBaseCollisionShape.cpp
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/18/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#include "ofxBulletBaseShape.h"

//--------------------------------------------------------------
ofxBulletBaseShape::ofxBulletBaseShape() {
	_world			= NULL;
	_rigidBody		= NULL;
	_shape			= NULL;
	_bCreated		= false;
	_bInited		= false;
	_bAdded			= false;
	_userPointer	= NULL;
}

//--------------------------------------------------------------
ofxBulletBaseShape::~ofxBulletBaseShape() {
	remove();
}

// you can call this function directly if you wish to use the same collision object for multiple rigid bodies,
// which will increase performance //
//--------------------------------------------------------------
void ofxBulletBaseShape::create( btDiscreteDynamicsWorld* a_world, btCollisionShape* a_colShape, btTransform a_bt_tr, float a_mass ) {
	if(a_world == NULL) {
		ofLog(OF_LOG_ERROR, "ofxBulletSphere :: create : a_world param is NULL");
		return;
	}
	_mass			= a_mass;
	_world			= a_world;
	
	_bCreated		= true;
	
	_rigidBody		= ofGetBtRigidBodyFromCollisionShape( a_colShape, a_bt_tr, a_mass);
	setProperties(.4, .75);
	setDamping( .25 );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::add() {
	_bAdded = true;
	_world->addRigidBody( _rigidBody );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::remove() {
	if(_userPointer != NULL) {
		delete _userPointer;
		_userPointer = NULL;
	}
	removeRigidBody();
}

//--------------------------------------------------------------
void ofxBulletBaseShape::removeRigidBody() {
	if(_world != NULL && _bAdded) {
		//cout << "ofxBulletBaseShape :: removeRigidBody : calling remove rigid body" << endl;
		if (_rigidBody && _rigidBody->getMotionState()) {
			delete _rigidBody->getMotionState();
		}
		_world->removeRigidBody(_rigidBody);
		delete _rigidBody;
		_rigidBody = NULL;
		
	}
	_bCreated = _bInited = _bAdded = false;
}

//--------------------------------------------------------------
bool ofxBulletBaseShape::operator==( const void* a_userData) const {
	return getData() == a_userData;
}

//--------------------------------------------------------------
bool ofxBulletBaseShape::operator==( const ofxBulletCollisionData& a_collisionData) const {
	return (getData() == a_collisionData.userData1) || (getData() == a_collisionData.userData2);
}

//--------------------------------------------------------------
bool ofxBulletBaseShape::operator!=( const ofxBulletCollisionData& a_collisionData) const {
	return (getData() != a_collisionData.userData1) && (getData() != a_collisionData.userData2);
}

//--------------------------------------------------------------
bool ofxBulletBaseShape::operator==( const ofxBulletMousePickEvent& a_e ) const {
	return getData() == a_e.userData;
}
//--------------------------------------------------------------
bool ofxBulletBaseShape::operator!=( const ofxBulletMousePickEvent& a_e ) const {
	return !(getData() == a_e.userData);
}

//--------------------------------------------------------------
bool ofxBulletBaseShape::operator==( const ofxBulletRaycastData& a_e ) const {
	return getData() == a_e.userData;
}
//--------------------------------------------------------------
bool ofxBulletBaseShape::operator!=( const ofxBulletRaycastData& a_e ) const {
	return !(getData() == a_e.userData);
}




// GETTERS //

//--------------------------------------------------------------
btRigidBody* ofxBulletBaseShape::getRigidBody() {
	return _rigidBody;
}

//--------------------------------------------------------------
void* ofxBulletBaseShape::getData() const {
	return _rigidBody->getUserPointer();
}

//--------------------------------------------------------------
btCollisionShape* ofxBulletBaseShape::getCollisionShape() const {
	return _shape;
}

//--------------------------------------------------------------
int ofxBulletBaseShape::getActivationState() {
	// returns OF_BT_ACTIVATION_STATE_ACTIVE || OF_BT_ACTIVATION_ISLAND_SLEEPING
	return ((btCollisionObject*)_rigidBody->getCollisionShape())->getActivationState();
}

//--------------------------------------------------------------
int ofxBulletBaseShape::getType() {
	return _type;
}


//--------------------------------------------------------------
float ofxBulletBaseShape::getMass() const {
	return _mass;
}

//--------------------------------------------------------------
void ofxBulletBaseShape::getOpenGLMatrix( btScalar* a_m ) {
	ofGetOpenGLMatrixFromRigidBody( _rigidBody, a_m );
}

//--------------------------------------------------------------
ofMatrix4x4 ofxBulletBaseShape::getTransformationMatrix() const {
	float	m[16];
	ofGetOpenGLMatrixFromRigidBody( _rigidBody, m );
	ofMatrix4x4 mat;
	mat.set(m[0], m[1], m[2], m[3],
			m[4], m[5], m[6], m[7],
			m[8], m[9], m[10], m[11],
			m[12], m[13], m[14], m[15]);
	return mat;
}

//--------------------------------------------------------------
ofVec3f ofxBulletBaseShape::getPosition() const {
	return ofGetVec3fPosFromRigidBody( _rigidBody );
}

// returns yaw, pitch, roll //
//--------------------------------------------------------------
ofVec3f ofxBulletBaseShape::getRotation( ) const {
	return ofGetRotationFromRigidBody( _rigidBody );
}

//--------------------------------------------------------------
ofVec3f ofxBulletBaseShape::getRotationAxis() const {
	btQuaternion rotQuat		= _rigidBody->getWorldTransform().getRotation();
	btVector3 btaxis			= rotQuat.getAxis();
	return ofVec3f( btaxis.getX(), btaxis.getY(), btaxis.getZ() );
}

//--------------------------------------------------------------
float ofxBulletBaseShape::getRotationAngle() const {
	btQuaternion rotQuat		= _rigidBody->getWorldTransform().getRotation();
	return rotQuat.getAngle();
}

//--------------------------------------------------------------
ofQuaternion ofxBulletBaseShape::getRotationQuat() const {
	ofVec3f axis	= getRotationAxis();
	return ofQuaternion( axis.x, axis.y, axis.z, getRotationAngle());
}




//--------------------------------------------------------------
float ofxBulletBaseShape::getRestitution() const {
	return (float)_rigidBody->getRestitution();
}

//--------------------------------------------------------------
float ofxBulletBaseShape::getFriction() const {
	return _rigidBody->getFriction();
}

//--------------------------------------------------------------
float ofxBulletBaseShape::getDamping() const {
	return (float)_rigidBody->getLinearDamping();
}

//--------------------------------------------------------------
float ofxBulletBaseShape::getAngularDamping() const {
	return (float)_rigidBody->getAngularDamping();
}



/**************************************************************/
// PROPERTY SETTERS, must be called after create() and before add() //

//--------------------------------------------------------------
void ofxBulletBaseShape::setProperties(float a_restitution, float a_friction) {
	setRestitution(a_restitution);
	setFriction(a_friction);
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setRestitution( float a_res ) {
	if(checkCreate()) {
		_rigidBody->setRestitution( btScalar(a_res) );
	}
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setFriction( float a_friction ) {
	if(checkCreate()) {
		_rigidBody->setFriction( btScalar(a_friction) );
	}
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setActivationState( int a_state ) {
	if(checkCreate()) {
		_rigidBody->setActivationState( a_state );
	}
}
/**************************************************************/




// SETTERS, may be called after create() //
//--------------------------------------------------------------
void ofxBulletBaseShape::setData(void* userPointer) {
	_userPointer = userPointer;
	_rigidBody->setUserPointer( _userPointer );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setDamping( float a_linear_damp ) {
	setDamping( a_linear_damp, getAngularDamping() );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setAngularDamping( float a_angular_damp ) {
	setDamping( getDamping(), a_angular_damp );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setDamping( float a_linear_damp, float a_angular_damp ) {
	_rigidBody->setDamping(a_linear_damp, a_angular_damp);
}





// CHECKERS //
//--------------------------------------------------------------
bool ofxBulletBaseShape::checkInit() {
	if(!_bInited) {
		ofLog(OF_LOG_WARNING, "ofxBulletBaseShape :: must call init() first!");
		return false;
	}
	return true;
}

//--------------------------------------------------------------
bool ofxBulletBaseShape::checkPropCreate() {
	if(_bCreated) {
		ofLog(OF_LOG_WARNING, "ofxBulletBaseShape :: these property changes will not affect the shape, must call before create()!");
		return true;
	}
	return false;
}
		 
//--------------------------------------------------------------
bool ofxBulletBaseShape::checkCreate() {
	if(!_bCreated) {
		ofLog(OF_LOG_WARNING, "ofxBulletBaseShape :: must call create() first");
		return false;
	}
	return true;
}




//--------------------------------------------------------------
void ofxBulletBaseShape::activate() {
	//((btCollisionObject*)_rigidBody->getCollisionShape())->activate( true );
	getRigidBody()->activate( true );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::enableKinematic() {
	getRigidBody()->setCollisionFlags( getRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::applyForce( const ofVec3f& a_frc, const ofVec3f& a_rel_pos ) {
	_rigidBody->applyForce( btVector3(a_frc.x, a_frc.y, a_frc.z), btVector3(a_rel_pos.x, a_rel_pos.y, a_rel_pos.z) );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::applyForce( const btVector3& a_frc, const btVector3& a_rel_pos ) {
	_rigidBody->applyForce( a_frc, a_rel_pos );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::applyCentralForce( const ofVec3f& a_frc ) {
	applyCentralForce( btVector3(a_frc.x, a_frc.y, a_frc.z) );
}
void ofxBulletBaseShape::applyCentralForce( float a_x, float a_y, float a_z ) {
	applyCentralForce( btVector3(a_x, a_y, a_z) );
}
void ofxBulletBaseShape::applyCentralForce( const btVector3& a_frc ) {
	_rigidBody->applyCentralForce( a_frc );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::applyTorque( const ofVec3f& a_torque ) {
	applyTorque( btVector3(a_torque.x, a_torque.y, a_torque.z) );
}
void ofxBulletBaseShape::applyTorque( float a_x, float a_y, float a_z ) {
	applyTorque( btVector3( a_x, a_y, a_z ) );
}
void ofxBulletBaseShape::applyTorque( const btVector3& a_torque ) {
	_rigidBody->applyTorque( a_torque );
}










