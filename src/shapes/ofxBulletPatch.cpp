//
//  ofxBulletPatch.cpp
//  ofxBullet
//
//  Created by Elias Zananiri on 2015-03-23.
//
//

#include "ofxBulletPatch.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"

//--------------------------------------------------------------
ofxBulletPatch::ofxBulletPatch() : ofxBulletSoftBody() {
    _cachedMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    _type = OFX_BULLET_SOFT_PATCH;
    _resx = 10;
    _resy = 10;
}

//--------------------------------------------------------------
void ofxBulletPatch::create(ofxBulletWorldSoft* a_world, const glm::vec3& a_p0, const glm::vec3& a_p1, const glm::vec3& a_p2, const glm::vec3& a_p3, int a_resx, int a_resy) {
    if(a_world == NULL) {
        ofLogError("ofxBulletPatch") << "create(): a_world param is NULL";
        return;
    }
    
    _resx = a_resx;
    _resy = a_resy;

	_world = a_world;
    
    _softBody = btSoftBodyHelpers::CreatePatch(_world->getInfo(), btVector3(a_p0.x, a_p0.y, a_p0.z), btVector3(a_p1.x, a_p1.y, a_p1.z), btVector3(a_p2.x, a_p2.y, a_p2.z), btVector3(a_p3.x, a_p3.y, a_p3.z), a_resx, a_resy, 0, true );
    setCreated(_softBody);
        
    
	createInternalUserData();
}

//--------------------------------------------------------------
void ofxBulletPatch::draw() {
    if(!checkCreate()) {
        ofLogWarning("ofxBulletPatch") << "draw() : must call create() first and add() after";
        return;
    }
    transformGL();
    getMesh().draw();
    restoreTransformGL();
}

//--------------------------------------------------------------
int ofxBulletPatch::getResolutionX() {
    return _resx;
}

//--------------------------------------------------------------
int ofxBulletPatch::getResolutionY() {
    return _resy;
}

//--------------------------------------------------------------
void ofxBulletPatch::updateMesh( ofMesh& aMesh ) {


    int totalNodes = getNumNodes();
    int totalFaces = getNumFaces();
    auto& tverts = aMesh.getVertices();
    
    if( _cachedMesh.getMode() == OF_PRIMITIVE_TRIANGLES ) {
        
        if( tverts.size() != totalFaces * 3 ) {
            tverts.resize( (getResolutionX()-1) * (getResolutionY()-1) * 6 );
        }
        
        auto& tnormals = aMesh.getNormals();
        if( tnormals.size() != tverts.size() ) {
            tnormals.resize( tverts.size() );
        }
        
        glm::vec3 p1, p2, p3, p4;
        glm::vec3 n1, n2, n3, n4;
        
        int ti = 0;
        for( int iy = 0; iy < getResolutionY()-1; iy++ ) {
            for( int ix = 0; ix < getResolutionX()-1; ix++ ) {
                
                int ni = iy * getResolutionX() + ix;
                p1= glm::vec3( _softBody->m_nodes[ni].m_x.x(), _softBody->m_nodes[ni].m_x.y(), _softBody->m_nodes[ni].m_x.z() );
                n1 = glm::vec3( _softBody->m_nodes[ni].m_n.x(), _softBody->m_nodes[ni].m_n.y(), _softBody->m_nodes[ni].m_n.z() );
                ni = (iy+1) * getResolutionX() + ix;
                p2 = glm::vec3( _softBody->m_nodes[ni].m_x.x(), _softBody->m_nodes[ni].m_x.y(), _softBody->m_nodes[ni].m_x.z() );
                n2 = glm::vec3( _softBody->m_nodes[ni].m_n.x(), _softBody->m_nodes[ni].m_n.y(), _softBody->m_nodes[ni].m_n.z() );
                ni = (iy+1) * getResolutionX() + ix+1;
                p3 = glm::vec3( _softBody->m_nodes[ni].m_x.x(), _softBody->m_nodes[ni].m_x.y(), _softBody->m_nodes[ni].m_x.z() );
                n3 = glm::vec3( _softBody->m_nodes[ni].m_n.x(), _softBody->m_nodes[ni].m_n.y(), _softBody->m_nodes[ni].m_n.z() );
                ni = (iy) * getResolutionX() + ix+1;
                p4 = glm::vec3( _softBody->m_nodes[ni].m_x.x(), _softBody->m_nodes[ni].m_x.y(), _softBody->m_nodes[ni].m_x.z() );
                n4 = glm::vec3( _softBody->m_nodes[ni].m_n.x(), _softBody->m_nodes[ni].m_n.y(), _softBody->m_nodes[ni].m_n.z() );
                
                tverts[ti] = p1;
                tnormals[ti] = n1;
                ti += 1;
                
//                ni = (iy+1) * getResolutionX() + ix;
                tverts[ti] = p2;
                tnormals[ti] = n2;
                ti += 1;
                
//                ni = (iy+1) * getResolutionX() + ix+1;
                tverts[ti] = p3;
                tnormals[ti] = n3;
                ti += 1;
                
                
                
                //
//                ni = (iy+1) * getResolutionX() + ix+1;
                tverts[ti] = p3;
                tnormals[ti] = n3;
                ti += 1;
                
//                ni = (iy) * getResolutionX() + ix+1;
                tverts[ti] = p4;
                tnormals[ti] = n4;
                ti += 1;
                
//                ni = (iy) * getResolutionX() + ix;
                tverts[ti] = p1;
                tnormals[ti] = n1;
                ti += 1;
            }
            
        }
        
    }
    
    _lastMeshUpdateFrame = ofGetFrameNum();
}

