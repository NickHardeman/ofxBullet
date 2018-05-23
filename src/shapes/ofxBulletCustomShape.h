/*
 *  ofxBulletCustomShape.h
 *  ofxBullet Events Example
 *
 *  Created by Nick Hardeman on 7/12/11.
 *
 */

#pragma once

#include "ofxBulletRigidBody.h"
#include "btShapeHull.h"

class ofxBulletCustomShape : public ofxBulletRigidBody {
public:
	ofxBulletCustomShape();
	~ofxBulletCustomShape();
	
	void init( btCompoundShape* a_colShape, glm::vec3 a_centroid );
	
	void create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc=glm::vec3(0,0,0), float a_mass=1.f );
	void create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc, glm::quat a_rot, float a_mass=1.f );
	void create( btDiscreteDynamicsWorld* a_world, btTransform &a_bt_tr, float a_mass=1.f );
    
    void removeShape();
	
	bool addShape( btCollisionShape* a_colShape, glm::vec3 a_localCentroidPos );
	bool addMesh(ofMesh a_mesh, glm::vec3 a_localScaling, bool a_bUseConvexHull);
	void add();
	
	glm::vec3 getCentroid();
	int		getNumChildShapes();
	
	void draw();
    void transformGL();
	
protected:
	btTransform				_startTrans;
	glm::vec3				_centroid;
	vector<glm::vec3>		centroids;
	vector<btCollisionShape*> shapes;
};
