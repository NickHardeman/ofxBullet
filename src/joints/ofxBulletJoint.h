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
	void	create( btDiscreteDynamicsWorld* a_world, ofxBulletRigidBody* a_shape, glm::vec3 a_pos );
	
	/******************************/
	// call before calling add() //
	void	setLinearLowerLimit( glm::vec3 a_limit );
	void	setLinearLowerLimit( float a_x, float a_y, float a_z );
	void	setLinearUpperLimit( glm::vec3 a_limit );
	void	setLinearUpperLimit( float a_x, float a_y, float a_z );
	void	setAngularLowerLimit( glm::vec3 a_limit );
	void	setAngularLowerLimit( float a_x, float a_y, float a_z );
	void	setAngularUpperLimit( glm::vec3 a_limit );
	void	setAngularUpperLimit( float a_x, float a_y, float a_z );
	/******************************/
	
	void	add();
	
	glm::vec3 getPivotAWorldPos();
	glm::vec3 getPivotBWorldPos();
	
	btRigidBody* getRigidBodyA() const;
	btRigidBody* getRigidBodyB() const;
	glm::vec3 getPositionA() const;
	glm::vec3 getPositionB() const;
	
	void	updatePivotPos( const glm::vec3 a_pos, float a_length );
	
	void	draw();
	void	drawJointConstraints();
	
	void	remove();
	
protected:
	void _setDefaults();
	
private:
	btDiscreteDynamicsWorld*	_world;
	btGeneric6DofConstraint*	_joint;
	glm::vec3					_targetPos;
	// is there two bodies the joint is connecting? if not, what is the target pos //
	bool						_bTwoBodies;
	bool						_bCreated;
	bool						_bAdded;
};

