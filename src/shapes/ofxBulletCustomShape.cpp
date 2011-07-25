/*
 *  ofxBulletCustomShape.cpp
 *  ofxBullet Events Example
 *
 *  Created by Nick Hardeman on 7/12/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#include "ofxBulletCustomShape.h"

//--------------------------------------------------------------
ofxBulletCustomShape::ofxBulletCustomShape() {
	
}

//--------------------------------------------------------------
ofxBulletCustomShape::~ofxBulletCustomShape() {
	
}

// pass in an already created compound shape //
//--------------------------------------------------------------
void ofxBulletCustomShape::init( btCompoundShape* $colShape, ofVec3f $centroid ) {
	_shape		= (btCollisionShape*)$colShape;
	_centroid	= $centroid;
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletCustomShape::create( btDiscreteDynamicsWorld* $world, ofVec3f $loc, float $mass ) {
	btTransform tr = ofGetBtTransformFromVec3f( $loc );
	create( $world, tr, $mass );
}

//--------------------------------------------------------------
void ofxBulletCustomShape::create( btDiscreteDynamicsWorld* $world, ofVec3f $loc, ofQuaternion $rot, float $mass ) {
	btTransform tr	= ofGetBtTransformFromVec3f( $loc );
	tr.setRotation( btQuaternion(btVector3($rot.x(), $rot.y(), $rot.z()), $rot.w()) );
	create( $world, tr, $mass );
}

//--------------------------------------------------------------
void ofxBulletCustomShape::create( btDiscreteDynamicsWorld* $world, btTransform $bt_tr, float $mass ) {
	cout << "customshape create" << endl;
	_world = $world;
	_mass = $mass;
	if(!_bInited) {
		_shape = new btCompoundShape(false);
		_centroid.set(0, 0, 0);
	}
	_startTrans		= $bt_tr;
	_bCreated		= true;
}

//--------------------------------------------------------------
bool ofxBulletCustomShape::addShape( btCollisionShape* $colShape, ofVec3f $localCentroidPos ) {
	if(_bAdded == true) {
		ofLog( OF_LOG_ERROR, "ofxBulletCustomShape :: addShape : can not call after calling add()" );
		return false;
	}
	shapes.push_back( $colShape );
	centroids.push_back( $localCentroidPos );
	return true;
}

//--------------------------------------------------------------
bool ofxBulletCustomShape::addMesh( ofMesh $mesh, ofVec3f $localScaling, bool $bUseConvexHull ) {
	if($mesh.getMode() != OF_PRIMITIVE_TRIANGLES) {
		ofLog( OF_LOG_ERROR, "ofxBulletCustomShape :: addMesh : mesh must be set to OF_PRIMITIVE_TRIANGLES!! aborting");
		return false;
	}
	if(_bAdded == true) {
		ofLog( OF_LOG_ERROR, "ofxBulletCustomShape :: addMesh : can not call after calling add()" );
		return false;
	}
	
	btVector3 localScaling( $localScaling.x, $localScaling.y, $localScaling.z );
	vector <ofIndexType>	indicies = $mesh.getIndices();
	vector <ofVec3f>		verticies = $mesh.getVertices();
	
	btVector3 centroid = btVector3(0, 0, 0);
	
	if(!$bUseConvexHull) {
		for(int i = 0; i < verticies.size(); i++) {
			btVector3 tempVec = btVector3(verticies[i].x, verticies[i].y, verticies[i].z);
			tempVec *= localScaling;
			centroid += tempVec;
		}
		centroid /= (float)verticies.size();
		
		vector<btVector3> newVerts;
		for ( int i = 0; i < indicies.size(); i++) {
			btVector3 vertex( verticies[indicies[i]].x, verticies[indicies[i]].y, verticies[indicies[i]].z);
			vertex *= localScaling;
			vertex -= centroid;
			newVerts.push_back(vertex);
		}
		
		btConvexHullShape* convexShape = new btConvexHullShape(&(newVerts[0].getX()), newVerts.size());
		convexShape->setMargin( 0.01f );
		shapes.push_back( convexShape );
		centroids.push_back( ofVec3f(centroid.getX(), centroid.getY(), centroid.getZ()) );
	} else {
		// HULL Building code from example ConvexDecompositionDemo.cpp //
		btTriangleMesh* trimesh = new btTriangleMesh();
		
		for ( int i = 0; i < indicies.size()/3; i++) {
			int index0 = indicies[i*3];
			int index1 = indicies[i*3+1];
			int index2 = indicies[i*3+2];
			
			btVector3 vertex0( verticies[index0].x, verticies[index0].y, verticies[index0].z );
			btVector3 vertex1( verticies[index1].x, verticies[index1].y, verticies[index1].z );
			btVector3 vertex2( verticies[index2].x, verticies[index2].y, verticies[index2].z );
			
			vertex0 *= localScaling;
			vertex1 *= localScaling;
			vertex2 *= localScaling;
			
			trimesh->addTriangle(vertex0, vertex1, vertex2);
		}
		
		cout << "ofxBulletCustomShape :: addMesh : input triangles = " << trimesh->getNumTriangles() << endl;
		cout << "ofxBulletCustomShape :: addMesh : input indicies = " << indicies.size() << endl;
		cout << "ofxBulletCustomShape :: addMesh : input verticies = " << verticies.size() << endl;
		
		btConvexShape* tmpConvexShape = new btConvexTriangleMeshShape(trimesh);
		
		//create a hull approximation
		btShapeHull* hull = new btShapeHull(tmpConvexShape);
		btScalar margin = tmpConvexShape->getMargin();
		hull->buildHull(margin);
		tmpConvexShape->setUserPointer(hull);
		
		centroid = btVector3(0., 0., 0.);
		for (int i = 0; i < hull->numVertices(); i++) {
			centroid += hull->getVertexPointer()[i];
		}
		centroid /= (float)hull->numVertices();
		
		printf("ofxBulletCustomShape :: addMesh : new hull numTriangles = %d\n", hull->numTriangles());
		printf("ofxBulletCustomShape :: addMesh : new hull numIndices = %d\n", hull->numIndices());
		printf("ofxBulletCustomShape :: addMesh : new hull numVertices = %d\n", hull->numVertices());
		
		btConvexHullShape* convexShape = new btConvexHullShape();
		for (int i=0;i<hull->numVertices();i++) {
			convexShape->addPoint(hull->getVertexPointer()[i] - centroid);
		}
		
		delete tmpConvexShape;
		delete hull;
		
		shapes.push_back( convexShape );
		centroids.push_back( ofVec3f(centroid.getX(), centroid.getY(), centroid.getZ()) );
	}
	return true;
}

//--------------------------------------------------------------
void ofxBulletCustomShape::add() {
	_bAdded = true;
	btTransform trans;
	trans.setIdentity();
	
	for(int i = 0; i < centroids.size(); i++) {
		_centroid += centroids[i];
	}
	if(centroids.size() > 0)
		_centroid /= (float)centroids.size();
	btVector3 shiftCentroid;
	for(int i = 0; i < shapes.size(); i++) {
		shiftCentroid = btVector3(centroids[i].x, centroids[i].y, centroids[i].z);
		shiftCentroid -= btVector3(_centroid.x, _centroid.y, _centroid.z);
		trans.setOrigin( ( shiftCentroid ) );
		((btCompoundShape*)_shape)->addChildShape( trans, shapes[i]);
	}
	_rigidBody = ofGetBtRigidBodyFromCollisionShape( _shape, _startTrans, _mass);
	setData( new ofxBulletUserData() );
	_world->addRigidBody(_rigidBody);
	setProperties(.4, .75);
	setDamping( .25 );
}

//--------------------------------------------------------------
ofVec3f ofxBulletCustomShape::getCentroid() {
	return _centroid;
}

//--------------------------------------------------------------
int ofxBulletCustomShape::getNumChildShapes() {
	if(_bAdded) {
		return ((btCompoundShape*)_shape)->getNumChildShapes();
	}
	return 0;
}

//--------------------------------------------------------------
void ofxBulletCustomShape::draw() {
	
}


