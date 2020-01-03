/*
 *  ofxBulletBaseCollisionShape.h
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/18/11.
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
	virtual ~ofxBulletBaseShape();
	
	enum CollisionFilterGroups {
		OFX_BULLET_BOX_SHAPE = 1,
		OFX_BULLET_SPHERE_SHAPE = 2,
		OFX_BULLET_CAPSULE_SHAPE = 3,
		OFX_BULLET_CONE_SHAPE = 4,
		OFX_BULLET_CYLINDER_SHAPE = 5,
		OFX_BULLET_CUSTOM_SHAPE = 6,
        OFX_BULLET_CONVEX_SHAPE = 7,
        OFX_BULLET_TRI_MESH_SHAPE = 8,

		OFX_BULLET_SOFT_ROPE = 20,
        OFX_BULLET_SOFT_PATCH = 21,
        OFX_BULLET_SOFT_ELLIPSOID = 22,
        OFX_BULLET_SOFT_TRI_MESH = 23
	};
	
    virtual void add() = 0;
	virtual void remove() = 0;
    
	// GETTERS //
	btCollisionObject* getCollisionObject();
	virtual void*	getData() const;
	int				getActivationState();
	
	int				getType();
	
	ofMatrix4x4		getTransformationMatrix() const;
	virtual glm::vec3 getPosition() const;
	glm::vec3		getRotation() const;
	glm::vec3		getRotationAxis() const;
	float			getRotationAngle() const;
    glm::quat   	getRotationQuat() const;
	
	float			getRestitution() const;
	float			getFriction() const;
	
	// used for checking collisions. Data is set using getData and operator is used to see if the same //
	bool	operator==( const void* userData) const;
	bool	operator==( const ofxBulletCollisionData& a_collisionData) const;
	bool	operator!=( const ofxBulletCollisionData& a_collisionData) const;
	
	bool	operator==( const ofxBulletMousePickEvent& a_e ) const;
	bool	operator!=( const ofxBulletMousePickEvent& a_e ) const;
	bool	operator==( const ofxBulletRaycastData& a_e ) const;
	bool	operator!=( const ofxBulletRaycastData& a_e ) const;
	
	// PROPERTY SETTERS, must be called after init() and before create() //
	// if you extend this class, must overwrite in your class //
	virtual void setData(void* userPointer);
	void setProperties(float a_restitution, float a_friction);
	void setRestitution( float a_res );
	void setFriction( float a_friction );
	void setActivationState( int a_state );
	
	// SETTERS, must be called after create() //
	void activate();
	// can be called at any time //
	// if you want to control the object. Make sure to have a mass of 0.f
	// and manipulate the rigid body and then call activate() //
	void enableKinematic();
    void enableStatic();
	
	// CHECKERS //
	bool checkInit();
	bool checkPropCreate();
	bool checkCreate();
	
	virtual void draw() {};
    
    virtual void transformGL();
    virtual void restoreTransformGL();
	
protected:
	void setCreated(btCollisionObject* object);
	void setRemoved();

    // initially use call this function in shape constructors so we know that the data was created
    // internally //
    void createInternalUserData();

	btCollisionObject* _object;
	
	void*	_userPointer;
	
	bool	_bInited;
	bool	_bCreated;
	bool	_bAdded;
    
	bool	_bUserDataCreatedInternally;
	
	int		_type;
};



