#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground( 10, 10, 10);

	camera.setPosition(ofVec3f(0, -4.f, -40.f));
	camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));

	world.setup();
	// enables mouse Pick events //
	world.enableGrabbing();
	ofAddListener(world.MOUSE_PICK_EVENT, this, &ofApp::mousePickEvent);

	world.enableCollisionEvents();
	ofAddListener(world.COLLISION_EVENT, this, &ofApp::onCollision);
	world.setCamera(&camera);
	world.setGravity( ofVec3f(0, 25., 0) );

	int ii = 0;
	// let's make a shape that all of the rigid bodies use, since it is faster //
	// though all of the spheres will be the same radius //
	sphereShape = ofBtGetSphereCollisionShape(2.5);
	for (int i = 0; i < 4; i++) {
		shapes.push_back( new ofxBulletSphere() );
		ii = shapes.size()-1;
		((ofxBulletSphere*)shapes[ii])->init(sphereShape);
		// no need to pass radius, since we already created it in the sphereShape //
		((ofxBulletSphere*)shapes[ii])->create(world.world, ofVec3f(ofRandom(-3, 3), ofRandom(-2, 2), ofRandom(-1, 1)), 0.1);
		shapes[ii]->setActivationState( DISABLE_DEACTIVATION );
		shapes[ii]->add();
		bColliding.push_back( false );
	}

	// now lets add some boxes //
	boxShape = ofBtGetBoxCollisionShape(2.65, 2.65, 2.65);
	for (int i = 0; i < 4; i++) {
		shapes.push_back( new ofxBulletBox() );
		ii = shapes.size()-1;
		((ofxBulletBox*)shapes[ii])->init(boxShape);
		((ofxBulletBox*)shapes[ii])->create(world.world, ofVec3f(ofRandom(-3, 3), ofRandom(-2, 2), ofRandom(-1, 1)), 0.2);
		shapes[ii]->setActivationState( DISABLE_DEACTIVATION );
		shapes[ii]->add();
		bColliding.push_back( false );
	}

	ofVec3f startLoc;
	ofPoint dimens;
	boundsWidth = 30.;
	float hwidth = boundsWidth*.5;
	float depth = 2.;
	float hdepth = depth*.5;

	for(int i = 0; i < 6; i++) {
		bounds.push_back( new ofxBulletBox() );
		if(i == 0) { // ground //
			startLoc.set( 0., hwidth+hdepth, 0. );
			dimens.set(boundsWidth, depth, boundsWidth);
		} else if (i == 1) { // back wall //
			startLoc.set(0, 0, hwidth+hdepth);
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
			startLoc.set(0, 0, -hwidth-hdepth);
			dimens.set(boundsWidth, boundsWidth, depth);
		}

		bounds[i]->create( world.world, startLoc, 0., dimens.x, dimens.y, dimens.z );
		bounds[i]->setProperties(.25, .95);
		bounds[i]->add();
	}
    
    light.setPosition(camera.getPosition().x, camera.getPosition().y-14, camera.getPosition().z);

	mousePickIndex	= -1;
	bDrawDebug		= false;
	bRenderShapes	= true;
	bAddCenterAttract	= true;
	bSpacebar		= false;
}

