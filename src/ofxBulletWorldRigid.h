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
	void setCameraPosition( ofVec3f $pos );
	void setCamera( ofCamera* $cam );
	
	void enableCollisionEvents();
	void disableCollisionEvents();
	void checkCollisions();
	
	ofxBulletRaycastData raycastTest(float $x, float $y, short int $filterMask=btBroadphaseProxy::AllFilter);
	
	void enableMousePickingEvents( short int $filterMask=btBroadphaseProxy::AllFilter );
	void disableMousePickingEvents();
	void checkMousePicking(float $mousex, float $mousey);
	
	void enableGrabbing( short int $filterMask=btBroadphaseProxy::AllFilter );
	void disableGrabbing();
	
	void enableDebugDraw();
	void drawDebug();
	
	bool checkWorld();
	void setGravity( ofVec3f $g );
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




