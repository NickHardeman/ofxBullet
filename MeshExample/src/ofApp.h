#pragma once

#include "ofMain.h"
#include "ofxBullet.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
    void exit();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	ofxBulletWorldRigid			world;
	
	bool		bDrawDebug;
	bool		bSpacebar;
    bool        bDrawFbos;
    bool        bAnimated;
	
	ofEasyCam	camera;
    
    ofMesh mesh, omesh;
    shared_ptr<ofxBulletTriMeshShape> bulletMesh;
    vector< shared_ptr<ofxBulletSphere> > bulletSpheres;
    
    float anisotropy;
    ofFbo fbo, fboSphere;
};
