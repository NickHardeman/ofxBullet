//
//  ofxBulletEllipsoid.h
//  ofxBullet
//
//  Created by Elias Zananiri on 2015-03-23.
//
//

#pragma once

#include "ofxBulletSoftBody.h"

class ofxBulletEllipsoid : public ofxBulletSoftBody {
public:
    ofxBulletEllipsoid();
    
    void create(ofxBulletWorldSoft* a_world, const ofVec3f& center, const ofVec3f& radius, int res = 10);
    
    void draw();
    
    ofVec3f getRadius();
    int getResolution();

protected:
    ofVec3f _radius;
};
