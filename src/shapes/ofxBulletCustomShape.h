/*
 *  ofxBulletCustomShape.h
 *  ofxBullet Events Example
 *
 *  Created by Nick Hardeman on 7/12/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#pragma once

#include "ofxBulletBaseShape.h"
#include "btShapeHull.h"

class ofxBulletCustomShape : public ofxBulletBaseShape {
public:
	ofxBulletCustomShape();
	~ofxBulletCustomShape();
	
	void init( btCompoundShape* $colShape, ofVec3f $centroid );
	
	void create( btDiscreteDynamicsWorld* $world, ofVec3f $loc=ofVec3f(0,0,0), float $mass=1.f );
	void create( btDiscreteDynamicsWorld* $world, ofVec3f $loc, ofQuaternion $rot, float $mass=1.f );
	void create( btDiscreteDynamicsWorld* $world, btTransform $bt_tr, float $mass=1.f );
	
	bool addShape( btCollisionShape* $colShape, ofVec3f $localCentroidPos );
	bool addMesh(ofMesh $mesh, ofVec3f $localScaling, bool $bUseConvexHull);
	void add();
	
	ofVec3f getCentroid();
	int		getNumChildShapes();
	
	void draw();
	
protected:
	btTransform				_startTrans;
	ofVec3f					_centroid;
	vector<ofVec3f>			centroids;
	vector<btCollisionShape*> shapes;
};