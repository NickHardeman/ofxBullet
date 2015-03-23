//
//  ofxBulletStaticPlane.h
//  emptyExample
//
//  Created by Lukasz Karluk on 18/12/12.
//
//

#pragma once

#include "ofxBulletRigidBody.h"

class ofxBulletStaticPlane : public ofxBulletRigidBody {
    
public:
    
    ofxBulletStaticPlane();
    ~ofxBulletStaticPlane();
    
    void init(btStaticPlaneShape * shape);
    
    void createFloor(btDiscreteDynamicsWorld * world, ofPoint planePosition);
    void createCeiling(btDiscreteDynamicsWorld * world, ofPoint planePosition);
    void create(btDiscreteDynamicsWorld * world, ofPoint planePosition, ofPoint planeNormal, float planeConstant=0);
    
    void removeShape();
};
