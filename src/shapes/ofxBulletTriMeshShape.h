//
//  ofxBulletTriMeshShape.h
//  ofxBulletMesh
//
//  Created by Nick Hardeman on 10/10/14.
//

#pragma once

#include "ofxBulletRigidBody.h"

class ofxBulletTriMeshShape : public ofxBulletRigidBody {
public:
    ofxBulletTriMeshShape();
    ~ofxBulletTriMeshShape();
    
    void create( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh, glm::vec3 a_loc, float a_mass,
                glm::vec3 aAAbbMin=glm::vec3(-100,-100,-100), glm::vec3 aAAbbMax=glm::vec3(100,100,100) );
	void create( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh, glm::vec3 a_loc, glm::quat a_rot, float a_mass,
                glm::vec3 aAAbbMin=glm::vec3(-100,-100,-100), glm::vec3 aAAbbMax=glm::vec3(100,100,100) );
	void create( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh, btTransform &a_bt_tr, float a_mass,
                glm::vec3 aAAbbMin=glm::vec3(-100,-100,-100), glm::vec3 aAAbbMax=glm::vec3(100,100,100) );
    
    void updateMesh( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh );
    
    void removeShape();
    
protected:
    btVector3* bullet_vertices;
    int* bullet_indices;
    btTriangleIndexVertexArray* bullet_indexVertexArrays;
    int totalVerts, totalIndices;
};






