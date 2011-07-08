/*
 *  ofxBulletBaseCollisionShape.h
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/18/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#pragma once 
#include "ofMain.h"
#include "btBulletDynamicsCommon.h"
#include "ofxBulletConstants.h"
#include "ofxBulletUtils.h"
#include "ofxBulletUserData.h"
#include "ofxBulletCollisionData.h"
#include "ofxBulletMousePickEvent.h"

class ofxBulletBaseShape {
public:
	ofxBulletBaseShape();
	~ofxBulletBaseShape();
	
	virtual void create( btDiscreteDynamicsWorld* $world, btCollisionShape* $colShape, btTransform $bt_tr, float $mass );
	void			add();
	
	// GETTERS //
	btRigidBody*	getRigidBody();
	virtual ofxBulletUserData*	getData() const;
	int				getActivationState();
	
	float			getMass();
	void			getOpenGLMatrix( btScalar* $m );
	ofVec3f			getPosition();
	ofVec3f			getRotation();
	ofVec3f			getRotationAxis();
	float			getRotationAngle();
	ofQuaternion	getRotationQuat();
	
	float			getRestitution();
	float			getFriction();
	float			getDamping();
	float			getAngularDamping();
	
	// used for checking collisions. Data is set using getData and operator is used to see if the same //
	bool	operator==( const ofxBulletUserData* userData) const;
	bool	operator==( const ofxBulletCollisionData& $collisionData) const;
	bool	operator!=( const ofxBulletCollisionData& $collisionData) const;
	
	bool	operator==( const ofxBulletMousePickEvent& $e ) const;
	bool	operator!=( const ofxBulletMousePickEvent& $e ) const;
	
	// PROPERTY SETTERS, must be called after init() and before create() //
	void setProperties(float $restitution, float $friction);
	void setRestitution( float $res );
	void setFriction( float $friction );
	void setActivationState( int $state );
	
	// SETTERS, must be called after create() //
	void setDamping( float $linear_damp );
	void setAngularDamping( float $angular_damp );
	void setDamping( float $linear_damp, float $angular_damp );
	void activate();
	
	// CHECKERS //
	bool checkInit();
	bool checkPropCreate();
	bool checkCreate();
	
	// FORCES //
	void applyForce( const ofVec3f& $frc, const ofVec3f& $rel_pos );
	void applyForce( const btVector3& $frc, const btVector3& $rel_pos );
	
	void applyCentralForce( const ofVec3f& $frc );
	void applyCentralForce( float $x, float $y, float $z );
	void applyCentralForce( const btVector3& $frc );
	
	void applyTorque( const ofVec3f& $torque );
	void applyTorque( float $x, float $y, float $z );
	void applyTorque( const btVector3& $torque );
	
	// TODO: utilize ofVbo //
	virtual void draw() {};
	virtual void destroy() {};
	
	/*
	 const btCylinderShape* cylinder = static_cast<const btCylinderShape*>(shapes[Body::UPPER]);
	*/
protected:
	// if you extend this class, must overwrite in your class //
	virtual void setData(ofxBulletUserData* userPointer);
	btDiscreteDynamicsWorld*	_world;
	btCollisionShape*			_shape;
	btRigidBody*				_rigidBody;
	float						_mass;
	
	bool						_bInited;
	bool						_bCreated;
};



