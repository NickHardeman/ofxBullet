/*
 *  ofxBulletCustomShape.h
 *  ofxBullet Events Example
 *
 *  Created by Nick Hardeman on 7/12/11.
 *
 */

#pragma once

#include "ofxBulletBaseShape.h"
#include "btShapeHull.h"

class ofxBulletCustomShape : public ofxBulletBaseShape {
public:
	ofxBulletCustomShape();
	~ofxBulletCustomShape();
	
	void init( btCompoundShape* a_colShape, ofVec3f a_centroid );
	
	void create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc=ofVec3f(0,0,0), float a_mass=1.f );
	void create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, ofQuaternion a_rot, float a_mass=1.f );
	void create( btDiscreteDynamicsWorld* a_world, btTransform a_bt_tr, float a_mass=1.f );
    
    void removeShape();
	
	bool addShape( btCollisionShape* a_colShape, ofVec3f a_localCentroidPos );
	bool addMesh(ofMesh a_mesh, ofVec3f a_localScaling, bool a_bUseConvexHull);
	void add();
	
	ofVec3f getCentroid();
	int		getNumChildShapes();
	
	void draw();
    void transformGL();
	
protected:
	btTransform				_startTrans;
	ofVec3f					_centroid;
	vector<ofVec3f>			centroids;
	vector<btCollisionShape*> shapes;
};