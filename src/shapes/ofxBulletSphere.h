/*
 *  ofxBulletSphere.h
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/18/11.
 *
 */

#pragma once

#include "ofxBulletRigidBody.h"
#include "ofVbo.h"

class ofxBulletSphere : public ofxBulletRigidBody {
public:
	ofxBulletSphere();
	~ofxBulletSphere();
	
	void	init( float a_radius );
	void	init( btSphereShape* a_colShape );
	
	void	create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc=glm::vec3(0,0,0), float a_mass=1.f, float a_radius=1.f );
	void	create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc, glm::quat a_rot, float a_mass=1.f, float a_radius=1.f );
	void	create( btDiscreteDynamicsWorld* a_world, btTransform &a_bt_tr, float a_mass=1.f, float a_radius=1.f );
    
    void    removeShape();
	
	void	draw();
	
	// GETTERS //
	float	getRadius();
    
protected:
	
private:
	
	
};
