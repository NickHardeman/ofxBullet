#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground( 10, 10, 10);
	
	camera.setPosition(ofVec3f(0, -3.f, -40.f));
	camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));

	world.setup();
	world.enableGrabbing();
	world.setCamera(&camera);
	world.setGravity( ofVec3f(0, 25., 0) );
	
	ofVec3f startLoc;
	ofPoint dimens;
	boundsWidth = 30.;
	float hwidth = boundsWidth*.5;
	float depth = 2.;
	float hdepth = depth*.5;
	boundsShape = new ofxBulletCustomShape();
	boundsShape->create(world.world, ofVec3f(0, 0, 0), 10.);
	
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
		btBoxShape* boxShape = ofBtGetBoxCollisionShape( dimens.x, dimens.y, dimens.z );
		boundsShape->addShape( boxShape, startLoc );
		
		bounds[i]->create( world.world, startLoc, 0., dimens.x, dimens.y, dimens.z );
		bounds[i]->setProperties(.25, .95);
		bounds[i]->add();
	}
	
	bDropBox	= false;
	bDrawDebug	= false;
	
	// this model is huge! scale it down 
	ofVec3f scale(.0009, .0009, .0009);
	
	// 3D logo by Lauren Licherdell | http://www.laurenlicherdell.com/ // 
	assimpModel.loadModel("OFlogo.dae", true);
	assimpModel.setScale(scale.x, scale.y, scale.z);
	assimpModel.setPosition(0, 0, 0);
	
	ofQuaternion startRot = ofQuaternion(1., 0., 0., PI);
    
    logos.resize( 3 );
	for (int i = 0; i < logos.size(); i++) {
        logos[i] = new ofxBulletCustomShape();
		startLoc = ofVec3f( ofRandom(-5, 5), ofRandom(0, -hwidth+5), ofRandom(-5, 5) );
		
		if(i == 0) {
			for(int i = 0; i < assimpModel.getNumMeshes(); i++) {
				logos[i]->addMesh(assimpModel.getMesh(i), scale, true);
			}
		} else {
			logos[i]->init( (btCompoundShape*)logos[0]->getCollisionShape(), logos[0]->getCentroid() );
		}
		logos[i]->create( world.world, startLoc, startRot, 3. );
		logos[i]->add();
	}
	
	// let's make an object for the light to follow //
	shapes.push_back( new ofxBulletSphere() );
	((ofxBulletSphere*)shapes[0])->create(world.world, ofVec3f(0, -hwidth+5, -5), .15f, 2.);
	((ofxBulletSphere*)shapes[0])->setActivationState( DISABLE_DEACTIVATION );
	shapes[0]->add();
	
	logoMat.setAmbientColor(ofFloatColor(0, 0, 0));
	logoMat.setDiffuseColor(ofFloatColor(150, 0, 150));
	logoMat.setSpecularColor(ofFloatColor(220, 0, 220));
	logoMat.setShininess(40);
	
	boundsMat.setAmbientColor(ofFloatColor(10, 9, 10));
	boundsMat.setDiffuseColor(ofFloatColor(12, 10, 12));
	boundsMat.setSpecularColor(ofFloatColor(1, 1, 1));
	boundsMat.setShininess(10);
	
	shapesMat.setShininess(80);
}

//--------------------------------------------------------------
void ofApp::update() {
	
	if(bDropBox && bounds.size() > 0) {
		for (int i = 0; i < bounds.size(); i++) {
			delete bounds[i];
		}
		bounds.clear();
		boundsShape->add();
	}
	
	if(bDropBox) {
		ofVec3f diff = ofVec3f(0, -5, 0) - boundsShape->getPosition();
		diff *= 200.f;
		boundsShape->applyCentralForce(diff);
	}
	
	world.update();
	
}

