/*
 *  ofxBulletCustomShape.cpp
 *  ofxBullet Events Example
 *
 *  Created by Nick Hardeman on 7/12/11.
 *
 */

#include "ofxBulletCustomShape.h"

//--------------------------------------------------------------
ofxBulletCustomShape::ofxBulletCustomShape() : ofxBulletRigidBody() {
	_type = OFX_BULLET_CUSTOM_SHAPE;
}

//--------------------------------------------------------------
ofxBulletCustomShape::~ofxBulletCustomShape() {
	
}

// pass in an already created compound shape //
//--------------------------------------------------------------
void ofxBulletCustomShape::init( btCompoundShape* a_colShape, glm::vec3 a_centroid ) {
	_shape		= (btCollisionShape*)a_colShape;
	_centroid	= a_centroid;
	_bInited	= true;
    // shape passed in externally, so not responsible for deleteing pointer
    _bColShapeCreatedInternally = false;
}

//--------------------------------------------------------------
void ofxBulletCustomShape::create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc, float a_mass ) {
	btTransform tr = ofGetBtTransformFromVec3f( a_loc );
	create( a_world, tr, a_mass );
}

//--------------------------------------------------------------
void ofxBulletCustomShape::create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc, glm::quat a_rot, float a_mass ) {
	btTransform tr	= ofGetBtTransformFromVec3f( a_loc );
	tr.setRotation( btQuaternion( a_rot.x, a_rot.y, a_rot.z, a_rot.w ));
	create( a_world, tr, a_mass );
}

//--------------------------------------------------------------
void ofxBulletCustomShape::create( btDiscreteDynamicsWorld* a_world, btTransform &a_bt_tr, float a_mass ) {
	_world = a_world;
	_mass = a_mass;
	if(!_bInited) {
		_shape = new btCompoundShape(true);
        _centroid = glm::vec3(0, 0, 0);
	}
	_startTrans		= a_bt_tr;
	_bCreated		= true;
}

//--------------------------------------------------------------
void ofxBulletCustomShape::removeShape() {
    if(_bColShapeCreatedInternally) {
        if(_shape) {
            delete (btCompoundShape *)_shape;
            _shape = NULL;
        }
    }
}

//--------------------------------------------------------------
bool ofxBulletCustomShape::addShape( btCollisionShape* a_colShape, glm::vec3 a_localCentroidPos ) {
	if(_bAdded == true) {
		ofLog( OF_LOG_ERROR, "ofxBulletCustomShape :: addShape : can not call after calling add()" );
		return false;
	}
	shapes.push_back( a_colShape );
	centroids.push_back( a_localCentroidPos );
	return true;
}

//--------------------------------------------------------------
bool ofxBulletCustomShape::addMesh( ofMesh a_mesh, glm::vec3 a_localScaling, bool a_bUseConvexHull ) {
	if(a_mesh.getMode() != OF_PRIMITIVE_TRIANGLES) {
		ofLog( OF_LOG_ERROR, "ofxBulletCustomShape :: addMesh : mesh must be set to OF_PRIMITIVE_TRIANGLES!! aborting");
		return false;
	}
	if(_bAdded == true) {
		ofLog( OF_LOG_ERROR, "ofxBulletCustomShape :: addMesh : can not call after calling add()" );
		return false;
	}
	
	btVector3 localScaling( a_localScaling.x, a_localScaling.y, a_localScaling.z );
	auto indicies = a_mesh.getIndices();
	auto verticies = a_mesh.getVertices();
    
	btVector3 centroid = btVector3(0, 0, 0);
	
	if(!a_bUseConvexHull) {
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
		centroids.push_back( glm::vec3(centroid.getX(), centroid.getY(), centroid.getZ()) );
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
		
		//cout << "ofxBulletCustomShape :: addMesh : input triangles = " << trimesh->getNumTriangles() << endl;
		//cout << "ofxBulletCustomShape :: addMesh : input indicies = " << indicies.size() << endl;
		//cout << "ofxBulletCustomShape :: addMesh : input verticies = " << verticies.size() << endl;
		
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
		
		//printf("ofxBulletCustomShape :: addMesh : new hull numTriangles = %d\n", hull->numTriangles());
		//printf("ofxBulletCustomShape :: addMesh : new hull numIndices = %d\n", hull->numIndices());
		//printf("ofxBulletCustomShape :: addMesh : new hull numVertices = %d\n", hull->numVertices());
		
		btConvexHullShape* convexShape = new btConvexHullShape();
		for (int i=0;i<hull->numVertices();i++) {
			convexShape->addPoint(hull->getVertexPointer()[i] - centroid);
		}
		
		delete tmpConvexShape;
		delete hull;
		
		shapes.push_back( convexShape );
		centroids.push_back( glm::vec3(centroid.getX(), centroid.getY(), centroid.getZ()) );
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
    setCreated(_rigidBody);
	createInternalUserData();
	_world->addRigidBody(_rigidBody);
	setProperties(.4, .75);
	setDamping( .25 );
}

//--------------------------------------------------------------
glm::vec3 ofxBulletCustomShape::getCentroid() {
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

//--------------------------------------------------------------
void ofxBulletCustomShape::transformGL() {
    ofxBulletBaseShape::transformGL();
    ofTranslate(-getCentroid().x, -getCentroid().y, -getCentroid().z);
}




