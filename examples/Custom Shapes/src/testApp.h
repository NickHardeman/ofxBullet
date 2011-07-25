#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
#include "ofxAssimpModelLoader.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

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
	ofxBulletCustomShape*		boundsShape;
	ofMaterial					boundsMat;
	float						boundsWidth;
	bool bDropBox;
	
	vector<ofxBulletCustomShape*>	logos;
	ofMaterial						logoMat;
	vector<ofxBulletBaseShape*>		shapes;
	ofMaterial						shapesMat;
	
	bool bDrawDebug;
	
	
	ofMesh						mesh;
	ofCamera					camera;
	ofLight						light;
	
	ofxAssimpModelLoader		assimpModel;
		
};
