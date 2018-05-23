/*
 *  ofxBulletCapsule.h
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/27/11.
 *
 */


#pragma once

#include "ofxBulletRigidBody.h"
#include "btShapeHull.h"

class ofxBulletCapsule : public ofxBulletRigidBody {
public:
	ofxBulletCapsule();
	~ofxBulletCapsule();
	
	void	init(float a_radius, float a_height);
	void	init( btCapsuleShape* a_colShape );
	
	void	create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc=glm::vec3(0,0,0), float a_mass=1.f, float a_radius=1.f, float a_height=10.f );
	// angles of Quaternion should be in Radians //
	void	create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc=glm::vec3(0,0,0), glm::quat a_rot=glm::quat(0, 1, 0, 1),
				   float a_mass=1.f, float a_radius=1.f, float a_height=10.f );
	void	create( btDiscreteDynamicsWorld* a_world, btTransform &a_bt_tr, float a_mass=1.f, float a_radius=1.f, float a_height=10.f );
    
    void    removeShape();
	
	void	draw();
	
    float   getRadius();
    float   getHeight();
    
protected:
	btShapeHull* hull;
    ofMesh _cachedMesh;
	
};