//--------------------------------------------------------------
void ofApp::draw() {
	glEnable( GL_DEPTH_TEST );
	
	camera.begin();
	
	ofSetLineWidth(1.f);
	if(bDrawDebug) world.drawDebug();
	
	ofEnableLighting();
	light.enable();
	light.setPosition(shapes[0]->getPosition());
	ofSetColor(255, 255, 255);
	shapes[0]->draw();
	
	ofSetColor(100., 100., 100.);
	
	if(!bDropBox) {
		boundsMat.begin();
		for(int i = 0; i < bounds.size()-1; i++) {
			bounds[i]->draw();
		}
		boundsMat.end();
	} else {
		ofNoFill();
        boundsShape->transformGL();
		ofDrawBox(ofVec3f(0, 0,0), boundsWidth);
		boundsShape->restoreTransformGL();
		ofFill();
	}
	
	ofDisableAlphaBlending();
	ofDisableBlendMode();
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    glEnable(GL_NORMALIZE);
    glDisable(GL_CULL_FACE);
	ofPoint scale		= assimpModel.getScale();
	
	ofSetColor(0, 0, 0);
	logoMat.begin();
	for(int i = 0; i < logos.size(); i++) {
        logos[i]->transformGL();
		ofScale(scale.x,scale.y,scale.z);
		assimpModel.getMesh(0).drawFaces();
		logos[i]->restoreTransformGL();
	}
	glPopAttrib();
	logoMat.end();
	
	ofSetColor(15,197,138);
	ofPushStyle();
	shapesMat.begin();
	for(int i = 0; i < shapes.size(); i++) {
		shapes[i]->draw();
	}
	shapesMat.end();
	ofPopStyle();
	
	light.disable();
	ofDisableLighting();
	
	camera.end();
	glDisable(GL_DEPTH_TEST);
	
	int totalShapes = shapes.size() + logos.size();
	ofVec3f gravity = world.getGravity();
	stringstream ss;
	ss << "Draw Debug (d): " << bDrawDebug << endl;
	ss << "Total Shapes: " << totalShapes << endl;
	ss << "Add logos(o)" << endl;
	ss << "add spherers (s)" << endl;
	ss << "add boxes (b)" << endl;
	ss << "Gravity(up/down/left/right): x=" << gravity.x << " y= " << gravity.y << " z= " << gravity.z << endl;
	ofSetColor(255, 255, 255);
	ofDrawBitmapString(ss.str().c_str(), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	ofVec3f mouseLoc = camera.screenToWorld( ofVec3f((float)ofGetMouseX(), (float)ofGetMouseY(), 0) );
	ofVec3f gravity = world.getGravity();
	ofQuaternion startRot = ofQuaternion(1., 0., 0., PI);
	float rsize = ofRandom(.3, 1.8);
	mouseLoc.z += 15;
	switch (key) {
		case 'd':
			bDrawDebug = !bDrawDebug;
			break;
		case 'o':
			logos.push_back( new ofxBulletCustomShape() );
			logos[logos.size()-1]->init( (btCompoundShape*)logos[0]->getCollisionShape(), logos[0]->getCentroid() );
			logos[logos.size()-1]->create( world.world, mouseLoc, startRot, 3. );
			logos[logos.size()-1]->add();
			break;
		case 's':
			shapes.push_back( new ofxBulletSphere() );
			((ofxBulletSphere*)shapes[shapes.size()-1])->create( world.world, mouseLoc, rsize*.2, rsize );
			((ofxBulletSphere*)shapes[shapes.size()-1])->setActivationState( DISABLE_DEACTIVATION );
			shapes[shapes.size()-1]->add();
			break;
		case 'b':
			shapes.push_back( new ofxBulletBox() );
			((ofxBulletBox*)shapes[shapes.size()-1])->create( world.world, mouseLoc, rsize*.2, rsize*2, rsize*2, rsize*2 );
			((ofxBulletBox*)shapes[shapes.size()-1])->setActivationState( DISABLE_DEACTIVATION );
			shapes[shapes.size()-1]->add();
			break;
		case ' ':
			bDropBox = true;
			break;
		case OF_KEY_UP:
			gravity.y -= 5.;
			world.setGravity( gravity );
			break;
		case OF_KEY_DOWN:
			gravity.y += 5.;
			world.setGravity( gravity );
			break;
		case OF_KEY_RIGHT:
			gravity.x += 5.;
			world.setGravity( gravity );
			break;
		case OF_KEY_LEFT:
			gravity.x -= 5.;
			world.setGravity( gravity );
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	
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