/*
 *  ofxBulletWorldBase.h
 *  ofxBullet_v1
 *
 *  Created by Nick Hardeman on 3/22/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
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
	
	void setup();
	void update();
	void setCameraPosition( ofVec3f a_pos );
	void setCamera( ofCamera* a_cam );
	
	void enableCollisionEvents();
	void disableCollisionEvents();
	void checkCollisions();
	
	ofxBulletRaycastData raycastTest(float a_x, float a_y, short int a_filterMask=btBroadphaseProxy::AllFilter);
	//ofxBulletRaycastData raycastTest( ofVec3f a_castRay, short int a_filterMask=btBroadphaseProxy::AllFilter);
	ofxBulletRaycastData raycastTest( ofVec3f a_rayStart, ofVec3f a_rayEnd, short int a_filterMask=btBroadphaseProxy::AllFilter);
	
	void enableMousePickingEvents( short int a_filterMask=btBroadphaseProxy::AllFilter );
	void disableMousePickingEvents();
	void checkMousePicking(float a_mousex, float a_mousey);
	
	void enableGrabbing( short int a_filterMask=btBroadphaseProxy::DefaultFilter | btBroadphaseProxy::KinematicFilter | btBroadphaseProxy::DebrisFilter | btBroadphaseProxy::SensorTrigger );
	void disableGrabbing();
	
	void enableDebugDraw();
	void drawDebug();
	
	bool checkWorld();
	void setGravity( ofVec3f a_g );
	ofVec3f getGravity();
	
	void removeMouseConstraint();
	void destroy();
	
	void mouseMoved( ofMouseEventArgs &a);
	void mouseDragged( ofMouseEventArgs &a );
	void mousePressed( ofMouseEventArgs &a);
	void mouseReleased( ofMouseEventArgs &a);
	
	btBroadphaseInterface*					broadphase;
	btDefaultCollisionConfiguration*		collisionConfig;
	btCollisionDispatcher*					dispatcher;
	btSequentialImpulseConstraintSolver*	solver;
	
	btDiscreteDynamicsWorld*				world;
	btRigidBody*							ground;
	
protected:
	bool bDispatchCollisionEvents;
	bool bDispatchPickingEvents;
	bool bRegisterGrabbing;
	bool bHasDebugDrawer;
private:
	ofCamera*	_camera;
	ofVec3f		_cameraPos;
	bool		_bMouseDown;
	short int	_mouseFilterMask; // if you don't want to pick certain objects //
	btRigidBody* _pickedBody;
	
	///constraint for mouse picking
	btTypedConstraint*	_pickConstraint;
	float gOldPickingDist;
};




