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
    _lastUpdateFrame = 0;
    
    _cachedMesh.setMode(OF_PRIMITIVE_TRIANGLES);
}

//--------------------------------------------------------------
void ofxBulletPatch::create(ofxBulletWorldSoft* a_world, const ofVec3f& a_p0, const ofVec3f& a_p1, const ofVec3f& a_p2, const ofVec3f& a_p3, int a_resx, int a_resy) {
    if(a_world == NULL) {
        ofLogError("ofxBulletPatch") << "create(): a_world param is NULL";
        return;
    }

	_world = a_world;
    
    _softBody = btSoftBodyHelpers::CreatePatch(_world->getInfo(), btVector3(a_p0.x, a_p0.y, a_p0.z), btVector3(a_p1.x, a_p1.y, a_p1.z), btVector3(a_p2.x, a_p2.y, a_p2.z), btVector3(a_p3.x, a_p3.y, a_p3.z), a_resx, a_resy, 0, true);
    setCreated(_softBody);
    
    // Add the initial vertices and indices to the mesh.
    //for (int i = 0; i < getNumNodes(); i++) {
    //    _cachedMesh.addVertex(getNodePos(i));
    //}
        
    _type = OFX_BULLET_SOFT_PATCH;
}

//--------------------------------------------------------------
void ofxBulletPatch::update() {
    if (_lastUpdateFrame == ofGetFrameNum()) return;
    
    // Build the mesh.
	_cachedMesh.clear();
	for (int i = 0; i < getNumFaces(); ++i) {
		for (int j = 0; j < 3; ++j) {
			_cachedMesh.addVertex(ofVec3f(_softBody->m_faces.at(i).m_n[j]->m_x.x(), 
										  _softBody->m_faces.at(i).m_n[j]->m_x.y(), 
										  _softBody->m_faces.at(i).m_n[j]->m_x.z()));
			_cachedMesh.addNormal(ofVec3f(_softBody->m_faces.at(i).m_n[j]->m_n.x(), 
										  _softBody->m_faces.at(i).m_n[j]->m_n.y(), 
										  _softBody->m_faces.at(i).m_n[j]->m_n.z()));
		}
    }
    
    _lastUpdateFrame = ofGetFrameNum();
}

//--------------------------------------------------------------
void ofxBulletPatch::draw() {
    if(!checkCreate()) {
        ofLogWarning("ofxBulletPatch") << "draw() : must call create() first and add() after";
        return;
    }
    update();
    transformGL();
    _cachedMesh.draw();
    restoreTransformGL();
}
