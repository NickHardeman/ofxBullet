#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground( 10, 10, 10);
	
	camera.setPosition(ofVec3f(0, -7.f, -10.f));
	camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));
	
	camera.cacheMatrices(true);
	
	world.setup();
	world.enableGrabbing();
	world.enableDebugDraw();
	world.setCamera(&camera);
	
	ground = new ofxBulletBox();
	ground->create( world.world, ofVec3f(0., 5.5, 0.), 0., 100.f, 1.f, 100.f );
	ground->setProperties(.25, .95);
	ground->add();
	
	for(int i = 0; i < 3; i++) {
		shapes.push_back( new ofxBulletSphere() );
		if(i == 0) {
			((ofxBulletSphere*)shapes[i])->create( world.world, ofVec3f(0, 2, 0), 0.9f, .8f );
		} else {
			((ofxBulletSphere*)shapes[i])->create( world.world, ofVec3f(ofRandom(-3, 3), ofRandom(-3, 3), ofRandom(-3, 3)), 0.1f, .2f );
		}
		shapes[i]->add();
	}
	
	for(int i = 0; i < shapes.size(); i++) {
		joints.push_back( new ofxBulletJoint() );
		if(i < shapes.size()-1) {
			joints[i]->create(world.world, shapes[i], shapes[i+1]);
		} else {
			joints[i]->create(world.world, shapes[i], shapes[0]);
		}
		joints[i]->add();
	}
	
	//joints.push_back( new ofxBulletJoint() );
	//joints[joints.size()-1]->create( world.world, shapes[1], shapes[3]);
	//joints[joints.size()-1]->add();
	
	joints.push_back( new ofxBulletJoint() );
	joints[joints.size()-1]->create( world.world, shapes[0], ofVec3f(0, 0, 0));
	joints[joints.size()-1]->add();
	
}

//--------------------------------------------------------------
void testApp::update() {
	world.update();
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
	
}

//--------------------------------------------------------------
void testApp::draw() {
	glEnable( GL_DEPTH_TEST );
	camera.begin();
	
	ofSetLineWidth(1.f);
	ofSetColor(255, 0, 200);
	world.drawDebug();
	
	ofSetColor(100, 100, 100);
	ground->draw();
	
	ofSetColor(255, 0, 255);
	for(int i = 0; i < shapes.size(); i++) {
		shapes[i]->draw();
	}
	ofSetColor(175, 175, 175);
	for(int i = 0; i < joints.size(); i++) {
		joints[i]->draw();
		//ofSetColor(255, 255, 0);
		//joints[i]->drawJointConstraints();
	}
	
	
	camera.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
	ofVec3f mouseRay = camera.screenToWorld( ofVec3f((float)ofGetMouseX(), (float)ofGetMouseY(), 0) );
	joints[joints.size()-1]->updatePivotPos( mouseRay, 2.f );
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg) {
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo) { 
	
}