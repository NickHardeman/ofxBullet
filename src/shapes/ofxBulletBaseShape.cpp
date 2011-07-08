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
}

//--------------------------------------------------------------
ofxBulletBaseShape::~ofxBulletBaseShape() {
	
}

// you can call this function directly if you wish to use the same collision object for multiple rigid bodies,
// which will increase performance //
//--------------------------------------------------------------
void ofxBulletBaseShape::create( btDiscreteDynamicsWorld* $world, btCollisionShape* $colShape, btTransform $bt_tr, float $mass ) {
	if($world == NULL) {
		ofLog(OF_LOG_ERROR, "ofxBulletSphere :: create : $world param is NULL");
		return;
	}
	_mass			= $mass;
	_world			= $world;
	
	_rigidBody		= ofGetBtRigidBodyFromCollisionShape( $colShape, $bt_tr, $mass);
	
	_bCreated		= true;
}

//--------------------------------------------------------------
void ofxBulletBaseShape::add() {
	_world->addRigidBody( _rigidBody );
}


//--------------------------------------------------------------
bool ofxBulletBaseShape::operator==( const ofxBulletUserData* $userData) const {
	return getData() == $userData;
}

//--------------------------------------------------------------
bool ofxBulletBaseShape::operator==( const ofxBulletCollisionData& $collisionData) const {
	return (getData() == $collisionData.userData1) || (getData() == $collisionData.userData2);
}

//--------------------------------------------------------------
bool ofxBulletBaseShape::operator!=( const ofxBulletCollisionData& $collisionData) const {
	return (getData() != $collisionData.userData1) && (getData() != $collisionData.userData2);
}

//--------------------------------------------------------------
bool ofxBulletBaseShape::operator==( const ofxBulletMousePickEvent& $e ) const {
	return getData() == $e.userData;
}
//--------------------------------------------------------------
bool ofxBulletBaseShape::operator!=( const ofxBulletMousePickEvent& $e ) const {
	return !(getData() == $e.userData);
}




// GETTERS //

//--------------------------------------------------------------
btRigidBody* ofxBulletBaseShape::getRigidBody() {
	return _rigidBody;
}

//--------------------------------------------------------------
ofxBulletUserData* ofxBulletBaseShape::getData() const {
	return (ofxBulletUserData*)_rigidBody->getUserPointer();
}

//--------------------------------------------------------------
int ofxBulletBaseShape::getActivationState() {
	// returns OF_BT_ACTIVATION_STATE_ACTIVE || OF_BT_ACTIVATION_ISLAND_SLEEPING
	return ((btCollisionObject*)_rigidBody->getCollisionShape())->getActivationState();
}



//--------------------------------------------------------------
float ofxBulletBaseShape::getMass() {
	return _mass;
}

//--------------------------------------------------------------
void ofxBulletBaseShape::getOpenGLMatrix( btScalar* $m ) {
	ofGetOpenGLMatrixFromRigidBody( _rigidBody, $m );
}

//--------------------------------------------------------------
ofVec3f ofxBulletBaseShape::getPosition() {
	return ofGetVec3fPosFromRigidBody( _rigidBody );
}

// returns yaw, pitch, roll //
//--------------------------------------------------------------
ofVec3f ofxBulletBaseShape::getRotation( ) {
	return ofGetRotationFromRigidBody( _rigidBody );
}

//--------------------------------------------------------------
ofVec3f ofxBulletBaseShape::getRotationAxis() {
	btQuaternion rotQuat		= _rigidBody->getWorldTransform().getRotation();
	btVector3 btaxis			= rotQuat.getAxis();
	return ofVec3f( btaxis.getX(), btaxis.getY(), btaxis.getZ() );
}

//--------------------------------------------------------------
float ofxBulletBaseShape::getRotationAngle() {
	btQuaternion rotQuat		= _rigidBody->getWorldTransform().getRotation();
	return rotQuat.getAngle();
}

//--------------------------------------------------------------
ofQuaternion ofxBulletBaseShape::getRotationQuat() {
	ofVec3f axis	= getRotationAxis();
	return ofQuaternion( axis.x, axis.y, axis.z, getRotationAngle());
}




//--------------------------------------------------------------
float ofxBulletBaseShape::getRestitution() {
	return (float)_rigidBody->getRestitution();
}

//--------------------------------------------------------------
float ofxBulletBaseShape::getFriction() {
	return _rigidBody->getFriction();
}

//--------------------------------------------------------------
float ofxBulletBaseShape::getDamping() {
	return (float)_rigidBody->getLinearDamping();
}

//--------------------------------------------------------------
float ofxBulletBaseShape::getAngularDamping() {
	return (float)_rigidBody->getAngularDamping();
}



/**************************************************************/
// PROPERTY SETTERS, must be called after init() and before create() //

//--------------------------------------------------------------
void ofxBulletBaseShape::setProperties(float $restitution, float $friction) {
	setRestitution($restitution);
	setFriction($friction);
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setRestitution( float $res ) {
	if(checkCreate()) {
		_rigidBody->setRestitution( btScalar($res) );
	}
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setFriction( float $friction ) {
	if(checkCreate()) {
		_rigidBody->setFriction( btScalar($friction) );
	}
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setActivationState( int $state ) {
	if(checkCreate()) {
		_rigidBody->setActivationState( $state );
	}
}
/**************************************************************/




// SETTERS, may be called after create() //
//--------------------------------------------------------------
void ofxBulletBaseShape::setData(ofxBulletUserData* userPointer) {
	_rigidBody->setUserPointer( userPointer );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setDamping( float $linear_damp ) {
	setDamping( $linear_damp, getAngularDamping() );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setAngularDamping( float $angular_damp ) {
	setDamping( getDamping(), $angular_damp );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setDamping( float $linear_damp, float $angular_damp ) {
	_rigidBody->setDamping($linear_damp, $angular_damp);
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
	((btCollisionObject*)_rigidBody->getCollisionShape())->activate( true );
}


//--------------------------------------------------------------
void ofxBulletBaseShape::applyForce( const ofVec3f& $frc, const ofVec3f& $rel_pos ) {
	_rigidBody->applyForce( btVector3($frc.x, $frc.y, $frc.z), btVector3($rel_pos.x, $rel_pos.y, $rel_pos.z) );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::applyForce( const btVector3& $frc, const btVector3& $rel_pos ) {
	_rigidBody->applyForce( $frc, $rel_pos );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::applyCentralForce( const ofVec3f& $frc ) {
	applyCentralForce( btVector3($frc.x, $frc.y, $frc.z) );
}
void ofxBulletBaseShape::applyCentralForce( float $x, float $y, float $z ) {
	applyCentralForce( btVector3($x, $y, $z) );
}
void ofxBulletBaseShape::applyCentralForce( const btVector3& $frc ) {
	_rigidBody->applyCentralForce( $frc );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::applyTorque( const ofVec3f& $torque ) {
	applyTorque( btVector3($torque.x, $torque.y, $torque.z) );
}
void ofxBulletBaseShape::applyTorque( float $x, float $y, float $z ) {
	applyTorque( btVector3( $x, $y, $z ) );
}
void ofxBulletBaseShape::applyTorque( const btVector3& $torque ) {
	_rigidBody->applyTorque( $torque );
}










