//
//  ofxBulletRigidBody.h
//  ofxBullet
//
//  Created by Elias Zananiri on 2014-11-12.
//
//

#pragma once

#include "ofMain.h"
#include "ofxBulletBaseShape.h"

class ofxBulletRigidBody : public ofxBulletBaseShape {
public:
	ofxBulletRigidBody();
	~ofxBulletRigidBody();

	virtual void create( btDiscreteDynamicsWorld* a_world, btCollisionShape* a_colShape, btTransform &a_bt_tr, float a_mass );
	virtual void add();
    virtual void add( short group, short mask );
    virtual void remove();
    virtual void removeShape();
	virtual void removeRigidBody();
	
	// GETTERS //
	btRigidBody* getRigidBody();
	btCollisionShape* getCollisionShape() const;

    bool	isCollisionShapeInternal();
	
	float	getMass() const;

	void	getOpenGLMatrix( btScalar* a_m );

	float	getDamping() const;
	float	getAngularDamping() const;

	// SETTERS, must be called after create() //
	void setDamping( float a_linear_damp );
	void setAngularDamping( float a_angular_damp );
	void setDamping( float a_linear_damp, float a_angular_damp );
	
	// FORCES //
    void applyForce( const glm::vec3& a_frc, const glm::vec3& a_rel_pos );
	void applyForce( const btVector3& a_frc, const btVector3& a_rel_pos );
	
	void applyCentralForce( const glm::vec3& a_frc );
	void applyCentralForce( float a_x, float a_y, float a_z );
	void applyCentralForce( const btVector3& a_frc );
	
	void applyTorque( const glm::vec3& a_torque );
	void applyTorque( float a_x, float a_y, float a_z );
	void applyTorque( const btVector3& a_torque );

	virtual void transformGL();

protected:
	btDiscreteDynamicsWorld*	_world;
	btCollisionShape*			_shape;
	btRigidBody*				_rigidBody;
	float						_mass;
	
	// if the collision shape was created internally, then, we must delete it //
    bool                        _bColShapeCreatedInternally;
};
