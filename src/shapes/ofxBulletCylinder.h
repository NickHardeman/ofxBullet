/*
 *  ofxBulletCylinder.h
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/23/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#pragma once

#include "ofxBulletBaseShape.h"
#include "ofVbo.h"

class ofxBulletCylinder : public ofxBulletBaseShape {
public:
	ofxBulletCylinder();
	~ofxBulletCylinder();
	void	init(float a_radius, float a_height);
	void	init( btCylinderShape* a_colShape );
	
	void	create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc=ofVec3f(0,0,0), float a_mass=1.f, float a_radius=1.f, float a_height=10.f );
	// angles of Quaternion should be in Radians //
	void	create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc=ofVec3f(0,0,0), ofQuaternion a_rot=ofQuaternion(0, 1, 0, 1), 
				   float a_mass=1.f, float a_radius=1.f, float a_height=10.f );
	void	create( btDiscreteDynamicsWorld* a_world, btTransform a_bt_tr, float a_mass=1.f, float a_radius=1.f, float a_height=10.f );
	
	// GETTERS //
	float	getRadius();
	float	getHeight();
	
	void	draw();
	void	drawCylinder(float radius, float halfHeight, int upAxis);
	//void	destroy();
	
protected:
	

private:
	
};