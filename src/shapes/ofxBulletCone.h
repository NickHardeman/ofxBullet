/*
 *  ofxBulletCone.h
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/24/11.
 *
 */

#pragma once

#include "ofxBulletRigidBody.h"
#include "ofMain.h"
#include "ofVbo.h"

class ofxBulletCone : public ofxBulletRigidBody {
public:
	ofxBulletCone();
	~ofxBulletCone();
	
	void	init(float a_radius, float a_height);
	void	init( btConeShape* a_colShape );
	
	void	create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc=glm::vec3(0,0,0), float a_mass=1.f, float a_radius=1.f, float a_height=2.f );
	void	create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc, glm::quat a_rot, float a_mass=1.f, float a_radius=1.f, float a_height=2.f );
	void	create( btDiscreteDynamicsWorld* a_world, btTransform &a_bt_tr, float a_mass=1.f, float a_radius=1.f, float a_height=2.f );
    
    void    removeShape();
	
	// GETTERS //
	float getHeight();
	float getRadius();
	int getUpIndex();
	
	void	draw();
protected:
	
private:

};
