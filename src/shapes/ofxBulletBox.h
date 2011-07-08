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
	
	void	init( float $sizeX, float $sizeY, float $sizeZ );
	void	init( btBoxShape* $colShape );
	
	void	create( btDiscreteDynamicsWorld* $world, ofVec3f $loc=ofVec3f(0,0,0), float $mass=1.f, float $sizeX=5.f, float $sizeY=5.f, float $sizeZ=5.f );
	void	create( btDiscreteDynamicsWorld* $world, ofVec3f $loc, ofQuaternion $rot, float $mass=1.f, float $sizeX=5.f, float $sizeY=5.f, float $sizeZ=5.f );
	void	create( btDiscreteDynamicsWorld* $world, btTransform $bt_tr, float $mass=1.f, float $sizeX=5.f, float $sizeY=5.f, float $sizeZ=5.f );
	
	void	draw();
	void	drawBox(float sizeX, float sizeY, float sizeZ);
	
	// GETTERS //
	ofVec3f	getSize() const;
	bool	isInside(const ofVec3f& $pt, float tolerance=0.1f);
	
protected:
	
private:
	
};