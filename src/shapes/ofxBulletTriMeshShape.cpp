//
//  ofxBulletTriMeshShape.cpp
//  ofxBulletMesh
//
//  Created by Nick Hardeman on 10/10/14.
//

#include "ofxBulletTriMeshShape.h"

//--------------------------------------------------------------
ofxBulletTriMeshShape::ofxBulletTriMeshShape() : ofxBulletRigidBody() {
    _type = OFX_BULLET_TRI_MESH_SHAPE;
    
    bullet_vertices     = NULL;
    bullet_indices      = NULL;
    bullet_indexVertexArrays = NULL;
}

//--------------------------------------------------------------
ofxBulletTriMeshShape::~ofxBulletTriMeshShape() {
    
}

//--------------------------------------------------------------
void ofxBulletTriMeshShape::create( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh, glm::vec3 a_loc, float a_mass, glm::vec3 aAAbbMin, glm::vec3 aAAbbMax ) {
    btTransform tr=ofGetBtTransformFromVec3f( a_loc );
    create( a_world, aMesh, tr, a_mass );
}

//--------------------------------------------------------------
void ofxBulletTriMeshShape::create( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh, glm::vec3 a_loc, glm::quat a_rot, float a_mass, glm::vec3 aAAbbMin, glm::vec3 aAAbbMax ) {
    btTransform tr	= ofGetBtTransformFromVec3f( a_loc );
	tr.setRotation( btQuaternion( a_rot.x, a_rot.y, a_rot.z, a_rot.w ));
    create( a_world, aMesh, tr, a_mass );
}

//--------------------------------------------------------------
void ofxBulletTriMeshShape::create( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh, btTransform &a_bt_tr, float a_mass, glm::vec3 aAAbbMin, glm::vec3 aAAbbMax ) {
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
        
        auto& tverts = aMesh.getVertices();
        auto& tindices = aMesh.getIndices();
        
        for( int i = 0; i < totalVerts; i++ ) {
            bullet_vertices[i].setValue( tverts[i].x, tverts[i].y, tverts[i].z );
        }
        for( int i = 0; i < totalIndices; i++ ) {
            bullet_indices[i] = (int)tindices[i];
        }
        
        bullet_indexVertexArrays = new btTriangleIndexVertexArray(totalTriangles, bullet_indices, indexStride,
                                                                  totalVerts, (btScalar*) &bullet_vertices[0].x(), vertStride);
        
        
//        if you are having trouble with objects falling through, try passing in smaller or larger aabbMin and aabbMax
//        to something closer to the size of your object //
//        btVector3 aabbMin(-10000,-10000,-10000),aabbMax(10000,10000,10000);
        if( aAAbbMin.length() > 0 && aAAbbMax.length() > 0 ) {
            btVector3 aabbMin( aAAbbMin.x, aAAbbMin.y, aAAbbMin.z );
            btVector3 aabbMax( aAAbbMax.x, aAAbbMax.y, aAAbbMax.z );
            _shape  = new btBvhTriangleMeshShape(bullet_indexVertexArrays, true, aabbMin, aabbMax );
        } else {
            _shape  = new btBvhTriangleMeshShape(bullet_indexVertexArrays, true, true );
        }
	}
    
    ofxBulletRigidBody::create( a_world, _shape, a_bt_tr, a_mass );
    
    
    createInternalUserData();
    updateMesh( a_world, aMesh );
}

//--------------------------------------------------------------
void ofxBulletTriMeshShape::updateMesh( btDiscreteDynamicsWorld* a_world, ofMesh& aMesh ) {
    if( aMesh.getNumVertices() != totalVerts || aMesh.getNumIndices() != totalIndices ) {
        ofLogWarning() << "updateMesh :: the verts or the indices are not the correct size, not updating";
        return;
    }
    
    auto& tverts = aMesh.getVertices();
    
    btVector3 aabbMin(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
    btVector3 aabbMax(-BT_LARGE_FLOAT,-BT_LARGE_FLOAT,-BT_LARGE_FLOAT);
    
    for( int i = 0; i < totalVerts; i++ ) {
        auto& v = tverts[i];
        bullet_vertices[i].setValue( v.x, v.y, v.z );
        
        aabbMin.setMin( bullet_vertices[i] );
        aabbMax.setMax( bullet_vertices[i] );
    }
    
    btBvhTriangleMeshShape* triShape = (btBvhTriangleMeshShape*)_shape;
//    triShape->partialRefitTree( aabbMin, aabbMax );
    triShape->refitTree( aabbMin, aabbMax );
    
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






