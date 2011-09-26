/*
 *  ofxBulletWorldBase.cpp
 *  ofxBullet_v1
 *
 *  Created by Nick Hardeman on 3/22/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#include "ofxBulletWorldRigid.h"

//--------------------------------------------------------------
ofxBulletWorldRigid::ofxBulletWorldRigid() {
	broadphase				= NULL;
	collisionConfig			= NULL;
	dispatcher				= NULL;
	solver					= NULL;
	world					= NULL;
	_camera					= NULL;
	_cameraPos				= ofVec3f(0, 0, 0);
	_bMouseDown				= false;
	_pickedBody				= NULL;
	_pickConstraint			= NULL;
	gOldPickingDist			= 0.f;
	bHasDebugDrawer			= false;
	// disable collision event dispatching by default //
	disableCollisionEvents();
	disableGrabbing();
	ofRegisterMouseEvents(this);
}

//--------------------------------------------------------------
ofxBulletWorldRigid::~ofxBulletWorldRigid() {
	destroy();
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::setup() {
	if(broadphase == NULL) {
		btVector3 worldAabbMin(-1000,-1000,-1000);
		btVector3 worldAabbMax(1000,1000,1000);
		broadphase = new btAxisSweep3 (worldAabbMin, worldAabbMax);
	}
	
	if(collisionConfig == NULL)			collisionConfig = new btDefaultCollisionConfiguration();
	if(dispatcher == NULL)				dispatcher = new btCollisionDispatcher( collisionConfig );
	if(solver == NULL)					solver = new btSequentialImpulseConstraintSolver;
	if(world == NULL)					world = new btDiscreteDynamicsWorld( dispatcher, broadphase, solver, collisionConfig );
	// default gravity //
	setGravity(ofVec3f(0.f, 9.8f, 0.f));
	
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::update() {
	if(!checkWorld()) return;
	// should this run on delta time? //
	world->stepSimulation(1.0f/60.0f, 6 );
	
	if(bDispatchCollisionEvents) {
		world->performDiscreteCollisionDetection();
		checkCollisions();
	}
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::setCameraPosition( ofVec3f a_pos ) {
	_cameraPos = a_pos;
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::setCamera( ofCamera* a_cam ) {
	_camera = a_cam;
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::enableCollisionEvents() {
	bDispatchCollisionEvents = true;
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::disableCollisionEvents() {
	bDispatchCollisionEvents = false;
}

// http://bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Callbacks_and_Triggers
//--------------------------------------------------------------
void ofxBulletWorldRigid::checkCollisions() {
	//Assume world->stepSimulation or world->performDiscreteCollisionDetection has been called
	int numManifolds = world->getDispatcher()->getNumManifolds();
	//cout << "numManifolds: " << numManifolds << endl;
	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
		
		int numContacts = contactManifold->getNumContacts();
		ofxBulletCollisionData cdata;
		if(numContacts > 0) {
			cdata.numContactPoints = numContacts;
			
			cdata.userData1 = (ofxBulletUserData*)obA->getUserPointer();
			cdata.body1		= btRigidBody::upcast(obA);
			
			cdata.userData2 = (ofxBulletUserData*)obB->getUserPointer();
			cdata.body2		= btRigidBody::upcast(obB);
		}
		
		for (int j = 0; j < numContacts; j++) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance() < 0.f) {
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;
				
				cdata.worldContactPoints1.push_back( ofVec3f(ptA.x(), ptA.y(), ptA.z()) );
				cdata.worldContactPoints2.push_back( ofVec3f(ptB.x(), ptB.y(), ptB.z()) );
				cdata.normalsOnShape2.push_back( ofVec3f(normalOnB.x(), normalOnB.y(), normalOnB.z()) );
			}
		}
		if(numContacts > 0) {
			ofNotifyEvent( COLLISION_EVENT, cdata, this );
		}
	}
	//you can un-comment out this line, and then all points are removed
	//contactManifold->clearManifold();	
	
}

//--------------------------------------------------------------
ofxBulletRaycastData ofxBulletWorldRigid::raycastTest(float a_x, float a_y, short int a_filterMask) {
	
	ofVec3f castRay = _camera->screenToWorld( ofVec3f(a_x, a_y, 0) );
	castRay = castRay - _camera->getPosition();
	castRay.normalize();
	castRay *= 300;
	
	return raycastTest( _camera->getPosition(), castRay, a_filterMask);
}

//--------------------------------------------------------------
ofxBulletRaycastData ofxBulletWorldRigid::raycastTest( ofVec3f a_rayStart, ofVec3f a_rayEnd, short int a_filterMask) {
	ofxBulletRaycastData data;
	data.bHasHit = false;
	if(_camera == NULL) {
		ofLog( OF_LOG_ERROR, "ofxBulletWorldRigid :: raycastTest : must set the camera first!!");
		return data;
	}
	
	btVector3 rayStart( a_rayStart.x, a_rayStart.y, a_rayStart.z );
	btVector3 rayEnd( a_rayEnd.x, a_rayEnd.y, a_rayEnd.z );
	
	btCollisionWorld::ClosestRayResultCallback rayCallback( rayStart, rayEnd );
	rayCallback.m_collisionFilterMask = a_filterMask;
	world->rayTest( rayStart, rayEnd, rayCallback );
	
	if (rayCallback.hasHit()) {
		btRigidBody* body = btRigidBody::upcast(rayCallback.m_collisionObject);
		if (body) {
			data.bHasHit			= true;
			data.userData			= (ofxBulletUserData*)body->getUserPointer();
			data.body				= body;
			data.rayWorldPos		= a_rayEnd;
			btVector3 pickPos		= rayCallback.m_hitPointWorld;
			data.pickPosWorld		= ofVec3f(pickPos.getX(), pickPos.getY(), pickPos.getZ());
			btVector3 localPos		= body->getCenterOfMassTransform().inverse() * pickPos;
			data.localPivotPos		= ofVec3f(localPos.getX(), localPos.getY(), localPos.getZ() );
		}
	}
	return data;
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::enableMousePickingEvents( short int a_filterMask ) {
	_mouseFilterMask = a_filterMask;
	bDispatchPickingEvents	= true;
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::disableMousePickingEvents() {
	bDispatchPickingEvents	= false;
}

//--------------------------------------------------------------
// pulled from DemoApplication in the AllBulletDemos project included in the Bullet physics download //
void ofxBulletWorldRigid::checkMousePicking(float a_mousex, float a_mousey) {
	ofxBulletRaycastData data = raycastTest(a_mousex, a_mousey, _mouseFilterMask);
	if(data.bHasHit) {
		ofxBulletMousePickEvent cdata;
		cdata.setRaycastData(data);
		if (cdata.body != NULL) {
			btVector3 m_cameraPosition( _camera->getPosition().x, _camera->getPosition().y, _camera->getPosition().z );
			//other exclusions?
			if (!(cdata.body->isStaticObject() || cdata.body->isKinematicObject()) && bRegisterGrabbing) {
				_pickedBody = cdata.body; //btRigidBody //
				_pickedBody->setActivationState( DISABLE_DEACTIVATION );
				
				btTransform tr;
				tr.setIdentity();
				tr.setOrigin(btVector3(cdata.localPivotPos.x, cdata.localPivotPos.y, cdata.localPivotPos.z));
				btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint(*cdata.body, tr, false);
				dof6->setLinearLowerLimit(btVector3(0,0,0));
				dof6->setLinearUpperLimit(btVector3(0,0,0));
				dof6->setAngularLowerLimit(btVector3(0,0,0));
				dof6->setAngularUpperLimit(btVector3(0,0,0));
				
				world->addConstraint(dof6);
				_pickConstraint = dof6;
				
				dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,0);
				dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,1);
				dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,2);
				dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,3);
				dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,4);
				dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,5);
				
				dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,0);
				dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,1);
				dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,2);
				dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,3);
				dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,4);
				dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,5);
				
				gOldPickingDist  = ( btVector3(cdata.pickPosWorld.x, cdata.pickPosWorld.y, cdata.pickPosWorld.z) - m_cameraPosition).length();
				
				//cout << "ofxBulletWorldRigid :: checkMousePicking : adding a mouse constraint" << endl;
			}
			//cout << "ofxBulletWorldRigid :: checkMousePicking : selected a body!!!" << endl;
			ofNotifyEvent( MOUSE_PICK_EVENT, cdata, this );
		}
	}
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::enableGrabbing(short int a_filterMask) {
	_mouseFilterMask = a_filterMask;
	bDispatchPickingEvents = true;
	bRegisterGrabbing = true;
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::disableGrabbing() {
	bRegisterGrabbing = false;
}


//--------------------------------------------------------------
void ofxBulletWorldRigid::enableDebugDraw() {
	if(!bHasDebugDrawer) {
		world->setDebugDrawer( new GLDebugDrawer() );
		// DBG_DrawContactPoints DBG_DrawAabb DBG_FastWireframe
		world->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
		bHasDebugDrawer = true;
	}
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::drawDebug() {
	if(!bHasDebugDrawer) {
		enableDebugDraw();
	}
	world->debugDrawWorld();
}


//--------------------------------------------------------------
bool ofxBulletWorldRigid::checkWorld() {
	if(world == NULL) {
		ofLog(OF_LOG_WARNING, "The world is not set, trying calling the init function first.");
		return false;
	}
	return true;
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::setGravity( ofVec3f a_g ) {
	if(!checkWorld()) return;
	world->setGravity( btVector3(a_g.x, a_g.y, a_g.z) );
}

//--------------------------------------------------------------
ofVec3f ofxBulletWorldRigid::getGravity() {
	if(!checkWorld()) return ofVec3f(0,0,0);
	btVector3 g = world->getGravity();
	return ofVec3f(g.getX(), g.getY(), g.getZ());
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::removeMouseConstraint() {
	if ( _pickConstraint != NULL && world != NULL ) {
		//cout << "ofxBulletWorldRigid :: checkMousePicking : removing a mouse constraint" << endl;
		world->removeConstraint( _pickConstraint );
		delete _pickConstraint;
		_pickConstraint = NULL;
	}
	if( _pickedBody != NULL && world != NULL) {
		_pickedBody->forceActivationState(ACTIVE_TAG);
		_pickedBody->setDeactivationTime( 0.f );
		_pickedBody		= NULL;
	}
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::destroy() {
	
	cout << "ofxBulletWorldRigid :: destroy : destroy() " << endl;
	//cleanup in the reverse order of creation/initialization
	int i;
	
	//remove/delete constraints
	if(world != NULL) {
		removeMouseConstraint();
		
		cout << "ofxBulletWorldRigid :: destroy : num constraints= " << world->getNumConstraints() << endl;
		for (i = world->getNumConstraints()-1; i >= 0; i--) {
			btTypedConstraint* constraint = world->getConstraint(i);
			world->removeConstraint(constraint);
			delete constraint;
		}
	}
	
	//remove the rigidbodies from the dynamics world and delete them
	if(world != NULL) {
		cout << "ofxBulletWorldRigid :: destroy : num collision objects= " << world->getNumCollisionObjects() << endl;
		for (i = world->getNumCollisionObjects()-1; i >= 0; i--) {
			btCollisionObject* obj = world->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body != 0 && body != NULL && body->getMotionState()) {
				delete body->getMotionState();
			}
			world->removeCollisionObject( obj );
			delete obj;
		}
	}
	
	if(world != NULL)				delete world; world = NULL;
    if(solver != NULL)				delete solver; solver = NULL;
	if(dispatcher != NULL)			delete dispatcher; dispatcher = NULL;
    if(collisionConfig != NULL)		delete collisionConfig; collisionConfig = NULL;
    if(broadphase != NULL)			delete broadphase; broadphase = NULL;
}


//--------------------------------------------------------------
void ofxBulletWorldRigid::mouseMoved( ofMouseEventArgs &a ) {
			
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::mouseDragged( ofMouseEventArgs &a ) {
	if (_pickConstraint != NULL) {
		//move the constraint pivot
		btGeneric6DofConstraint* pickCon = static_cast<btGeneric6DofConstraint*>(_pickConstraint);
		if (pickCon) {
			//cout << "ofxBulletWorldRigid :: mouseMoved : moving the mouse! with constraint" << endl;
			//keep it at the same picking distance
			ofVec3f mouseRay = _camera->screenToWorld( ofVec3f((float)a.x, (float)a.y, 0) );
			mouseRay = mouseRay - _camera->getPosition();
			mouseRay.normalize();
			mouseRay *= 300.f;
			btVector3 newRayTo(mouseRay.x, mouseRay.y, mouseRay.z);
			
			btVector3 oldPivotInB = pickCon->getFrameOffsetA().getOrigin();
			
			btVector3 m_cameraPosition( _camera->getPosition().x, _camera->getPosition().y, _camera->getPosition().z );
			btVector3 rayFrom = m_cameraPosition;
			
			btVector3 dir = newRayTo-rayFrom;
			dir.normalize();
			dir *= gOldPickingDist;
			
			btVector3 newPivotB = rayFrom + dir;
			
			pickCon->getFrameOffsetA().setOrigin(newPivotB);
		}
	}
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::mousePressed( ofMouseEventArgs &a ) {
	_bMouseDown = true;
	//cout << "ofxBulletWorldRigid :: mousePressed : x = " << a.x << " y = " << a.y << endl;
	if(bDispatchPickingEvents) checkMousePicking( (float) a.x, (float)a.y);
}

//--------------------------------------------------------------
void ofxBulletWorldRigid::mouseReleased( ofMouseEventArgs &a ) {
	_bMouseDown = false;
	
	removeMouseConstraint();
}














