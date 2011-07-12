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
#include "ofxBulletRaycastData.h"
#include "ofxBulletBaseShape.h"

class ofxBulletMousePickEvent : public ofxBulletRaycastData {
public:
	ofxBulletMousePickEvent() {}
	~ofxBulletMousePickEvent() {}
	
	void setRaycastData( ofxBulletRaycastData& $raycastdata ) {
		bHasHit			= $raycastdata.bHasHit;
		body			= $raycastdata.body;
		userData		= $raycastdata.userData;
		rayWorldPos		= $raycastdata.rayWorldPos;
		rayScreenPos	= $raycastdata.rayScreenPos;
		pickPosWorld	= $raycastdata.pickPosWorld;
		localPivotPos	= $raycastdata.localPivotPos;
	}
};