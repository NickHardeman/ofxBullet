/*
 *  ofxBulletJoint.cpp
 *  ofxBullet_v7_joints
 *
 *  Created by Nick Hardeman on 7/6/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#include "ofxBulletJoint.h"

//--------------------------------------------------------------
ofxBulletJoint::ofxBulletJoint() {
	_bCreated		= false;
	_bAdded			= false;
	_bTwoBodies		= false;
	_targetPos.set(0, 0, 0);
}

//--------------------------------------------------------------
ofxBulletJoint::~ofxBulletJoint() {
	remove();
}

//--------------------------------------------------------------
void ofxBulletJoint::create( btDiscreteDynamicsWorld* a_world, ofxBulletBaseShape* a_shape1, ofxBulletBaseShape* a_shape2 ) {
	_world = a_world;
	// we should have these always influenced by the joint, so don't let them go to sleep //
	a_shape1->setActivationState( DISABLE_DEACTIVATION );
	a_shape2->setActivationState( DISABLE_DEACTIVATION );
	
	ofVec3f diff = a_shape2->getPosition() - a_shape1->getPosition();
	
	btTransform frameInA = btTransform::getIdentity();
	frameInA.setOrigin( btVector3(btScalar(-diff.x), btScalar(-diff.y), btScalar(-diff.z)) );
	btTransform frameInB = btTransform::getIdentity();
	frameInB.setOrigin( btVector3(btScalar(0.), btScalar(0.), btScalar(0.)) );
	
	_joint = new btGeneric6DofConstraint(*a_shape2->getRigidBody(), *a_shape1->getRigidBody(), frameInA, frameInB, true);
	
	_setDefaults();
	
	_bTwoBodies	= true;
	_bCreated	= true;
}

//--------------------------------------------------------------
void ofxBulletJoint::create( btDiscreteDynamicsWorld* a_world, ofxBulletBaseShape* a_shape, ofVec3f a_pos ) {
	_world = a_world;
	// we should have these always influenced by the joint, so don't let them go to sleep //
	a_shape->setActivationState( DISABLE_DEACTIVATION );
	
	btVector3 localPivot	= a_shape->getRigidBody()->getCenterOfMassTransform().inverse() * btVector3(a_pos.x, a_pos.y, a_pos.z);
	btTransform tr;
	tr.setIdentity();
	tr.setOrigin( localPivot );
	_joint = new btGeneric6DofConstraint(*a_shape->getRigidBody(), tr, false);
	
	_setDefaults();
	
	_targetPos.set(a_pos.x, a_pos.y, a_pos.z);
	_bTwoBodies = false;
	_bCreated	= true;
}

//--------------------------------------------------------------
void ofxBulletJoint::_setDefaults() {
	_joint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,0);
	_joint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,1);
	_joint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,2);
	_joint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,3);
	_joint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,4);
	_joint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,5);
	
	_joint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,0);
	_joint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,1);
	_joint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,2);
	_joint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,3);
	_joint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,4);
	_joint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,5);
}

/******************************************************/
// call these before add() and after create //
void ofxBulletJoint::setLinearLowerLimit( ofVec3f a_limit ) {
	setLinearLowerLimit( a_limit.x, a_limit.y, a_limit.z );
}
void ofxBulletJoint::setLinearLowerLimit( float a_x, float a_y, float a_z ) {
	_joint->setLinearLowerLimit( btVector3(a_x, a_y, a_z) );
}
void ofxBulletJoint::setLinearUpperLimit( ofVec3f a_limit ) {
	setLinearUpperLimit( a_limit.x, a_limit.y, a_limit.z );
}
void ofxBulletJoint::setLinearUpperLimit( float a_x, float a_y, float a_z ) {
	_joint->setLinearUpperLimit( btVector3(a_x, a_y, a_z) );
}
void ofxBulletJoint::setAngularLowerLimit( ofVec3f a_limit ) {
	setAngularLowerLimit( a_limit.x, a_limit.y, a_limit.z );
}
void ofxBulletJoint::setAngularLowerLimit( float a_x, float a_y, float a_z ) {
	_joint->setAngularLowerLimit( btVector3(a_x, a_y, a_z) );
}
void ofxBulletJoint::setAngularUpperLimit( ofVec3f a_limit ) {
	setAngularUpperLimit( a_limit.x, a_limit.y, a_limit.z );
}
void ofxBulletJoint::setAngularUpperLimit( float a_x, float a_y, float a_z ) {
	_joint->setAngularUpperLimit( btVector3(a_x, a_y, a_z) );
}
/******************************************************/

