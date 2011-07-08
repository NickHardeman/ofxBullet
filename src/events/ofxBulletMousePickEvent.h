/*
 *  ofxBulletMousePickEvent.h
 *  ofxBullet_v7_joints
 *
 *  Created by Nick Hardeman on 7/6/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBulletBaseShape.h"

class ofxBulletMousePickEvent {
public:
	ofxBulletUserData* userData;
	// location in the bullet world //
	ofVec3f	mouseWorldPos;
	// location of the mouse on the screen, z will always be 0 //
	ofVec3f mouseScreenPos;
	// location of the pick position in the world coords //
	ofVec3f pickPosWorld;
	// local point in rigid body coords //
	ofVec3f localPivotPos;
};