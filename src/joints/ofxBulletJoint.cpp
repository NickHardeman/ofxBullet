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
	
	_bCreated = true;
}

//--------------------------------------------------------------
void ofxBulletJoint::create( btDiscreteDynamicsWorld* $world, ofxBulletBaseShape* $shape, ofVec3f $pos ) {
	_world = $world;
	// we should have these always influenced by the joint, so don't let them go to sleep //
	$shape->setActivationState( DISABLE_DEACTIVATION );
	
	//ofVec3f diff = $shape2->getPosition() - $shape1->getPosition();
	
	//btTransform frameInA = btTransform::getIdentity();
	//frameInA.setOrigin( btVector3(btScalar(-diff.x), btScalar(-diff.y), btScalar(-diff.z)) );
	//btTransform frameInB = btTransform::getIdentity();
	//frameInB.setOrigin( btVector3(btScalar(0.), btScalar(0.), btScalar(0.)) );
	
	//_joint = new btGeneric6DofConstraint(*$shape2->getRigidBody(), *$shape1->getRigidBody(), frameInA, frameInB, true);
	
	//_bCreated = true;
}

/******************************************************/
// call these before add() //
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
void ofxBulletJoint::draw() {
	ofVec3f pa = getPositionA();
	ofVec3f pb = getPositionB();
	
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
}







