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
    _type = OFX_BULLET_SOFT_ELLIPSOID;
    _cachedMesh.setMode(OF_PRIMITIVE_TRIANGLES);
}

//--------------------------------------------------------------
void ofxBulletEllipsoid::create(ofxBulletWorldSoft* a_world, const glm::vec3& a_center, const glm::vec3& a_radius, int a_res) {
    if(a_world == NULL) {
        ofLogError("ofxBulletEllipsoid") << "create(): a_world param is NULL";
        return;
    }
    
    _radius = a_radius;
	_world = a_world;
    
    _softBody = btSoftBodyHelpers::CreateEllipsoid(_world->getInfo(), btVector3(a_center.x, a_center.y, a_center.z), btVector3(a_radius.x, a_radius.y, a_radius.z), a_res);
    setCreated(_softBody);
        
    
	createInternalUserData();
}

//--------------------------------------------------------------
void ofxBulletEllipsoid::draw() {
    if(!checkCreate()) {
        ofLogWarning("ofxBulletEllipsoid") << "draw() : must call create() first and add() after";
        return;
    }
    transformGL();
    getMesh().draw();
    restoreTransformGL();
}

//--------------------------------------------------------------
glm::vec3 ofxBulletEllipsoid::getRadius() {
    return _radius;
}