//--------------------------------------------------------------
void ofxBulletJoint::add() {
	_world->addConstraint(_joint, true);
	_bAdded = true;
}

//--------------------------------------------------------------
void ofxBulletJoint::remove() {
	cout << "ofxBulletJoint :: remove : " << endl;
	_world->removeConstraint(_joint);
	delete _joint;
	_joint = NULL;
}

//--------------------------------------------------------------
ofVec3f ofxBulletJoint::getPivotAWorldPos() {
	btQuaternion rotQuat	= _joint->getCalculatedTransformA().getRotation();
	btVector3 btaxis		= rotQuat.getAxis();
	btVector3 frameA		= _joint->getFrameOffsetA().getOrigin();
	ofVec3f dir				= ofVec3f(frameA.getX(), frameA.getY(), frameA.getZ());
	dir.normalize();
	dir.rotateRad(rotQuat.getAngle(), ofVec3f(btaxis.getX(), btaxis.getY(), btaxis.getZ() ));
	dir *= -frameA.length();
	return dir + getPivotBWorldPos();
}

//--------------------------------------------------------------
ofVec3f ofxBulletJoint::getPivotBWorldPos() {
	btVector3 trb = _joint->getCalculatedTransformB().getOrigin();
	return ofVec3f( trb.getX(), trb.getY(), trb.getZ() );
}

//--------------------------------------------------------------
btRigidBody* ofxBulletJoint::getRigidBodyA() const {
	return (btRigidBody*)&_joint->getRigidBodyA();
}

//--------------------------------------------------------------
btRigidBody* ofxBulletJoint::getRigidBodyB() const {
	return (btRigidBody*)&_joint->getRigidBodyB();
}

//--------------------------------------------------------------
ofVec3f ofxBulletJoint::getPositionA() const {
	return ofGetVec3fPosFromRigidBody( getRigidBodyA() );
}

//--------------------------------------------------------------
ofVec3f ofxBulletJoint::getPositionB() const {
	return ofGetVec3fPosFromRigidBody( getRigidBodyB() );
}

//--------------------------------------------------------------
void ofxBulletJoint::updatePivotPos( const ofVec3f a_pos, float a_length ) {
	if(!_bCreated) {ofLog(OF_LOG_ERROR, "ofxBulletJoint :: updatePivotPos : must call create() first"); return;}
	
	_joint->getFrameOffsetA().setOrigin( btVector3(a_pos.x, a_pos.y, a_pos.z) );
	
	_targetPos.set( a_pos.x, a_pos.y, a_pos.z );
}

//--------------------------------------------------------------
void ofxBulletJoint::draw() {
	if(!_bCreated) {ofLog(OF_LOG_ERROR, "ofxBulletJoint :: draw : must call create() first"); return;}
	
	ofVec3f pa;
	ofVec3f pb;
	
	pb = getPositionB();
	if(_bTwoBodies) {
		pa = getPositionA();
	} else {
		pa = _targetPos;
	}
	
	glBegin(GL_LINES);
		glVertex3f(pa.x, pa.y, pa.z);
		glVertex3f(pb.x, pb.y, pb.z);
	glEnd();
}

//--------------------------------------------------------------
// draws the length of the joint and locations of pivots, but if bodies are sprung more than the length,
// will not connect all the way //
void ofxBulletJoint::drawJointConstraints() {
	ofVec3f pa = getPivotAWorldPos();
	ofVec3f pb = getPivotBWorldPos();
	
	glBegin(GL_LINES);
		glVertex3f(pa.x, pa.y, pa.z);
		glVertex3f(pb.x, pb.y, pb.z);
	glEnd();
	
	ofSetColor(255, 0, 0);
	ofSphere(pa, .5);
	ofSetColor(0, 0, 255);
	ofSphere(pb, .5);
}







