//
//  ofxBulletConvexShape.h
//  emptyExample
//
//  Created by Nick Hardeman on 3/7/13.
//
//

#pragma once

#include "ofxBulletRigidBody.h"

class ofxBulletConvexShape : public ofxBulletRigidBody {
public:
    ofxBulletConvexShape();
	~ofxBulletConvexShape();
	
	void	init( ofMesh& aMesh, glm::vec3 a_localScaling=glm::vec3(1,1,1), bool a_bUseConvexHull=false );
	void	init( btConvexHullShape* a_colShape, glm::vec3 a_centroid );
	
	void	create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc, float a_mass );
	void	create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc, glm::quat a_rot, float a_mass );
	void	create( btDiscreteDynamicsWorld* a_world, btTransform &a_bt_tr, float a_mass );
    
    void    removeShape();
	
    glm::vec3 getCentroid() const;
    
    void transformGL();
	
	// GETTERS //
	bool	isInside(const glm::vec3& a_pt, float tolerance=0.1f);
    ofMesh& getMesh() { return _cachedMesh; }
    
private:
    glm::vec3 _centroid = glm::vec3(0,0,0);
    ofMesh _cachedMesh;
};
