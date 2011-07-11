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
	// enables mouse Pick events //
	world.enableGrabbing();
	ofAddListener(world.MOUSE_PICK_EVENT, this, &testApp::mousePickEvent);
	
	world.enableCollisionEvents();
	ofAddListener(world.COLLISION_EVENT, this, &testApp::onCollision);
	// turn on debug drawing //
	world.enableDebugDraw();
	world.setCamera(&camera);
	
	int ii = 0;
	// let's make a shape that all of the rigid bodies use, since it is faster //
	// though all of the spheres will be the same radius //
	sphereShape = ofBtGetSphereCollisionShape(.5);
	for (int i = 0; i < 4; i++) {
		shapes.push_back( new ofxBulletSphere() );
		ii = shapes.size()-1;
		((ofxBulletSphere*)shapes[ii])->init(sphereShape);
		// no need to pass radius, since we already created it in the sphereShape //
		((ofxBulletSphere*)shapes[ii])->create(world.world, ofVec3f(ofRandom(-3, 3), ofRandom(-2, 2), ofRandom(-1, 1)), 0.1);
		shapes[ii]->add();
		bColliding.push_back( false );
	}
	
	// now lets add some boxes //
	boxShape = ofBtGetBoxCollisionShape(.65, .65, .65);
	for (int i = 0; i < 4; i++) {
		shapes.push_back( new ofxBulletBox() );
		ii = shapes.size()-1;
		((ofxBulletBox*)shapes[ii])->init(boxShape);
		((ofxBulletBox*)shapes[ii])->create(world.world, ofVec3f(ofRandom(-3, 3), ofRandom(-2, 2), ofRandom(-1, 1)), 0.2);
		shapes[ii]->add();
		bColliding.push_back( false );
	}
	
	ground = new ofxBulletBox();
	ground->create( world.world, ofVec3f(0., 5.5, 0.), 0., 100.f, 1.f, 100.f );
	ground->setProperties(.25, .95);
	ground->add();
	
	mousePickIndex	= -1;
	bDrawDebug		= true;
	bDrawWorldDebug	= true;
	bAddSpheres		= true;
	bAddShape		= false;
	bRenderShapes	= true;
}

//--------------------------------------------------------------
void testApp::update() {
	for(int i = 0; i < shapes.size(); i++) {
		bColliding[i] = false;
	}
	if(bAddShape) {
		int ii = 0;
		
		float mxpct = ((float)ofGetMouseX() - ((float)ofGetWidth() * .5)) / ((float)ofGetWidth()*.5);
		mxpct = ofClamp(mxpct, -1.f, 1.f);
		float mypct = ((float)ofGetMouseY() - ((float)ofGetHeight() * .5)) / ((float)ofGetHeight()*.5);
		mypct = ofClamp(mypct, -1.f, 1.f);
		
		if(bAddSpheres) {
			shapes.push_back( new ofxBulletSphere() );
			ii = shapes.size()-1;
			((ofxBulletSphere*)shapes[ii])->init(sphereShape);
			((ofxBulletSphere*)shapes[ii])->create(world.world, ofVec3f(7.f*mxpct, 4.5f*mypct, ofRandom(-1, 1)), 0.1);
			shapes[ii]->add();
			bColliding.push_back( false );
		} else {
			shapes.push_back( new ofxBulletBox() );
			ii = shapes.size()-1;
			((ofxBulletBox*)shapes[ii])->init(boxShape);
			((ofxBulletBox*)shapes[ii])->create(world.world, ofVec3f(7.f*mxpct, 4.5f*mypct, ofRandom(-1, 1)), 0.2);
			shapes[ii]->add();
			bColliding.push_back( false );
		}
		bAddShape = false;
	}
	world.update();
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
}

//--------------------------------------------------------------
void testApp::draw() {
	glEnable( GL_DEPTH_TEST );
	camera.begin();
	
	ofSetLineWidth(1.f);
	ofSetColor(255, 0, 200);
	if(bDrawWorldDebug) world.drawDebug();
	
	if(bRenderShapes) {
		ofSetColor(100, 100, 100);
		ground->draw();
		
		for(int i = 0; i < shapes.size(); i++) {
			ofSetColor(225, 200, 0);
			if(mousePickIndex == i) 
				ofSetColor(255, 0, 0);
			else if (bColliding[i] == true) 
				ofSetColor(0, 255, 100);
			shapes[i]->draw();
		}
	}
	
	camera.end();
	glDisable(GL_DEPTH_TEST);
	if(bDrawDebug) {
		ofSetColor(255, 255, 255);
		stringstream ss;
		ss << "draw debug (d): " << ofToString(bDrawDebug, 0) << endl;
		ss << "draw bullet world debug (w): " << ofToString(bDrawWorldDebug, 0) << endl;
		ss << "render shapes (r): " << ofToString(bRenderShapes, 0) << endl;
		ss << "add shapes with spacebar" << endl;
		ss << "add spherers (s): " << ofToString(bAddSpheres, 0) << endl;
		ss << "add boxes (b): " << ofToString(!bAddSpheres, 0) << endl;
		ofDrawBitmapString(ss.str().c_str(), 10, 10);
	}
}

//--------------------------------------------------------------
void testApp::onCollision(ofxBulletCollisionData& cdata) {
	if(*ground != cdata) {
		for (int i = 0; i < shapes.size(); i++) {
			if(*shapes[i] == cdata) {
				bColliding[i] = true;
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::mousePickEvent( ofxBulletMousePickEvent &e ) {
	mousePickIndex = -1;
	for(int i = 0; i < shapes.size(); i++) {
		if(*shapes[i] == e) {
			mousePickIndex = i;
		}
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	switch (key) {
		case OF_KEY_DEL:
		case 127:
			if(mousePickIndex > -1) {
				if(shapes.size() > 0) {
					delete shapes[mousePickIndex];
					shapes.erase( shapes.begin()+mousePickIndex );
				}
				mousePickIndex = -1;
			}
			break;
		case 'd':
			bDrawDebug = !bDrawDebug;
			break;
		case 'w':
			bDrawWorldDebug = !bDrawWorldDebug;
			break;
		case 'b':
			bAddSpheres = false;
			break;
		case 's':
			bAddSpheres = true;
			break;
		case ' ':
			bAddShape = true;
			break;
		case 'r':
			bRenderShapes = !bRenderShapes;
			break;
		default:
			break;
	}
	bColliding.clear();
	for(int i = 0; i < shapes.size(); i++) {
		bColliding.push_back( false );
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