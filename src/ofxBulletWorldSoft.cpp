//
//  ofxBulletWorldSoft.cpp
//  ofxBullet
//
//  Created by Elias Zananiri on 2014-11-12.
//
//

#include "ofxBulletWorldSoft.h"

//--------------------------------------------------------------
void ofxBulletWorldSoft::setup() {
    ofxBulletWorldRigid::setup();
    
    softBodyWorldInfo.air_density = 1.2;
    softBodyWorldInfo.water_density = 0;
    softBodyWorldInfo.water_offset = 0;
    softBodyWorldInfo.water_normal = btVector3(0,0,0);
    
    softBodyWorldInfo.m_dispatcher = dispatcher;
    softBodyWorldInfo.m_broadphase = broadphase;
    softBodyWorldInfo.m_sparsesdf.Initialize();
    
    setGravity( getGravity() );
}

//--------------------------------------------------------------
btCollisionConfiguration* ofxBulletWorldSoft::createCollisionConfig() {
    return new btSoftBodyRigidBodyCollisionConfiguration();
}

//--------------------------------------------------------------
btDiscreteDynamicsWorld* ofxBulletWorldSoft::createWorld() {
    return new btSoftRigidDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
}

//--------------------------------------------------------------
btSoftRigidDynamicsWorld* ofxBulletWorldSoft::getWorld() {
    return (btSoftRigidDynamicsWorld*)world;
}

//--------------------------------------------------------------
btSoftBodyWorldInfo& ofxBulletWorldSoft::getInfo() {
    return softBodyWorldInfo;
}

//--------------------------------------------------------------
void ofxBulletWorldSoft::setGravity( glm::vec3 a_g ) {
    ofxBulletWorldRigid::setGravity( a_g );
    softBodyWorldInfo.m_gravity = world->getGravity();
}
