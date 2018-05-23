/*
 *  ofxBulletWorldBase.h
 *  ofxBullet_v1
 *
 *  Created by Nick Hardeman on 3/22/11.
 *
 */

#pragma once

// http://bulletphysics.org/mediawiki-1.5.8/index.php/Hello_World
#include "ofMain.h"
#include "btBulletDynamicsCommon.h"
#include "ofxBulletUtils.h"
#include "ofxBulletConstants.h"
#include "ofxBulletBaseShape.h"
#include "ofxBulletCollisionData.h"
#include "ofxBulletMousePickEvent.h"
#include "GLDebugDrawer.h"
#include "ofxBulletRaycastData.h"

class ofxBulletWorldRigid {
public:
	ofEvent<ofxBulletCollisionData> COLLISION_EVENT;
	ofEvent<ofxBulletMousePickEvent> MOUSE_PICK_EVENT;
	
	ofxBulletWorldRigid();
	~ofxBulletWorldRigid();
	
	virtual void setup();
    
	void update( float aDeltaTimef=1.0f/60.0f, int aNumIterations=6 );
	void setCameraPosition( glm::vec3 a_pos );
	void setCamera( ofCamera* a_cam );
	
	void enableCollisionEvents();
	void disableCollisionEvents();
	void checkCollisions();
	
	ofxBulletRaycastData raycastTest(float a_x, float a_y, short int a_filterMask=btBroadphaseProxy::AllFilter);
	//ofxBulletRaycastData raycastTest( glm::vec3 a_castRay, short int a_filterMask=btBroadphaseProxy::AllFilter);
	ofxBulletRaycastData raycastTest( glm::vec3 a_rayStart, glm::vec3 a_rayEnd, short int a_filterMask=btBroadphaseProxy::AllFilter);
	
	void enableMousePickingEvents( short int a_filterMask=btBroadphaseProxy::AllFilter );
	void disableMousePickingEvents();
	void checkMousePicking(float a_mousex, float a_mousey);
	
    btDiscreteDynamicsWorld* getWorld();
	void enableGrabbing( short int a_filterMask=btBroadphaseProxy::DefaultFilter | btBroadphaseProxy::KinematicFilter | btBroadphaseProxy::DebrisFilter | btBroadphaseProxy::SensorTrigger );
	void disableGrabbing();
	
	void enableDebugDraw();
	void drawDebug();
	
	bool checkWorld();
	virtual void setGravity( glm::vec3 a_g );
	glm::vec3 getGravity();
	
	void removeMouseConstraint();
	void destroy();
	
	void mouseMoved( ofMouseEventArgs &a);
	void mouseDragged( ofMouseEventArgs &a );
	void mousePressed( ofMouseEventArgs &a);
	void mouseReleased( ofMouseEventArgs &a);
	
	btBroadphaseInterface*					broadphase;
	btCollisionConfiguration*               collisionConfig;
	btCollisionDispatcher*					dispatcher;
	btSequentialImpulseConstraintSolver*	solver;
	
	btDiscreteDynamicsWorld*                world;
	btRigidBody*							ground;
	
protected:
    virtual btBroadphaseInterface* createBroadphase();
    virtual btCollisionConfiguration* createCollisionConfig();
    virtual btDiscreteDynamicsWorld* createWorld();
    
	bool bDispatchCollisionEvents;
	bool bDispatchPickingEvents;
	bool bRegisterGrabbing;
	bool bHasDebugDrawer;
    
private:
	ofCamera*	_camera;
	glm::vec3	_cameraPos;
	bool		_bMouseDown;
	short int	_mouseFilterMask; // if you don't want to pick certain objects //
	btRigidBody* _pickedBody;
	
	///constraint for mouse picking
	btTypedConstraint*	_pickConstraint;
	float gOldPickingDist;
};
