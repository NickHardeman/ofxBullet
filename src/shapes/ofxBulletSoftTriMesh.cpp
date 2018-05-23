//
//  ofxBulletSoftTriMesh.cpp
//  ofxBulletSoftBodyMeshExample
//
//  Created by Nick Hardeman on 7/5/15.
//

#include "ofxBulletSoftTriMesh.h"

//--------------------------------------------------------------
ofxBulletSoftTriMesh::ofxBulletSoftTriMesh() : ofxBulletSoftBody() {
    _type = OFX_BULLET_SOFT_TRI_MESH;
    bullet_vertices = NULL;
}

//--------------------------------------------------------------
void ofxBulletSoftTriMesh::create( ofxBulletWorldSoft* a_world, ofMesh& aMesh, glm::vec3 a_loc, float a_mass ) {
    btTransform tr=ofGetBtTransformFromVec3f( a_loc );
    create(a_world, aMesh, tr, a_mass );
}

//--------------------------------------------------------------
void ofxBulletSoftTriMesh::create( ofxBulletWorldSoft* a_world, ofMesh& aMesh, glm::vec3 a_loc, glm::quat a_rot, float a_mass ) {
    btTransform tr = ofGetBtTransform( a_loc, a_rot );
    create(a_world, aMesh, tr, a_mass );
}

//--------------------------------------------------------------
void ofxBulletSoftTriMesh::create( ofxBulletWorldSoft* a_world, ofMesh& aMesh, btTransform &a_bt_tr, float a_mass ) {
    
    if(a_world == NULL) {
        ofLogError("ofxBulletSoftTriMesh") << "create(): a_world param is NULL";
        return;
    }
    
    if( aMesh.getMode() != OF_PRIMITIVE_TRIANGLES ) {
        ofLogError("ofxBulletSoftTriMesh") << " only excepts meshes that are triangles";
        return;
    }
    
    _world = a_world;
    
    _cachedMesh.clear();
    _cachedMesh = aMesh;
    
    if( bullet_vertices != NULL ) {
        delete bullet_vertices;
        bullet_vertices = NULL;
    }
    
    int vertStride  = sizeof(btVector3);
    int indexStride = 3*sizeof(int);
    
    int totalVerts    = (int)aMesh.getNumVertices();
    int totalIndices  = (int)aMesh.getNumIndices();
    
    bullet_vertices = new btScalar[ totalVerts * 3 ];
    int* bullet_indices = new int[ totalIndices ];
    
    auto& tverts       = aMesh.getVertices();
    vector< ofIndexType >& tindices = aMesh.getIndices();
    
    for( int i = 0; i < totalVerts; i++ ) {
        bullet_vertices[i*3+0] = tverts[i].x;
        bullet_vertices[i*3+1] = tverts[i].y;
        bullet_vertices[i*3+2] = tverts[i].z;
    }
    for( int i = 0; i < totalIndices; i++ ) {
        bullet_indices[i] = tindices[i];
    }
    
    _softBody = btSoftBodyHelpers::CreateFromTriMesh( _world->getInfo(),
                                                         bullet_vertices,
                                                         bullet_indices,
                                                         totalIndices/3 );
    _softBody->transform( a_bt_tr );
    setMass( a_mass, true );
    
    setCreated(_softBody);
    createInternalUserData();

    delete [] bullet_indices;
    
}

//--------------------------------------------------------------
void ofxBulletSoftTriMesh::draw() {
    getMesh().draw();
}

//--------------------------------------------------------------
void ofxBulletSoftTriMesh::updateMesh( ofMesh& aMesh ) {
    
    int totalNodes = getNumNodes();
    auto& tverts = aMesh.getVertices();
    
    if( _cachedMesh.getMode() == OF_PRIMITIVE_TRIANGLES ) {
        
        if( tverts.size() != totalNodes ) {
            tverts.resize( totalNodes );
        }
        
        auto& tnormals = aMesh.getNormals();
        if( aMesh.getNumNormals() != totalNodes ) {
            tnormals.resize( totalNodes );
        }
        
        for( int i = 0; i < totalNodes; i++ ) {
            tverts[i].x = _softBody->m_nodes[i].m_x.x();
            tverts[i].y = _softBody->m_nodes[i].m_x.y();
            tverts[i].z = _softBody->m_nodes[i].m_x.z();
            
            tnormals[i].x = _softBody->m_nodes[i].m_n.x();
            tnormals[i].y = _softBody->m_nodes[i].m_n.y();
            tnormals[i].z = _softBody->m_nodes[i].m_n.z();
        }
        
    }
    
    _lastMeshUpdateFrame = ofGetFrameNum();
}



