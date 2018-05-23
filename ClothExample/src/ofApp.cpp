#include "ofApp.h"

bool shouldRemoveRigidBody( const shared_ptr<ofxBulletRigidBody>& ab ) {
    return ab->getPosition().y > 15;
}

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate( 60 );
    
    camera.disableMouseInput();
    camera.setDistance( 14 );
    
    camera.setPosition(ofVec3f(0, -4.f, -10.f));
	camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));
    
    world.setup();
    world.setCamera(&camera);
    
    
	ground.create( world.world, ofVec3f(0., 5.5, 0.), 0., 50., 1.f, 50.f );
	ground.setProperties(.25, .95);
	ground.add();
    
    camera.enableMouseInput();
    
    
    light.setPosition( 0, -10, 0 );
    
    shared_ptr< ofxBulletSphere > sphere( new ofxBulletSphere() );
    sphere->create( world.world, ofVec3f(0,-2.55,0), 0., 1.75 );
    sphere->enableKinematic();
    sphere->setFriction( 0.8 );
    sphere->add();
    sphere->getCollisionShape()->setMargin( 0.25 );
    rigidBodies.push_back( sphere );

    
}

//--------------------------------------------------------------
void ofApp::update() {
	world.update();
    
    ofRemove( rigidBodies, shouldRemoveRigidBody );
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    
    ofEnableDepthTest();
    ofBackground( ofColor( 34,107,126 ) );
    
    camera.begin();
    
//    world.drawDebug();
    
    ofEnableLighting();
    light.enable();
    ofSetColor( 34,107,126 );
    ground.draw();
    
    ofSetHexColor( 0xC4EF02 );
    for( int i = 0; i < rigidBodies.size(); i++ ) {
        rigidBodies[i]->draw();
    }
    
    ofSetColor( 221,59,73 );
    for( int i = 0; i < patches.size(); i++ ) {
        patches[i]->getMesh().drawWireframe();
    }
    
    ofSetLineWidth( 1 );
    
    
    
    light.disable();
    ofDisableLighting();
    
    camera.end();
    
    ofDisableDepthTest();
    
    ofSetColor( 220, 220, 220 );
    ofDrawBitmapString("Shoot ball(spacebar): "+ofToString( rigidBodies.size(), 0)+"\nAdd Cloth(p): "+ofToString(patches.size(), 0), 20, 20 );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if( key == ' ' ) {
        shared_ptr< ofxBulletSphere > ss( new ofxBulletSphere() );
        ss->create( world.world, camera.getPosition(), 0.02, .6 );
        ss->add();
        
        ofVec3f frc = -camera.getPosition();
        frc.normalize();
        ss->applyCentralForce( frc * 50 );
        
        rigidBodies.push_back( ss );
    }
    
    if( key == 'p' ) {
        patches.clear();
        
        const btScalar	s = 8;
        const btScalar	h = -7;
        const int		r = 64;
        
        shared_ptr<ofxBulletPatch> patch( new ofxBulletPatch() );
        patch->create( &world, ofVec3f(-s,h,-s), ofVec3f(s,h,-s), ofVec3f(-s, h, s ), ofVec3f(s,h,s), r, r );
        patch->getSoftBody()->getCollisionShape()->setMargin(0.25);
        
        patch->getSoftBody()->generateBendingConstraints(1, patch->getSoftBody()->m_materials[0]);
        patch->getSoftBody()->m_materials[0]->m_kLST		=	0.4;
        
        patch->add();
        patch->setMass( 0.25, false );
        
        patch->getSoftBody()->m_cfg.piterations = 20;
        patch->getSoftBody()->m_cfg.citerations = 20;
        patch->getSoftBody()->m_cfg.diterations = 20;
        
        patches.push_back( patch );
    }
    
    if( key == 127 || key == OF_KEY_DEL ) {
        if( patches.size() ) {
            patches.erase( patches.begin() );
        }
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