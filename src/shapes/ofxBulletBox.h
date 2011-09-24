/*
 *  ofxBulletBox.h
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/23/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#pragma once

#include "ofxBulletBaseShape.h"
#include "ofVbo.h"

class ofxBulletBox : public ofxBulletBaseShape {
public:
	ofxBulletBox();
	~ofxBulletBox();
	
	void	init( float a_sizeX, float a_sizeY, float a_sizeZ );
	void	init( btBoxShape* a_colShape );
	
	void	create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc=ofVec3f(0,0,0), float a_mass=1.f, float a_sizeX=5.f, float a_sizeY=5.f, float a_sizeZ=5.f );
	void	create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, ofQuaternion a_rot, float a_mass=1.f, float a_sizeX=5.f, float a_sizeY=5.f, float a_sizeZ=5.f );
	void	create( btDiscreteDynamicsWorld* a_world, btTransform a_bt_tr, float a_mass=1.f, float a_sizeX=5.f, float a_sizeY=5.f, float a_sizeZ=5.f );
	
	void	draw();
	void	drawBox(float sizeX, float sizeY, float sizeZ);
	
	// GETTERS //
	ofVec3f	getSize() const;
	bool	isInside(const ofVec3f& a_pt, float tolerance=0.1f);
	
protected:
	
private:
	
};