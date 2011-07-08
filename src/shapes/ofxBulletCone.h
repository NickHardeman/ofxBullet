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
	
	void	init(float $radius, float $height);
	void	init( btConeShape* $colShape );
	
	void	create( btDiscreteDynamicsWorld* $world, ofVec3f $loc=ofVec3f(0,0,0), float $mass=1.f, float $radius=1.f, float $height=2.f );
	void	create( btDiscreteDynamicsWorld* $world, ofVec3f $loc, ofQuaternion $rot, float $mass=1.f, float $radius=1.f, float $height=2.f );
	void	create( btDiscreteDynamicsWorld* $world, btTransform $bt_tr, float $mass=1.f, float $radius=1.f, float $height=2.f );
	
	// GETTERS //
	float getHeight();
	float getRadius();
	int getUpIndex();
	
	void	draw();
protected:
	
private:

};