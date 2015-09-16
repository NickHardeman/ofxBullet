//
//  ofxBulletStaticPlane.cpp
//  emptyExample
//
//  Created by Lukasz Karluk on 18/12/12.
//
//

#include "ofxBulletStaticPlane.h"
#include "btBulletDynamicsCommon.h"

//--------------------------------------------------------------
ofxBulletStaticPlane::ofxBulletStaticPlane() : ofxBulletRigidBody() {
    //
}

//--------------------------------------------------------------
ofxBulletStaticPlane::~ofxBulletStaticPlane() {
    remove();
}

//--------------------------------------------------------------
void ofxBulletStaticPlane::init(btStaticPlaneShape * shape) {
    _shape = shape;
    _bColShapeCreatedInternally = false;
    _bInited = true;
}

//--------------------------------------------------------------
void ofxBulletStaticPlane::createFloor(btDiscreteDynamicsWorld * world, ofPoint planePosition) {
    create(world, planePosition, ofPoint(0, -1, 0));
}

//--------------------------------------------------------------
void ofxBulletStaticPlane::createCeiling(btDiscreteDynamicsWorld * world, ofPoint planePosition) {
    create(world, planePosition, ofPoint(0, 1, 0));
}

//--------------------------------------------------------------
void ofxBulletStaticPlane::create(btDiscreteDynamicsWorld * world, ofPoint planePosition, ofPoint planeNormal, float planeConstant) {
    btStaticPlaneShape * staticPlaneShape = NULL;
    if(_bInited == false || _shape == NULL) {
        staticPlaneShape = new btStaticPlaneShape(btVector3(planeNormal.x, planeNormal.y, planeNormal.z), planeConstant);
        _bInited = true;
    } else {
        staticPlaneShape = (btStaticPlaneShape *)_shape;
    }
    
    btTransform transform = ofGetBtTransformFromVec3f(planePosition);
    float mass = 0.0;
    ofxBulletRigidBody::create(world, staticPlaneShape, transform, mass);
    
    createInternalUserData();
}

//--------------------------------------------------------------
void ofxBulletStaticPlane::removeShape() {
    if(_bColShapeCreatedInternally) {
        if(_shape) {
            delete (btStaticPlaneShape *)_shape;
            _shape = NULL;
        }
    }
}