//--------------------------------------------------------------
void ofApp::update() {
	for(int i = 0; i < shapes.size(); i++) {
		bColliding[i] = false;
	}
	
	if(bSpacebar) {
        mousePickIndex = -1;
		ofVec3f mouseLoc = camera.screenToWorld( ofVec3f((float)ofGetMouseX(), (float)ofGetMouseY(), 0) );
		mouseLoc.z += 15;
		ofVec3f diff;
		for(int i = 0; i < shapes.size(); i++) {
			diff = mouseLoc - shapes[i]->getPosition();
			diff *= 2.f;
			if (!bAddCenterAttract) diff *= -1.f;
			shapes[i]->applyCentralForce( diff );
		}
	}

	world.update();
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofEnableDepthTest();
	camera.begin();

	ofSetLineWidth(1.f);
	ofSetColor(255, 0, 200);
	if(bDrawDebug) world.drawDebug();

	ofEnableLighting();
	light.enable();

	if(bRenderShapes) {
		ofSetColor(100, 100, 100);
		for(int i = 0; i < bounds.size()-1; i++) {
			bounds[i]->draw();
		}

		for(int i = 0; i < shapes.size(); i++) {
			if(shapes[i]->getType() == ofxBulletBaseShape::OFX_BULLET_BOX_SHAPE) {
				ofSetColor(15,197,138);
			} else {
				ofSetColor(220, 0, 220);
			}
			if(mousePickIndex == i) {
				ofSetColor(255, 0, 0);
			} else if (bColliding[i] == true) {
				if(shapes[i]->getType() == ofxBulletBaseShape::OFX_BULLET_BOX_SHAPE) {
					ofSetColor(220, 180, 60);
				} else {
					ofSetColor(255, 20, 50);
				}
			}
			shapes[i]->draw();
		}
	}
	light.disable();
	ofDisableLighting();

	camera.end();
    ofDisableDepthTest();

	ofEnableAlphaBlending();
	ofSetColor(0, 0, 0, 150);
	ofDrawRectangle(0, 0, 250, 120);
	ofDisableAlphaBlending();

	ofSetColor(255, 255, 255);
	stringstream ss;
	ss << "framerate: " << ofToString(ofGetFrameRate(),0) << endl;
	ss << "num shapes: " << (shapes.size()+bounds.size()) << endl;
	ss << "draw debug (d): " << ofToString(bDrawDebug, 0) << endl;
	ss << "render shapes (r): " << ofToString(bRenderShapes, 0) << endl;
	ss << "mouse force with spacebar: " << bSpacebar << endl;
	ss << "force direction(f): " << bAddCenterAttract << endl;
	ss << "add spherers (s)" << endl;
	ss << "add boxes (b)" << endl;
	ofDrawBitmapString(ss.str().c_str(), 10, 10);
}

//--------------------------------------------------------------
void ofApp::onCollision(ofxBulletCollisionData& cdata) {
	for(int j = 0; j < bounds.size(); j++) {
		if(*bounds[j] == cdata) {
			return;
		}
	}

	for (int i = 0; i < shapes.size(); i++) {
		if(*shapes[i] == cdata) {
			bColliding[i] = true;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePickEvent( ofxBulletMousePickEvent &e ) {
	mousePickIndex = -1;
	for(int i = 0; i < shapes.size(); i++) {
		if(*shapes[i] == e) {
			mousePickIndex = i;
			mousePickPos = e.pickPosWorld;
			break;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	int ii = 0;

	ofVec3f mouseLoc = camera.screenToWorld( ofVec3f((float)ofGetMouseX(), (float)ofGetMouseY(), 0) );
	float rsize = ofRandom(.3, 1.8);
	mouseLoc.z += 15;

	switch (key) {
		case OF_KEY_DEL:
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
		case 'b':
			shapes.push_back( new ofxBulletBox() );
			ii = shapes.size()-1;
			((ofxBulletBox*)shapes[ii])->create(world.world, mouseLoc, rsize*.2, rsize*2, rsize*2, rsize*2);
			shapes[ii]->setActivationState( DISABLE_DEACTIVATION );
			shapes[ii]->add();
			bColliding.push_back( false );
			break;
		case 's':
			shapes.push_back( new ofxBulletSphere() );
			ii = shapes.size()-1;
			((ofxBulletSphere*)shapes[ii])->create(world.world, mouseLoc, rsize*.2, rsize);
			shapes[ii]->setActivationState( DISABLE_DEACTIVATION );
			shapes[ii]->add();
			bColliding.push_back( false );
			break;
		case 'r':
			bRenderShapes = !bRenderShapes;
			break;
		case ' ':
			bSpacebar = true;
			break;
		case 'f':
			bAddCenterAttract = !bAddCenterAttract;
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
void ofApp::keyReleased(int key) {
	if(key == ' ')
		bSpacebar = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    mousePickIndex = -1;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
