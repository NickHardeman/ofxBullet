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
    
    void createFloor(btDiscreteDynamicsWorld * world, glm::vec3 planePosition);
    void createCeiling(btDiscreteDynamicsWorld * world, glm::vec3 planePosition);
    void create(btDiscreteDynamicsWorld * world, glm::vec3 planePosition, glm::vec3 planeNormal, float planeConstant=0);
    
    void removeShape();
};
