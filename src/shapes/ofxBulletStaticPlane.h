//
//  ofxBulletStaticPlane.h
//  emptyExample
//
//  Created by Lukasz Karluk on 18/12/12.
//
//

#pragma once

#include "ofxBulletBaseShape.h"

class ofxBulletStaticPlane : public ofxBulletBaseShape {
    
public:
    
    ofxBulletStaticPlane();
    ~ofxBulletStaticPlane();
    
    void init(btStaticPlaneShape * shape);
    
    void createFloor(btDiscreteDynamicsWorld * world, ofPoint planePosition);
    void createCeiling(btDiscreteDynamicsWorld * world, ofPoint planePosition);
    void create(btDiscreteDynamicsWorld * world, ofPoint planePosition, ofPoint planeNormal, float planeConstant=1);
    
    void removeShape();
};
