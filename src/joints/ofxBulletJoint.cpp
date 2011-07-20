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
	
}

//--------------------------------------------------------------
void ofxBulletJoint::create( btDiscreteDynamicsWorld* $world, ofxBulletBaseShape* $shape1, ofxBulletBaseShape* $shape2 ) {
	_world = $world;
	// we should have these always influenced by the joint, so don't let them go to sleep //
	$shape1->setActivationState( DISABLE_DEACTIVATION );
	$shape2->setActivationState( DISABLE_DEACTIVATION );
	
	ofVec3f diff = $shape2->getPosition() - $shape1->getPosition();
	
	btTransform frameInA = btTransform::getIdentity();
	frameInA.setOrigin( btVector3(btScalar(-diff.x), btScalar(-diff.y), btScalar(-diff.z)) );
	btTransform frameInB = btTransform::getIdentity();
	frameInB.setOrigin( btVector3(btScalar(0.), btScalar(0.), btScalar(0.)) );
	
	_joint = new btGeneric6DofConstraint(*$shape2->getRigidBody(), *$shape1->getRigidBody(), frameInA, frameInB, true);
	
	_setDefaults();
	
	_bTwoBodies	= true;
	_bCreated	= true;
}

//--------------------------------------------------------------
void ofxBulletJoint::create( btDiscreteDynamicsWorld* $world, ofxBulletBaseShape* $shape, ofVec3f $pos ) {
	_world = $world;
	// we should have these always influenced by the joint, so don't let them go to sleep //
	$shape->setActivationState( DISABLE_DEACTIVATION );
	
	btVector3 localPivot	= $shape->getRigidBody()->getCenterOfMassTransform().inverse() * btVector3($pos.x, $pos.y, $pos.z);
	btTransform tr;
	tr.setIdentity();
	tr.setOrigin( localPivot );
	_joint = new btGeneric6DofConstraint(*$shape->getRigidBody(), tr, false);
	
	_setDefaults();
	
	_targetPos.set($pos.x, $pos.y, $pos.z);
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
void ofxBulletJoint::setLinearLowerLimit( ofVec3f $limit ) {
	setLinearLowerLimit( $limit.x, $limit.y, $limit.z );
}
void ofxBulletJoint::setLinearLowerLimit( float $x, float $y, float $z ) {
	_joint->setLinearLowerLimit( btVector3($x, $y, $z) );
}
void ofxBulletJoint::setLinearUpperLimit( ofVec3f $limit ) {
	setLinearUpperLimit( $limit.x, $limit.y, $limit.z );
}
void ofxBulletJoint::setLinearUpperLimit( float $x, float $y, float $z ) {
	_joint->setLinearUpperLimit( btVector3($x, $y, $z) );
}
void ofxBulletJoint::setAngularLowerLimit( ofVec3f $limit ) {
	setAngularLowerLimit( $limit.x, $limit.y, $limit.z );
}
void ofxBulletJoint::setAngularLowerLimit( float $x, float $y, float $z ) {
	_joint->setAngularLowerLimit( btVector3($x, $y, $z) );
}
void ofxBulletJoint::setAngularUpperLimit( ofVec3f $limit ) {
	setAngularUpperLimit( $limit.x, $limit.y, $limit.z );
}
void ofxBulletJoint::setAngularUpperLimit( float $x, float $y, float $z ) {
	_joint->setAngularUpperLimit( btVector3($x, $y, $z) );
}
/******************************************************/

//--------------------------------------------------------------
void ofxBulletJoint::add() {
	_world->addConstraint(_joint, true);
	_bAdded = true;
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
void ofxBulletJoint::updatePivotPos( const ofVec3f $pos, float $length ) {
	if(!_bCreated) {ofLog(OF_LOG_ERROR, "ofxBulletJoint :: updatePivotPos : must call create() first"); return;}
	
	_joint->getFrameOffsetA().setOrigin( btVector3($pos.x, $pos.y, $pos.z) );
	
	_targetPos.set( $pos.x, $pos.y, $pos.z );
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







