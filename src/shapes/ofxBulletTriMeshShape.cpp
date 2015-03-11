//
//  ofxBulletTriMeshShape.cpp
//  ofxBulletMesh
//
//  Created by Nick Hardeman on 10/10/14.
//

#include "ofxBulletTriMeshShape.h"

//--------------------------------------------------------------
ofxBulletTriMeshShape::ofxBulletTriMeshShape() {
    _type = OFX_BULLET_TRI_MESH_SHAPE;
    
    bullet_vertices     = NULL;
    bullet_indices      = NULL;
    bullet_indexVertexArrays = NULL;
}

//--------------------------------------------------------------
ofxBulletTriMeshShape::~ofxBulletTriMeshShape() {
    
}

//--------------------------------------------------------------
void ofxBulletTriMeshShape::create( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh, ofVec3f a_loc, float a_mass ) {
    btTransform tr=ofGetBtTransformFromVec3f( a_loc );
    create( a_world, aMesh, tr, a_mass );
}

//--------------------------------------------------------------
void ofxBulletTriMeshShape::create( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh, ofVec3f a_loc, ofQuaternion a_rot, float a_mass ) {
    btTransform tr	= ofGetBtTransformFromVec3f( a_loc );
	tr.setRotation( btQuaternion(btVector3(a_rot.x(), a_rot.y(), a_rot.z()), a_rot.w()) );
    create( a_world, aMesh, tr, a_mass );
}

//--------------------------------------------------------------
void ofxBulletTriMeshShape::create( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh, btTransform &a_bt_tr, float a_mass ) {
    if( aMesh.getMode() != OF_PRIMITIVE_TRIANGLES ) {
        ofLogWarning() << " ofxBulletTriMeshShape :: create : mesh must be using triangles, not creating!!" << endl;
        return;
    }
    if( aMesh.getNumIndices() < 3 ) {
        ofLogWarning() << " ofxBulletTriMeshShape :: create : mesh must have indices, not creating!" << endl;
        return;
    }
    
    
    if( !_bInited || _shape == NULL ) {
        int vertStride  = sizeof(btVector3);
        int indexStride = 3*sizeof(int);
        
        totalVerts    = (int)aMesh.getNumVertices();
        totalIndices  = (int)aMesh.getNumIndices();
        
        const int totalTriangles = totalIndices / 3;
        
        if( bullet_indices != NULL ) {
            removeShape();
        }
        if( bullet_vertices != NULL ) {
            removeShape();
        }
        if( bullet_indexVertexArrays != NULL ) {
            removeShape();
        }
        if( _shape != NULL ) {
            removeShape();
        }
        
        bullet_vertices   = new btVector3[ totalVerts ];
        bullet_indices    = new int[ totalIndices ];
        
        vector< ofVec3f >& tverts       = aMesh.getVertices();
        vector< ofIndexType >& tindices = aMesh.getIndices();
        
        for( int i = 0; i < totalVerts; i++ ) {
            bullet_vertices[i].setValue( tverts[i].x, tverts[i].y, tverts[i].z );
        }
        for( int i = 0; i < totalIndices; i++ ) {
            bullet_indices[i] = (int)tindices[i];
        }
        
        bullet_indexVertexArrays = new btTriangleIndexVertexArray(totalTriangles, bullet_indices, indexStride,
                                                                  totalVerts, (btScalar*) &bullet_vertices[0].x(), vertStride);
        
        _shape = new btBvhTriangleMeshShape( bullet_indexVertexArrays, true, true );
	}
    
    ofxBulletBaseShape::create( a_world, _shape, a_bt_tr, a_mass );
    
    
    createInternalUserData();
}

//--------------------------------------------------------------
void ofxBulletTriMeshShape::updateMesh( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh ) {
    if( aMesh.getNumVertices() != totalVerts || aMesh.getNumIndices() != totalIndices ) {
        ofLogWarning() << "updateMesh :: the verts or the indices are not the correct size, not updating";
        return;
    }
    vector< ofVec3f >& tverts       = aMesh.getVertices();
    
    btVector3 aabbMin(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
    btVector3 aabbMax(-BT_LARGE_FLOAT,-BT_LARGE_FLOAT,-BT_LARGE_FLOAT);
    
    for( int i = 0; i < totalVerts; i++ ) {
        ofVec3f& v = tverts[i];
        bullet_vertices[i].setValue( v.x, v.y, v.z );
        if( v.x < aabbMin.x() ) aabbMin.setX( v.x );
        if( v.x > aabbMax.x() ) aabbMax.setX( v.x );
        
        if( v.y < aabbMin.y() ) aabbMin.setY( v.y );
        if( v.y > aabbMax.y() ) aabbMax.setY( v.y );
        
        if( v.z < aabbMin.z() ) aabbMin.setZ( v.z );
        if( v.z > aabbMax.z() ) aabbMax.setZ( v.z );
    }
    
    btBvhTriangleMeshShape* triShape = (btBvhTriangleMeshShape*)_shape;
    triShape->partialRefitTree( aabbMin, aabbMax );
    
    //clear all contact points involving mesh proxy. Note: this is a slow/unoptimized operation.
    a_world->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs( getRigidBody()->getBroadphaseHandle(), a_world->getDispatcher());
}

//--------------------------------------------------------------
void ofxBulletTriMeshShape::removeShape() {
    if( bullet_indices != NULL ) {
        delete  bullet_indices;
        bullet_indices = NULL;
    }
    if( bullet_vertices != NULL ) {
        delete bullet_vertices;
        bullet_vertices = NULL;
    }
    if( bullet_indexVertexArrays ) {
        delete bullet_indexVertexArrays;
        bullet_indexVertexArrays = NULL;
    }
    
    if(_bColShapeCreatedInternally) {
        if(_shape) {
            delete (btBvhTriangleMeshShape *)_shape;
            _shape = NULL;
        }
    }
}






