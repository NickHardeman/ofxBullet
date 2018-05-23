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
    
    void create(ofxBulletWorldSoft* a_world, const glm::vec3& center, const glm::vec3& radius, int res = 10);
    
    void draw();
    
    glm::vec3 getRadius();
    int getResolution();

protected:
    glm::vec3 _radius;
};
