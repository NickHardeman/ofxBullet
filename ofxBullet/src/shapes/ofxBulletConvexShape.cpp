//
//  ofxBulletConvexShape.cpp
//  emptyExample
//
//  Created by Nick Hardeman on 3/7/13.
//
//

#include "ofxBulletConvexShape.h"
#include "btShapeHull.h"

//--------------------------------------------------------------
ofxBulletConvexShape::ofxBulletConvexShape() {
    _type = OFX_BULLET_CONVEX_SHAPE;
}

//--------------------------------------------------------------
ofxBulletConvexShape::~ofxBulletConvexShape() {
    
}

//--------------------------------------------------------------
void ofxBulletConvexShape::init( ofMesh& aMesh, ofVec3f a_localScaling, bool a_bUseConvexHull ) {
    _centroid.zero();
    btVector3 centroid = btVector3(0, 0, 0);
    btVector3 localScaling( a_localScaling.x, a_localScaling.y, a_localScaling.z );
    
    vector <ofIndexType>	indicies    = aMesh.getIndices();
	vector <ofVec3f>		verticies   = aMesh.getVertices();
	
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
        _shape = convexShape;
        _centroid = ofVec3f(centroid.getX(), centroid.getY(), centroid.getZ() );
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
		
		btConvexHullShape* convexShape = new btConvexHullShape();
		for (int i=0;i<hull->numVertices();i++) {
			convexShape->addPoint(hull->getVertexPointer()[i] - centroid);
		}
		
		delete tmpConvexShape;
		delete hull;
		
        _shape = convexShape;
        _centroid = ofVec3f(centroid.getX(), centroid.getY(), centroid.getZ() );
	}
    
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletConvexShape::init( btConvexHullShape* a_colShape, ofVec3f a_centroid ) {
    _centroid   = a_centroid;
    _shape		= (btCollisionShape*)a_colShape;
	_bInited	= true;
    // shape passed in externally, so not responsible for deleteing pointer
    _bColShapeCreatedInternally = false;
}

//--------------------------------------------------------------
void ofxBulletConvexShape::create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, float a_mass) {
	create( a_world, ofGetBtTransformFromVec3f( a_loc ), a_mass );
}

//--------------------------------------------------------------
void ofxBulletConvexShape::create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, ofQuaternion a_rot, float a_mass ) {
	btTransform tr	= ofGetBtTransformFromVec3f( a_loc );
	tr.setRotation( btQuaternion(btVector3(a_rot.x(), a_rot.y(), a_rot.z()), a_rot.w()) );
	create( a_world, tr, a_mass);
}

//--------------------------------------------------------------
void ofxBulletConvexShape::create( btDiscreteDynamicsWorld* a_world, btTransform const& a_bt_tr, float a_mass ) {
	if(!_bInited || _shape == NULL) {
		ofLogWarning("ofxBulletConvexShape :: create : must call init first");
        return;
	}
    ofxBulletBaseShape::create( a_world, _shape, a_bt_tr, a_mass );
	createInternalUserData();
}

//--------------------------------------------------------------
ofVec3f ofxBulletConvexShape::getCentroid() const {
    return _centroid;
}

//--------------------------------------------------------------
void ofxBulletConvexShape::transformGL() {
    ofxBulletBaseShape::transformGL();
    glTranslatef(-getCentroid().x, -getCentroid().y, -getCentroid().z);
}

//--------------------------------------------------------------
bool ofxBulletConvexShape::isInside(const ofVec3f& a_pt, float tolerance) {
    if(_shape != NULL)
        return ((btConvexHullShape*)_shape)->isInside( btVector3(a_pt.x, a_pt.y, a_pt.z), tolerance);
}





