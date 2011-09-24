/*
 *  ofxBulletCapsule.h
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/27/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */


#pragma once

#include "ofxBulletBaseShape.h"
#include "btShapeHull.h"

class ofxBulletCapsule : public ofxBulletBaseShape {
public:
	ofxBulletCapsule();
	~ofxBulletCapsule();
	
	void	init(float a_radius, float a_height);
	void	init( btCapsuleShape* a_colShape );
	
	void	create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc=ofVec3f(0,0,0), float a_mass=1.f, float a_radius=1.f, float a_height=10.f );
	// angles of Quaternion should be in Radians //
	void	create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc=ofVec3f(0,0,0), ofQuaternion a_rot=ofQuaternion(0, 1, 0, 1), 
				   float a_mass=1.f, float a_radius=1.f, float a_height=10.f );
	void	create( btDiscreteDynamicsWorld* a_world, btTransform a_bt_tr, float a_mass=1.f, float a_radius=1.f, float a_height=10.f );
	
	void	draw();
	
protected:
	btShapeHull* hull;
	
};