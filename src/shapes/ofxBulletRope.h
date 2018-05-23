//
//  ofxBulletRope.h
//  ofxBullet
//
//  Created by Elias Zananiri on 2014-12-02.
//
//

#pragma once

#include "ofxBulletSoftBody.h"

class ofxBulletRope : public ofxBulletSoftBody {
public:
    ofxBulletRope();
    
    void create(ofxBulletWorldSoft* a_world, const glm::vec3& a_from, const glm::vec3& a_to, int a_res = 10);
    
    void draw();
    
    void appendNode();
    
    glm::vec3 getPoint(float pct);
    
    float getLinkLength() const;
    
    void updateMesh( ofMesh& aMesh );

protected:
    float _linkLength;
};
