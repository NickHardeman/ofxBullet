/*
 *  ofxBulletCapsule.cpp
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/27/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#include "ofxBulletCapsule.h"

//--------------------------------------------------------------
ofxBulletCapsule::ofxBulletCapsule() {
	_type	= OFX_BULLET_CAPSULE_SHAPE;
	hull	= NULL;
}

//--------------------------------------------------------------
ofxBulletCapsule::~ofxBulletCapsule() {
	
}

//--------------------------------------------------------------
void ofxBulletCapsule::init(float a_radius, float a_height) {
	_shape		= (btCollisionShape*)new btCapsuleShape( a_radius, a_height );
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletCapsule::init( btCapsuleShape* a_colShape ) {
	_shape		= (btCollisionShape*)a_colShape;
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletCapsule::create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, float a_mass, float a_radius, float a_height ) {
	create(a_world, ofGetBtTransformFromVec3f( a_loc ), a_mass, a_radius, a_height);
}

//--------------------------------------------------------------
void ofxBulletCapsule::create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, ofQuaternion a_rot, float a_mass, float a_radius, float a_height ) {
	btTransform tr	= ofGetBtTransformFromVec3f( a_loc );
	tr.setRotation( btQuaternion(btVector3(a_rot.x(), a_rot.y(), a_rot.z()), a_rot.w()) );
	
	create( a_world, tr, a_mass, a_radius, a_height );
}

//--------------------------------------------------------------
void ofxBulletCapsule::create( btDiscreteDynamicsWorld* a_world, btTransform a_bt_tr, float a_mass, float a_radius, float a_height ) {
	if(!_bInited || _shape == NULL) {
		ofxBulletBaseShape::create( a_world, (btCollisionShape*)new btCapsuleShape(a_radius, a_height ), a_bt_tr, a_mass );
	} else {
		ofxBulletBaseShape::create( a_world, _shape, a_bt_tr, a_mass );
	}
	setData( new ofxBulletUserData() );
}

//--------------------------------------------------------------
void ofxBulletCapsule::draw() {
	if(!_bCreated || _rigidBody == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBulletCapsule :: draw : must call create() first and add() after");
		return;
	}
	
	btScalar	m[16];
	ofGetOpenGLMatrixFromRigidBody( _rigidBody, m );
	glPushMatrix(); 
	glMultMatrixf( m );
	
	if(hull == NULL) {
		hull = new btShapeHull((btConvexShape*)_rigidBody->getCollisionShape());
	
		btScalar margin = _rigidBody->getCollisionShape()->getMargin();
		hull->buildHull(margin);
	}
	
	if (hull->numTriangles () > 0) {
		int index = 0;
		const unsigned int* idx = hull->getIndexPointer();
		const btVector3* vtx = hull->getVertexPointer();
		
		glBegin (GL_TRIANGLES);
		
		for (int i = 0; i < hull->numTriangles (); i++) {
			int i1 = index++;
			int i2 = index++;
			int i3 = index++;
			btAssert(i1 < hull->numIndices () &&
					 i2 < hull->numIndices () &&
					 i3 < hull->numIndices ());
			
			int index1 = idx[i1];
			int index2 = idx[i2];
			int index3 = idx[i3];
			btAssert(index1 < hull->numVertices () &&
					 index2 < hull->numVertices () &&
					 index3 < hull->numVertices ());
			
			btVector3 v1 = vtx[index1];
			btVector3 v2 = vtx[index2];
			btVector3 v3 = vtx[index3];
			btVector3 normal = (v3-v1).cross(v2-v1);
			normal.normalize();
			glNormal3f(normal.getX(),normal.getY(),normal.getZ());
			glVertex3f (v1.x(), v1.y(), v1.z());
			glVertex3f (v2.x(), v2.y(), v2.z());
			glVertex3f (v3.x(), v3.y(), v3.z());
			
		}
		glEnd ();
		
	}
	
	glPopMatrix();
}


