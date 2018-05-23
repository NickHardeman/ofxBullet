/*
 *  ofxBulletRaycastData.h
 *  ofxBullet Events Example
 *
 *  Created by Nick Hardeman on 7/12/11.
 *
 */

#pragma once

#include "ofMain.h"

class ofxBulletRaycastData {
public:
	ofxBulletRaycastData() {
		body		= NULL;
		userData	= NULL;
        bHasHit     = false;
	}
	~ofxBulletRaycastData() {};
	
	bool bHasHit;
	// body that it hit //
	btRigidBody* body;
	ofxBulletUserData* userData;
	// location in the bullet world //
	glm::vec3	rayWorldPos;
	// location of the mouse on the screen, z will always be 0 //
	glm::vec3 rayScreenPos;
	// location of the pick position in the world coords //
	glm::vec3 pickPosWorld;
	// local point in rigid body coords //
	glm::vec3 localPivotPos;
};
