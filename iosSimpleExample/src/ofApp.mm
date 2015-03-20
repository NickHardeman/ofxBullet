#include "ofApp.h"

static bool removeIfNull( ofxBulletBaseShape* bs ) {
    return bs == NULL;
}

//--------------------------------------------------------------
void ofApp::setup(){	
    ofSetFrameRate( 60 );
    world.setup();
    
    world.setCamera(&camera);
    world.setGravity( ofVec3f( 0, 0., 40.f) );
    
    camera.setPosition(ofVec3f(0, 0.f, -40.f));
	camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));
    
    lastShapeAddTimef   = -100.f;
    
    ofSetSphereResolution(5);
    
    
    ofVec3f startLoc;
	ofPoint dimens;
	float boundsWidth = 24.;
	float hwidth = boundsWidth*.5f;
	float depth = 2.;
	float hdepth = depth*.5;
    for(int i = 0; i < 6; i++) {
		bounds.push_back( new ofxBulletBox() );
		if(i == 0) { // ground //
			startLoc.set( 0., (hwidth)+hdepth, 0. );
			dimens.set(boundsWidth, depth, boundsWidth);
		} else if (i == 1) { // back wall //
			startLoc.set(0, 0, (hwidth*.1)+hdepth);
			dimens.set(boundsWidth, boundsWidth, depth);
		} else if (i == 2) { // right wall //
			startLoc.set(hwidth+hdepth, 0, 0.);
			dimens.set(depth, boundsWidth, boundsWidth);
		} else if (i == 3) { // left wall //
			startLoc.set(-hwidth-hdepth, 0, 0.);
			dimens.set(depth, boundsWidth, boundsWidth);
		} else if (i == 4) { // ceiling //
			startLoc.set(0, -hwidth-hdepth, 0.);
			dimens.set(boundsWidth, depth, boundsWidth);
		} else if (i == 5) { // front wall //
			startLoc.set(0, 0, (hwidth*.1)-hdepth);
			dimens.set(boundsWidth*.4, boundsWidth*.2, depth);
		}
        
		bounds[i]->create( world.world, startLoc, 0., dimens.x, dimens.y, dimens.z );
		bounds[i]->setProperties(.25, .95);
		bounds[i]->add();
	}
    
    
    light.setPosition(camera.getPosition().x+4, camera.getPosition().y-14, camera.getPosition().z);
    
    // initialize the accelerometer
	ofxAccelerometer.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i = 0; i < shapes.size(); i++ ) {
        ofVec3f pos = shapes[i]->getPosition();
        float dist = 50.f;
        if(pos.z > dist || pos.y > dist || pos.y < -dist || pos.x > dist || pos.x < -dist ) {
            delete shapes[i];
            shapes[i] = NULL;
        }
    }
    ofRemove(shapes, removeIfNull );
    
    colors.resize(shapes.size());
    
    ofVec3f gForce = ofxAccelerometer.getForce();
    gForce.y *= -1.f;
    gForce.z *= -1.f;
    world.setGravity( gForce * 50.f );
    world.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    camera.begin();
    
    ofEnableLighting();
	light.enable();
    glEnable(GL_DEPTH_TEST);
    //    world.drawDebug();
    
    ofSetColor(100, 100, 100);
    for(int i = 0; i < bounds.size(); i++) {
        bounds[i]->draw();
    }
    
    ofMaterial tmat;
    tmat.setShininess(128.f);
    tmat.begin();
    ofSetColor(255, 0, 255);
	for(int i = 0; i < shapes.size(); i++) {
        ofSetColor( colors[i] );
        shapes[i]->draw();
    }
    tmat.end();
    
    light.disable();
	ofDisableLighting();
    
    camera.end();
    glDisable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------
void ofApp::exit(){
    for(int i = 0; i < bounds.size(); i++ ) {
        delete bounds[i];
        bounds[i] = NULL;
    }
    bounds.clear();
    for(int i = 0; i < shapes.size(); i++ ) {
        delete shapes[i];
        shapes[i] = NULL;
    }
    shapes.clear();
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
    if(ofGetElapsedTimef() - lastShapeAddTimef >= .2f ) {
        
        float randyBamJam = ofRandom(0.f, 1.f );
        ofVec3f newLoc = camera.screenToWorld( ofVec3f((float)touch.x, (float)touch.y, 0) );
        int ii;
        
        float rsize = ofRandom(.5, 2.);
        
        if(randyBamJam >= .3) {
            shapes.push_back( new ofxBulletSphere() );
            ii = shapes.size()-1;
            ((ofxBulletSphere*)shapes[ii])->create(world.world, newLoc, rsize*.2, rsize);
            shapes[ii]->setActivationState( DISABLE_DEACTIVATION );
            shapes[ii]->add();
        } else {
            shapes.push_back( new ofxBulletBox() );
			ii = shapes.size()-1;
			((ofxBulletBox*)shapes[ii])->create(world.world, newLoc, rsize*.2, rsize, rsize, rsize);
			shapes[ii]->setActivationState( DISABLE_DEACTIVATION );
			shapes[ii]->add();
        }
        // make sure the pieces land in the box //
        shapes[ii]->applyCentralForce( camera.getLookAtDir() * (2000.f * shapes[ii]->getMass()) );
        
        ofColor tcolor(200, 200, 60);
        tcolor.setHue( (sin(ofGetElapsedTimef()*2.f)*.5f + .5f) * 255.f );
        colors.push_back( tcolor );
        
        lastShapeAddTimef = ofGetElapsedTimef();
    }
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
    for(int i = 0; i < shapes.size(); i++ ) {
        shapes[i]->applyCentralForce(ofVec3f(0,0,-900.f));
    }
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){

}

//--------------------------------------------------------------
void ofApp::gotFocus(){

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){

}

