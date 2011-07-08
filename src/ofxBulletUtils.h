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

static btTransform ofGetBtTransformFromVec3f( ofVec3f $v ) {
	btTransform tr;
	tr.setIdentity();
	tr.setOrigin( btVector3( btScalar($v.x), btScalar($v.y), btScalar($v.z)) );
	tr.getBasis().setEulerZYX( 0,0,0 );
	return tr;
}

static btRigidBody* ofGetBtRigidBodyFromCollisionShape(btCollisionShape* $cs, btTransform $tr, float $mass) {
	btDefaultMotionState* motionState	= new btDefaultMotionState( $tr );
	btScalar mass	= btScalar( $mass );
	btVector3 fallInertia(0., 0., 0.);
	$cs->calculateLocalInertia( mass, fallInertia );
	
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI( mass, motionState, $cs, fallInertia );
	return new btRigidBody( fallRigidBodyCI );
}

static btRigidBody* ofGetBtRigidBodyFromCollisionShape(btCollisionShape* $cs, ofVec3f $loc, float $mass) {
	return ofGetBtRigidBodyFromCollisionShape( $cs, ofGetBtTransformFromVec3f($loc), $mass);
}

// Collision Shapes //
static btBoxShape* ofBtGetBoxCollisionShape( float $sizeX, float $sizeY, float $sizeZ ) {
	//btVector3 boxHalfExtents;
	//boxHalfExtents.setX($sizeX*.5f); boxHalfExtents.setY($sizeY*.5f); boxHalfExtents.setZ($sizeZ*.5f);
	return new btBoxShape( btVector3($sizeX*.5, $sizeY*.5, $sizeZ*.5) );
}

// Collision Shapes //
static btBoxShape* ofBtGetBoxCollisionShape( float $size ) {
	return ofBtGetBoxCollisionShape( $size, $size, $size );
}

static btCylinderShape* ofBtGetCylinderCollisionShape( float $radius, float $height ) {
	return new btCylinderShape( btVector3($radius, $height, $radius) );
}

static btSphereShape* ofBtGetSphereCollisionShape( float $radius ) {
		return new btSphereShape( $radius );
}



static float ofGetRadiusFromBtSphere( btRigidBody* $s ) {
	return ((btSphereShape*) $s->getCollisionShape())->getRadius();
}

static ofVec3f ofGetVec3fPosFromRigidBody( btRigidBody* $rb ) {
	btTransform trans;
	$rb->getMotionState()->getWorldTransform( trans );
	btVector3 btv = trans.getOrigin();
	return ofVec3f( btv.getX(), btv.getY(), btv.getZ() );
}

// returns ofVec3f( yaw, pitch, roll ) //
static ofVec3f ofGetRotationFromRigidBody( btRigidBody* $rb ) {
	btTransform transform;
	$rb->getMotionState()->getWorldTransform( transform );
	
	btScalar yaw, pitch, roll;
	btMatrix3x3 mat = btMatrix3x3( transform.getRotation() );
	mat.getEulerYPR(yaw, pitch, roll);
	return ofVec3f( yaw, pitch, roll );
}

// in degress //
/*
static void ofSetRotationRigidBody( btRigidBody* $rb, float $yaw, float $pitch, float $roll) {
	btTransform transform;
	$rb->getMotionState()->getWorldTransform( transform );
	
	//btScalar yaw, pitch, roll;
	btMatrix3x3 mat = btMatrix3x3( transform.getRotation() );
	mat.setEulerYPR($yaw, $pitch, $roll);
	
	transform.setBasis( mat );
	
	$rb->getMotionState()->setWorldTransform( transform );
}
*/

static void ofGetOpenGLMatrixFromRigidBody( btRigidBody* $rb, btScalar* $m ) {
	btDefaultMotionState* myMotionState = (btDefaultMotionState*)$rb->getMotionState();
	myMotionState->m_graphicsWorldTrans.getOpenGLMatrix( $m );
}




