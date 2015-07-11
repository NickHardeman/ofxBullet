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
    
    void create(ofxBulletWorldSoft* a_world, const ofVec3f& a_from, const ofVec3f& a_to, int a_res = 10);
    
    void draw();
    
    void appendNode();
    
    ofVec3f getPoint(float pct);
    
    float getLinkLength() const;
    
    void updateMesh( ofMesh& aMesh );

protected:
    float _linkLength;
};
