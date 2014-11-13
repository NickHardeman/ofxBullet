/*
 *  ofxBulletMousePickEvent.h
 *  ofxBullet_v7_joints
 *
 *  Created by Nick Hardeman on 7/6/11.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBulletRaycastData.h"

class ofxBulletMousePickEvent : public ofxBulletRaycastData {
public:
	ofxBulletMousePickEvent() {}
	~ofxBulletMousePickEvent() {}
	
	void setRaycastData( ofxBulletRaycastData& a_raycastdata ) {
		bHasHit			= a_raycastdata.bHasHit;
		body			= a_raycastdata.body;
		userData		= a_raycastdata.userData;
		rayWorldPos		= a_raycastdata.rayWorldPos;
		rayScreenPos	= a_raycastdata.rayScreenPos;
		pickPosWorld	= a_raycastdata.pickPosWorld;
		localPivotPos	= a_raycastdata.localPivotPos;
	}
};