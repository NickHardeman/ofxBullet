/*
 *  ofxBulletJoint.h
 *  ofxBullet_v7_joints
 *
 *  Created by Nick Hardeman on 7/6/11.
 *
 */

#pragma once
#include "ofMain.h"
#include "btBulletDynamicsCommon.h"
#include "ofxBulletConstants.h"
#include "ofxBulletUtils.h"
#include "ofxBulletRigidBody.h"

// creates a btGeneric6DofConstraint joint, free rotation, no constraints //
class ofxBulletJoint {
public:
	ofxBulletJoint();
	~ofxBulletJoint();
	
	void	create( btDiscreteDynamicsWorld* a_world, ofxBulletRigidBody* a_shape1, ofxBulletRigidBody* a_shape2 );
	void	create( btDiscreteDynamicsWorld* a_world, ofxBulletRigidBody* a_shape, ofVec3f a_pos );
	
	/******************************/
	// call before calling add() //
	void	setLinearLowerLimit( ofVec3f a_limit );
	void	setLinearLowerLimit( float a_x, float a_y, float a_z );
	void	setLinearUpperLimit( ofVec3f a_limit );
	void	setLinearUpperLimit( float a_x, float a_y, float a_z );
	void	setAngularLowerLimit( ofVec3f a_limit );
	void	setAngularLowerLimit( float a_x, float a_y, float a_z );
	void	setAngularUpperLimit( ofVec3f a_limit );
	void	setAngularUpperLimit( float a_x, float a_y, float a_z );
	/******************************/
	
	void	add();
	
	ofVec3f getPivotAWorldPos();
	ofVec3f getPivotBWorldPos();
	
	btRigidBody* getRigidBodyA() const;
	btRigidBody* getRigidBodyB() const;
	ofVec3f getPositionA() const;
	ofVec3f getPositionB() const;
	
	void	updatePivotPos( const ofVec3f a_pos, float a_length );
	
	void	draw();
	void	drawJointConstraints();
	
	void	remove();
	
protected:
	void _setDefaults();
	
private:
	btDiscreteDynamicsWorld*	_world;
	btGeneric6DofConstraint*	_joint;
	ofVec3f						_targetPos;
	// is there two bodies the joint is connecting? if not, what is the target pos //
	bool						_bTwoBodies;
	bool						_bCreated;
	bool						_bAdded;
};

