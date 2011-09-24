/*
 *  OfBtUtils.h
 *  ofxBullet_v1
 *
 *  Created by Nick Hardeman on 3/25/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#pragma once

#include "btBulletDynamicsCommon.h"

static btTransform ofGetBtTransformFromVec3f( ofVec3f a_v ) {
	btTransform tr;
	tr.setIdentity();
	tr.setOrigin( btVector3( btScalar(a_v.x), btScalar(a_v.y), btScalar(a_v.z)) );
	tr.getBasis().setEulerZYX( 0,0,0 );
	return tr;
}

static btRigidBody* ofGetBtRigidBodyFromCollisionShape(btCollisionShape* a_cs, btTransform a_tr, float a_mass) {
	btDefaultMotionState* motionState	= new btDefaultMotionState( a_tr );
	btScalar mass	= btScalar( a_mass );
	btVector3 fallInertia(0., 0., 0.);
	a_cs->calculateLocalInertia( mass, fallInertia );
	
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI( mass, motionState, a_cs, fallInertia );
	return new btRigidBody( fallRigidBodyCI );
}

static btRigidBody* ofGetBtRigidBodyFromCollisionShape(btCollisionShape* a_cs, ofVec3f a_loc, float a_mass) {
	return ofGetBtRigidBodyFromCollisionShape( a_cs, ofGetBtTransformFromVec3f(a_loc), a_mass);
}

// Collision Shapes //
static btBoxShape* ofBtGetBoxCollisionShape( float a_sizeX, float a_sizeY, float a_sizeZ ) {
	//btVector3 boxHalfExtents;
	//boxHalfExtents.setX(a_sizeX*.5f); boxHalfExtents.setY(a_sizeY*.5f); boxHalfExtents.setZ(a_sizeZ*.5f);
	return new btBoxShape( btVector3(a_sizeX*.5, a_sizeY*.5, a_sizeZ*.5) );
}

// Collision Shapes //
static btBoxShape* ofBtGetBoxCollisionShape( float a_size ) {
	return ofBtGetBoxCollisionShape( a_size, a_size, a_size );
}

static btCylinderShape* ofBtGetCylinderCollisionShape( float a_radius, float a_height ) {
	return new btCylinderShape( btVector3(a_radius, a_height, a_radius) );
}

static btSphereShape* ofBtGetSphereCollisionShape( float a_radius ) {
		return new btSphereShape( a_radius );
}



static float ofGetRadiusFromBtSphere( btRigidBody* a_s ) {
	return ((btSphereShape*) a_s->getCollisionShape())->getRadius();
}

static ofVec3f ofGetVec3fPosFromRigidBody( btRigidBody* a_rb ) {
	btTransform trans;
	a_rb->getMotionState()->getWorldTransform( trans );
	btVector3 btv = trans.getOrigin();
	return ofVec3f( btv.getX(), btv.getY(), btv.getZ() );
}

// returns ofVec3f( yaw, pitch, roll ) //
static ofVec3f ofGetRotationFromRigidBody( btRigidBody* a_rb ) {
	btTransform transform;
	a_rb->getMotionState()->getWorldTransform( transform );
	
	btScalar yaw, pitch, roll;
	btMatrix3x3 mat = btMatrix3x3( transform.getRotation() );
	mat.getEulerYPR(yaw, pitch, roll);
	return ofVec3f( yaw, pitch, roll );
}

// in degress //
/*
static void ofSetRotationRigidBody( btRigidBody* a_rb, float a_yaw, float a_pitch, float a_roll) {
	btTransform transform;
	a_rb->getMotionState()->getWorldTransform( transform );
	
	//btScalar yaw, pitch, roll;
	btMatrix3x3 mat = btMatrix3x3( transform.getRotation() );
	mat.setEulerYPR(a_yaw, a_pitch, a_roll);
	
	transform.setBasis( mat );
	
	a_rb->getMotionState()->setWorldTransform( transform );
}
*/

static void ofGetOpenGLMatrixFromRigidBody( btRigidBody* a_rb, btScalar* a_m ) {
	btDefaultMotionState* myMotionState = (btDefaultMotionState*)a_rb->getMotionState();
	myMotionState->m_graphicsWorldTrans.getOpenGLMatrix( a_m );
}




