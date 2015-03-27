//
//  ofxBulletEllipsoid.cpp
//  ofxBullet
//
//  Created by Elias Zananiri on 2015-03-23.
//
//

#include "ofxBulletEllipsoid.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"

//--------------------------------------------------------------
ofxBulletEllipsoid::ofxBulletEllipsoid() : ofxBulletSoftBody() {
    _lastUpdateFrame = 0;
    
    _cachedMesh.setMode(OF_PRIMITIVE_TRIANGLES);
}

//--------------------------------------------------------------
void ofxBulletEllipsoid::create(ofxBulletWorldSoft* a_world, const ofVec3f& a_center, const ofVec3f& a_radius, int a_res) {
    if(a_world == NULL) {
        ofLogError("ofxBulletEllipsoid") << "create(): a_world param is NULL";
        return;
    }

	_world = a_world;
    
    _softBody = btSoftBodyHelpers::CreateEllipsoid(_world->getInfo(), btVector3(a_center.x, a_center.y, a_center.z), btVector3(a_radius.x, a_radius.y, a_radius.z), a_res);
    setCreated(_softBody);
        
    _type = OFX_BULLET_SOFT_ELLIPSOID;
	createInternalUserData();
}

//--------------------------------------------------------------
void ofxBulletEllipsoid::update() {
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
void ofxBulletEllipsoid::draw() {
    if(!checkCreate()) {
        ofLogWarning("ofxBulletEllipsoid") << "draw() : must call create() first and add() after";
        return;
    }
    update();
    transformGL();
    _cachedMesh.draw();
    restoreTransformGL();
}
