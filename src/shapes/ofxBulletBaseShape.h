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
	virtual void add();
	void	remove();
	void	removeRigidBody();
	
	// GETTERS //
	btRigidBody*	getRigidBody();
	virtual ofxBulletUserData*	getData() const;
	btCollisionShape* getCollisionShape() const;
	int				getActivationState();
	
	float			getMass() const;
	void			getOpenGLMatrix( btScalar* $m );
	ofMatrix4x4		getTransformationMatrix() const;
	ofVec3f			getPosition() const;
	ofVec3f			getRotation() const;
	ofVec3f			getRotationAxis() const;
	float			getRotationAngle() const;
	ofQuaternion	getRotationQuat() const;
	
	float			getRestitution() const;
	float			getFriction() const;
	float			getDamping() const;
	float			getAngularDamping() const;
	
	// used for checking collisions. Data is set using getData and operator is used to see if the same //
	bool	operator==( const ofxBulletUserData* userData) const;
	bool	operator==( const ofxBulletCollisionData& $collisionData) const;
	bool	operator!=( const ofxBulletCollisionData& $collisionData) const;
	
	bool	operator==( const ofxBulletMousePickEvent& $e ) const;
	bool	operator!=( const ofxBulletMousePickEvent& $e ) const;
	bool	operator==( const ofxBulletRaycastData& $e ) const;
	bool	operator!=( const ofxBulletRaycastData& $e ) const;
	
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
	
	ofxBulletUserData*			_userPointer;
	
	bool						_bInited;
	bool						_bCreated;
	bool						_bAdded;
};



