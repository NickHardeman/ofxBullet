/*
 *  ofxBulletJoint.h
 *  ofxBullet_v7_joints
 *
 *  Created by Nick Hardeman on 7/6/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "btBulletDynamicsCommon.h"
#include "ofxBulletConstants.h"
#include "ofxBulletUtils.h"
#include "ofxBulletBaseShape.h"

// creates a btGeneric6DofConstraint joint, free rotation, no constraints //
class ofxBulletJoint {
public:
	ofxBulletJoint();
	~ofxBulletJoint();
	
	void	create( btDiscreteDynamicsWorld* $world, ofxBulletBaseShape* $shape1, ofxBulletBaseShape* $shape2 );
	void	create( btDiscreteDynamicsWorld* $world, ofxBulletBaseShape* $shape, ofVec3f $pos );
	
	/******************************/
	// call before calling add() //
	void	setLinearLowerLimit( ofVec3f $limit );
	void	setLinearLowerLimit( float $x, float $y, float $z );
	void	setLinearUpperLimit( ofVec3f $limit );
	void	setLinearUpperLimit( float $x, float $y, float $z );
	void	setAngularLowerLimit( ofVec3f $limit );
	void	setAngularLowerLimit( float $x, float $y, float $z );
	void	setAngularUpperLimit( ofVec3f $limit );
	void	setAngularUpperLimit( float $x, float $y, float $z );
	/******************************/
	
	void	add();
	
	ofVec3f getPivotAWorldPos();
	ofVec3f getPivotBWorldPos();
	
	btRigidBody* getRigidBodyA() const;
	btRigidBody* getRigidBodyB() const;
	ofVec3f getPositionA() const;
	ofVec3f getPositionB() const;
	
	void	updatePivotPos( const ofVec3f $pos, float $length );
	
	void	draw();
	void	drawJointConstraints();
	
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

