#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
//#include "ofxXmlSettings.h"

#define NUM_LIGHTS 3

class testApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void onCollision(ofxBulletCollisionData& cdata);
	void mousePickEvent( ofxBulletMousePickEvent &e );

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
	ofxBulletBox*				ground;
	
	btBoxShape*					boxShape;
	btSphereShape*				sphereShape;
	vector<ofxBulletBaseShape*>	shapes;
	vector<bool>				bColliding;
	int mousePickIndex;
	
	bool						bDrawDebug;
	bool						bDrawWorldDebug;
	bool						bAddSpheres;
	bool						bAddShape;
	bool						bRenderShapes;
	
	ofCamera					camera;
	
};
