//
//  ofxBulletPatch.h
//  ofxBullet
//
//  Created by Elias Zananiri on 2015-03-23.
//
//

#pragma once

#include "ofxBulletSoftBody.h"

class ofxBulletPatch : public ofxBulletSoftBody {
public:
    ofxBulletPatch();
    
    void create(ofxBulletWorldSoft* a_world, const glm::vec3& a_p0, const glm::vec3& a_p1, const glm::vec3& a_p2, const glm::vec3& a_p3, int a_resx = 10, int a_resy = 10);
    
    void draw();
    
    int getResolutionX();
    int getResolutionY();
    
    void updateMesh( ofMesh& aMesh );
    void updateMeshTexCoords( ofMesh& aMesh );

protected:
    int _resx, _resy;
};
