//
//  ofxBulletConvexShape.h
//  emptyExample
//
//  Created by Nick Hardeman on 3/7/13.
//
//

#pragma once
#include "ofxBulletBaseShape.h"

class ofxBulletConvexShape : public ofxBulletBaseShape {
public:
    ofxBulletConvexShape();
	~ofxBulletConvexShape();
	
	void	init( ofMesh& aMesh, ofVec3f a_localScaling=ofVec3f(1,1,1), bool a_bUseConvexHull=false );
	void	init( btConvexHullShape* a_colShape, ofVec3f a_centroid );
	
	void	create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, float a_mass );
	void	create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, ofQuaternion a_rot, float a_mass );
	void	create( btDiscreteDynamicsWorld* a_world, btTransform a_bt_tr, float a_mass );
	
    ofVec3f getCentroid() const;
//	void	draw();
    void transformGL();
	
	// GETTERS //
	bool	isInside(const ofVec3f& a_pt, float tolerance=0.1f);
    
private:
    ofVec3f _centroid;
};