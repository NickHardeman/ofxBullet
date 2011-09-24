/*
 *  ofxBulletCone.h
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/24/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#pragma once

#include "ofxBulletBaseShape.h"
#include "ofMain.h"
#include "ofVbo.h"

class ofxBulletCone : public ofxBulletBaseShape {
public:
	ofxBulletCone();
	~ofxBulletCone();
	
	void	init(float a_radius, float a_height);
	void	init( btConeShape* a_colShape );
	
	void	create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc=ofVec3f(0,0,0), float a_mass=1.f, float a_radius=1.f, float a_height=2.f );
	void	create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, ofQuaternion a_rot, float a_mass=1.f, float a_radius=1.f, float a_height=2.f );
	void	create( btDiscreteDynamicsWorld* a_world, btTransform a_bt_tr, float a_mass=1.f, float a_radius=1.f, float a_height=2.f );
	
	// GETTERS //
	float getHeight();
	float getRadius();
	int getUpIndex();
	
	void	draw();
protected:
	
private:

};