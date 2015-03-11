//
//  ofxBulletTriMeshShape.h
//  ofxBulletMesh
//
//  Created by Nick Hardeman on 10/10/14.
//

#pragma once
#include "ofxBulletBaseShape.h"

class ofxBulletTriMeshShape : public ofxBulletBaseShape {
public:
    ofxBulletTriMeshShape();
    ~ofxBulletTriMeshShape();
    
    void create( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh, ofVec3f a_loc, float a_mass );
	void create( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh, ofVec3f a_loc, ofQuaternion a_rot, float a_mass );
	void create( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh, btTransform &a_bt_tr, float a_mass );
    
    void updateMesh( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh );
    
    void removeShape();
    
protected:
    btVector3* bullet_vertices;
    int* bullet_indices;
    btTriangleIndexVertexArray* bullet_indexVertexArrays;
    int totalVerts, totalIndices;
};






