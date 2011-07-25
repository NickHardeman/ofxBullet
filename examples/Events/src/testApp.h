#pragma once

#include "ofMain.h"
#include "ofxBullet.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	
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
	vector <ofxBulletBox*>		bounds;
	float						boundsWidth;
	
	ofLight						light;
	
	btBoxShape*					boxShape;
	btSphereShape*				sphereShape;
	vector<ofxBulletBaseShape*>	shapes;
	vector<bool>				bColliding;
	int							mousePickIndex;
	ofVec3f						mousePickPos;
	
	bool						bDrawDebug;
	bool						bRenderShapes;
	bool						bAddCenterAttract;
	bool						bSpacebar;
	
	ofCamera					camera;
		
};
