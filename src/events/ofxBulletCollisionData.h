/*
 *  CollisionData.h
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/25/11.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBulletUserData.h"

class ofxBulletCollisionData {
public:
	int numContactPoints;
	ofxBulletUserData* userData1;
	vector<ofVec3f>	worldContactPoints1;
	const btRigidBody* body1;
	
	ofxBulletUserData* userData2;
	vector<ofVec3f>	worldContactPoints2;
	vector<ofVec3f> normalsOnShape2;
	const btRigidBody* body2;
};