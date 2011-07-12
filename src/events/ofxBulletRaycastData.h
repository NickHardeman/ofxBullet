/*
 *  ofxBulletRaycastData.h
 *  ofxBullet Events Example
 *
 *  Created by Nick Hardeman on 7/12/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBulletBaseShape.h"

class ofxBulletRaycastData {
public:
	ofxBulletRaycastData() {
		body		= NULL;
		userData	= NULL;
	}
	~ofxBulletRaycastData() {};
	
	bool bHasHit;
	// body that it hit //
	btRigidBody* body;
	ofxBulletUserData* userData;
	// location in the bullet world //
	ofVec3f	rayWorldPos;
	// location of the mouse on the screen, z will always be 0 //
	ofVec3f rayScreenPos;
	// location of the pick position in the world coords //
	ofVec3f pickPosWorld;
	// local point in rigid body coords //
	ofVec3f localPivotPos;
};