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
void ofxBulletSoftTriMesh::create( ofxBulletWorldSoft* a_world, ofMesh& aMesh, ofVec3f a_loc, float a_mass ) {
    btTransform tr=ofGetBtTransformFromVec3f( a_loc );
    create(a_world, aMesh, tr, a_mass );
}

//--------------------------------------------------------------
void ofxBulletSoftTriMesh::create( ofxBulletWorldSoft* a_world, ofMesh& aMesh, ofVec3f a_loc, ofQuaternion a_rot, float a_mass ) {
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
    
    bullet_vertices  = new btScalar[ totalVerts * 3 ];
    int bullet_indices[ (int)(totalIndices/3) ][3];//         = int[ totalIndices/3 ][3];
    
    vector< ofVec3f >& tverts       = aMesh.getVertices();
    vector< ofIndexType >& tindices = aMesh.getIndices();
    
    for( int i = 0; i < totalVerts; i++ ) {
        bullet_vertices[i*3+0] = tverts[i].x;
        bullet_vertices[i*3+1] = tverts[i].y;
        bullet_vertices[i*3+2] = tverts[i].z;
    }
    for( int i = 0; i < totalIndices; i += 3 ) {
        bullet_indices[i/3][0] = tindices[i+0];
        bullet_indices[i/3][1] = tindices[i+1];
        bullet_indices[i/3][2] = tindices[i+2];
    }
    
    _softBody = btSoftBodyHelpers::CreateFromTriMesh( _world->getInfo(),
                                                         bullet_vertices,
                                                         &bullet_indices[0][0],
                                                         totalIndices/3 );
    _softBody->transform( a_bt_tr );
    setMass( a_mass, true );
    
    setCreated(_softBody);
    createInternalUserData();
    
}

//--------------------------------------------------------------
void ofxBulletSoftTriMesh::draw() {
    getMesh().draw();
}

//--------------------------------------------------------------
ofVec3f	ofxBulletSoftTriMesh::getPosition() const {
    // approximation //
    btVector3& bounds = _softBody->m_bounds[0];
    btVector3& bounds2 = _softBody->m_bounds[1];
    ofVec3f tpos( (bounds.x()+bounds2.x())/2, (bounds.y()+bounds2.y())/2, (bounds.z()+bounds2.z())/2 );
    return tpos;
}

//--------------------------------------------------------------
void ofxBulletSoftTriMesh::updateMesh( ofMesh& aMesh ) {
    
    int totalNodes = getNumNodes();
    vector< ofVec3f >& tverts = aMesh.getVertices();
    
    if( _cachedMesh.getMode() == OF_PRIMITIVE_TRIANGLES ) {
        
        if( tverts.size() != totalNodes ) {
            tverts.resize( totalNodes );
        }
        
        vector< ofVec3f >& tnormals = aMesh.getNormals();
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



