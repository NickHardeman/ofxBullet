#pragma once

#include "ofMain.h"
#include "ofxiOS.h"
#include "ofxiOSExtras.h"
#include "ofxBullet.h"

class ofApp : public ofxiOSApp{
	
public:
    void setup();
    void update();
    void draw();
    void exit();

    void touchDown(ofTouchEventArgs & touch);
    void touchMoved(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);
    void touchDoubleTap(ofTouchEventArgs & touch);
    void touchCancelled(ofTouchEventArgs & touch);

    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(int newOrientation);
    
    ofxBulletWorldRigid			world;
    vector<ofxBulletRigidBody*>	shapes;
    vector<ofColor> colors;
    float lastShapeAddTimef;
    ofCamera    camera;
    ofLight     light;
    vector <ofxBulletBox*>		bounds;

};