//--------------------------------------------------------------
void ofxBulletPatch::updateMeshTexCoords( ofMesh& aMesh ) {

    auto& tcoords = getMesh().getTexCoords();
    int numVerts = getMesh().getVertices().size();
    if( tcoords.size() != numVerts ) {
        tcoords.resize( numVerts );
    }
    
    int tz = 0;
    for( int iy = 0; iy < _resy-1; iy++ ) {
        for( int ix = 0; ix < _resx-1; ix++ ) {
            
//            const bool	mdx=(ix+1)<resx;
//			const bool	mdy=(iy+1)<resy;
            
            
//            if(mdx&&mdy) {
            
                // psb->appendFace(node00,node10,node11);
                int ti = tz + 0;
                //                tcoords[ti].x = btSoftBodyHelpers::CalculateUV(resx,resy,ix,iy,0);
                //                tcoords[ti].y = btSoftBodyHelpers::CalculateUV(resx,resy,ix,iy,1);
                tcoords[ti].x = ofMap( ix, 0, _resx-1, 0, 1, true );
                tcoords[ti].y = ofMap( iy, 0, _resy-1, 0, 1, true );
                
                
                ti = tz + 1;
                //                tcoords[ti].x = btSoftBodyHelpers::CalculateUV(resx,resy,ix,iy,0);
                //                tcoords[ti].y = btSoftBodyHelpers::CalculateUV(resx,resy,ix,iy,2);
                tcoords[ti].x = ofMap( ix, 0, _resx-1, 0, 1, true );
                tcoords[ti].y = ofMap( iy+1, 0, _resy-1, 0, 1, true );
                
                ti = tz + 2;
                //                tcoords[ti].x = btSoftBodyHelpers::CalculateUV(resx,resy,ix,iy,3);
                //                tcoords[ti].y = btSoftBodyHelpers::CalculateUV(resx,resy,ix,iy,2);
                tcoords[ti].x = ofMap( ix+1, 0, _resx-1, 0, 1, true );
                tcoords[ti].y = ofMap( iy+1, 0, _resy-1, 0, 1, true );
                
                
                // psb->appendFace(node11,node01,node00);
                ti = tz + 3;
                //                tcoords[ti ].x = btSoftBodyHelpers::CalculateUV(resx,resy,ix,iy,3);
                //                tcoords[ti ].y = btSoftBodyHelpers::CalculateUV(resx,resy,ix,iy,2);
                tcoords[ti].x = ofMap( ix+1, 0, _resx-1, 0, 1, true );
                tcoords[ti].y = ofMap( iy+1, 0, _resy-1, 0, 1, true );
                //
                ti = tz + 4;
                //                tcoords[ti ].x = btSoftBodyHelpers::CalculateUV(resx,resy,ix,iy,3);
                //                tcoords[ti ].y = btSoftBodyHelpers::CalculateUV(resx,resy,ix,iy,1);
                tcoords[ti].x = ofMap( ix+1, 0, _resx-1, 0, 1, true );
                tcoords[ti].y = ofMap( iy, 0, _resy-1, 0, 1, true );
                //
                ti = tz + 5;
                tcoords[ti].x = ofMap( ix, 0, _resx-1, 0, 1, true );
                tcoords[ti].y = ofMap( iy, 0, _resy-1, 0, 1, true );
                //                tcoords[ti].x = btSoftBodyHelpers::CalculateUV(resx,resy,ix,iy,0);
                //                tcoords[ti].y = btSoftBodyHelpers::CalculateUV(resx,resy,ix,iy,1);
                
                //            tz += 12;
                tz += 6;
//            }
            
        }
    }
}





