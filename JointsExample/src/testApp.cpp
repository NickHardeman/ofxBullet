#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground( 10, 10, 10);
	
	camera.setPosition(ofVec3f(0, -4.f, -10.f));
	camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));
	
	world.setup();
	world.setCamera(&camera);
	
	ground = new ofxBulletBox();
	ground->create( world.world, ofVec3f(0., 5.5, 0.), 0., 50., 1.f, 50.f );
	ground->setProperties(.25, .95);
	ground->add();
	
	
	colors[0] = ofColor(15,197,138);
	colors[1] = ofColor(220, 0, 220);
	colors[2] = ofColor(220, 180, 60);
	colors[3] = ofColor(255, 20, 50);
	
	jointLength = .35f;
	
	shapes.push_back( new ofxBulletSphere() );
	((ofxBulletSphere*)shapes[0])->create( world.world, ofVec3f(0, 2, 0), 0.4f, .5f );
	shapes[0]->add();
	
	shapeColors.push_back( colors[(int)round(ofRandom(0, 3))] );
	
	joints.push_back( new ofxBulletJoint() );
	joints[joints.size()-1]->create( world.world, shapes[0], ofVec3f(0, 0, 0));
	joints[joints.size()-1]->add();
	
	ofSetSmoothLighting(true);
	light.setAmbientColor(ofColor(.0, .0, .0));
	light.setDiffuseColor(ofColor(.0, .0, .0));
	light.setSpecularColor(ofColor(255, .1, .1));
	
	bDrawDebug	= false;
	bSpacebar	= false;
	bShapesNeedErase = false;
	
	ofHideCursor();
}

//--------------------------------------------------------------
void testApp::update() {
	world.update();
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
	
	mousePos = camera.screenToWorld( ofVec3f((float)ofGetMouseX(), (float)ofGetMouseY(), 0) );
	joints[0]->updatePivotPos( mousePos, 2.f );
	
	if(bSpacebar) {
		for (int i = 1; i < joints.size(); i++) {
			delete joints[i];
		}
		joints.erase(joints.begin()+1, joints.end());
		bSpacebar = false;
		bShapesNeedErase = true;
	}
	
	for (int i = 1; i < joints.size(); i++) {
		if(i == 1) {
			joints[i]->updatePivotPos( shapes[i-1]->getPosition(), jointLength*2.f );
		} else {
			joints[i]->updatePivotPos( shapes[i-1]->getPosition(), jointLength );
		}
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	glEnable( GL_DEPTH_TEST );
	camera.begin();
	
	ofSetLineWidth(1.f);
	if(bDrawDebug) world.drawDebug();
	
	ofSetColor(255, 255, 255);
	ofSphere(mousePos, .15f);
	
	ofEnableLighting();
	light.enable();
	light.setPosition( mousePos );
	
	ofSetColor(100, 100, 100);
	ground->draw();
	
	ofSetColor(255, 0, 255);
	for(int i = 0; i < shapes.size(); i++) {
		ofSetColor(shapeColors[i]);
		shapes[i]->draw();
	}
	ofSetColor(220, 220, 220);
	for(int i = 0; i < joints.size(); i++) {
		joints[i]->draw();
	}
	light.disable();
	ofDisableLighting();
	
	camera.end();
	glDisable(GL_DEPTH_TEST);
	
	ofSetColor(255, 255, 255);
	stringstream ss;
	ss << "framerate: " << ofToString(ofGetFrameRate(),0) << endl;
	ss << "num shapes: " << (shapes.size()) << endl;
	ss << "draw debug (d): " << ofToString(bDrawDebug, 0) << endl;
	ss << "break joints with spacebar: " << bSpacebar << endl;
	ofDrawBitmapString(ss.str().c_str(), 10, 10);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	switch (key) {
		case ' ':
			bSpacebar = true;
			break;
		case 'd':
			bDrawDebug = !bDrawDebug;
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	if(bShapesNeedErase) {
		for (int i = 1; i < shapes.size(); i++) {
			delete shapes[i];
		}
		shapes.erase( shapes.begin()+1, shapes.end() );
		bShapesNeedErase = false;
		shapeColors.erase(shapeColors.begin()+1, shapeColors.end());
	}
	
	float rsize = ofRandom(.15, .3);
	
	ofVec3f diff;
	if(shapes.size() < 2) {
		diff = mousePos - shapes[0]->getPosition();
	} else {
		diff = shapes[shapes.size()-2]->getPosition() - shapes[shapes.size()-1]->getPosition();
	}
	diff.normalize();
	if(shapes.size() < 2) {
		diff *= -(jointLength*2.f);
	} else {
		diff *= -jointLength;
	}
	diff += shapes[shapes.size()-1]->getPosition();
	
	shapes.push_back( new ofxBulletSphere() );
	((ofxBulletSphere*)shapes[shapes.size()-1])->create( world.world, diff, .8, rsize );
	shapes[shapes.size()-1]->add();
	
	shapeColors.push_back( colors[(int)round(ofRandom(0, 3))] );
	
	
	joints.push_back( new ofxBulletJoint() );
	joints[joints.size()-1]->create(world.world, shapes[shapes.size()-1], shapes[shapes.size()-2]->getPosition());
	joints[joints.size()-1]->add();
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