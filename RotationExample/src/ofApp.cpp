#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground( 10, 10, 10);
	
    camera.disableMouseInput();
    camera.setAutoDistance( false );
	camera.setPosition(ofVec3f(0, -7.f, -10.f));
	camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));

	world.setup();
	world.enableGrabbing();
	world.setCamera(&camera);
    world.setGravity( ofVec3f(0, 30, 0) );
    
    ground.create( world.world, ofVec3f(0., 5.5, 0.), 0., 100.f, 1.f, 100.f );
	ground.setProperties(.25, .95);
	ground.add();
	
    anchor.set( 0, 0, 0 );
    axis.set( 1, 0, 0 );
    
    bAdd = false;
}

//--------------------------------------------------------------
void ofApp::update() {
	world.update();
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
    
    axis.rotate( ofWrapDegrees(ofGetLastFrameTime() * 4.6 * 60.), ofVec3f(0,1,0) );
    
    float tlen = ((sin( ofGetElapsedTimef() * 5.5 ) * 0.5 + 0.5) + 0.5) * 4.5;
    end = anchor + axis * tlen;
    
    if( ofGetFrameNum() % 10 == 0 ) {
        bAdd = true;
    }
    
    if( bAdd ) {
        shared_ptr< ofxBulletCapsule > capsule( new ofxBulletCapsule() );
        // get the mid pt, since position is the center of the object //
        ofVec3f pos = (end + anchor) * 0.5;
        // figure out the rotation //
        // the up vector of the capsule object is 0,1,0
        ofVec3f localCapsuleAxis( 0, 1, 0 );
        // now make a vector that points along the line we are making from anchor to end
        ofVec3f direction = end - anchor;
        
        float radius = ofRandom(0.2, 0.4 );
        float height = 5.;
        float mass  = radius;
        
        height = direction.length();
        // reduce the height to account for the rounded caps that bullet adds //
        height -= radius * 2.;
        
        direction.normalize();
        
        ofQuaternion rotation;
        rotation.makeRotate( localCapsuleAxis, direction );
        capsule->create( world.world, pos, rotation, mass, radius, height );
        capsule->add();
        
        bodies.push_back( capsule );
        bAdd = false;
    }
    
    if( bodies.size() > 200 ) {
        bodies.erase( bodies.begin() );
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofEnableDepthTest();
	camera.begin();
	
	ofSetLineWidth(1.f);
	ofSetColor(255, 0, 200);
	world.drawDebug();
	
	ofSetColor(100, 100, 100);
	ground.draw();
	
	ofSetColor(225, 225, 225);
	for( int i = 0; i < bodies.size(); i++ ) {
        bodies[i]->draw();
    }
    
    ofSetColor( 255 );
    ofDrawSphere( anchor, 0.15 );
//    ofDrawSphere( end, 0.15 );
    ofSetColor( 220, 0, 0 );
//    ofDrawLine( end, anchor );
    ofDrawArrow( anchor, end, 0.35 );
	
	camera.end();
    ofDisableDepthTest();
    
    stringstream ss;
    ss << "enable camera move (hold spacebar and drag mouse) " << endl;
    ss << "add capsule (a): " << bodies.size() << endl;
    
    ofDrawBitmapStringHighlight( ss.str(), 20, 20 );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if( key == 'a' ) {
        bAdd = true;
    }
    if( key == ' ' ) {
        world.disableGrabbing();
        camera.enableMouseInput();
    }
    
    if( key == OF_KEY_RIGHT ) {
        anchor.x += 0.5;
    }
    
    if( key == OF_KEY_LEFT ) {
        anchor.x -= 0.5;
    }
    if( key == OF_KEY_UP ) {
        anchor.z += 0.5;
    }
    
    if( key == OF_KEY_DOWN ) {
        anchor.z -= 0.5;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if( key == ' ' ) {
        world.enableGrabbing();
        camera.disableMouseInput();
    }
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