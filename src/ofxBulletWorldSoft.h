//
//  ofxBulletWorldSoft.h
//  ofxBullet
//
//  Created by Elias Zananiri on 2014-11-12.
//
//

#pragma once

#include "ofMain.h"
#include "ofxBulletWorldRigid.h"
#include "BulletSoftBody/btSoftBody.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

class ofxBulletWorldSoft : public ofxBulletWorldRigid {
public:
    virtual void setup();
    virtual void setGravity( glm::vec3 a_g );

    btSoftRigidDynamicsWorld* getWorld();
    btSoftBodyWorldInfo& getInfo();

protected:
    virtual btCollisionConfiguration* createCollisionConfig();
    virtual btDiscreteDynamicsWorld* createWorld();
    
    btSoftBodyWorldInfo	softBodyWorldInfo;

};
