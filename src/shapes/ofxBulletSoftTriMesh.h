//
//  ofxBulletSoftTriMesh.h
//  ofxBulletSoftBodyMeshExample
//
//  Created by Nick Hardeman on 7/5/15.
//

#pragma once

#include "ofxBulletSoftBody.h"

class ofxBulletSoftTriMesh : public ofxBulletSoftBody {
public:
    ofxBulletSoftTriMesh();
    
    void create( ofxBulletWorldSoft* a_world, ofMesh& aMesh, ofVec3f a_loc, float a_mass );
	void create( ofxBulletWorldSoft* a_world, ofMesh& aMesh, ofVec3f a_loc, ofQuaternion a_rot, float a_mass );
	void create( ofxBulletWorldSoft* a_world, ofMesh& aMesh, btTransform &a_bt_tr, float a_mass );
    
    void updateCacheMesh();
    void draw();
    
    ofMesh& getMesh();
    ofVec3f	getPosition() const;
    
protected:
    ofMesh cachedMesh;
    btScalar* bullet_vertices;
};
