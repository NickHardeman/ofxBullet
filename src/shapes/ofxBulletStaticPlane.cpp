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
    //
}

//--------------------------------------------------------------
void ofxBulletStaticPlane::init(btStaticPlaneShape * shape) {
    _shape = shape;
    _bColShapeCreatedInternally = false;
    _bInited = true;
}

//--------------------------------------------------------------
void ofxBulletStaticPlane::createFloor(btDiscreteDynamicsWorld * world, glm::vec3 planePosition) {
    create(world, planePosition, glm::vec3(0, -1, 0));
}

//--------------------------------------------------------------
void ofxBulletStaticPlane::createCeiling(btDiscreteDynamicsWorld * world, glm::vec3 planePosition) {
    create(world, planePosition, glm::vec3(0, 1, 0));
}

//--------------------------------------------------------------
void ofxBulletStaticPlane::create(btDiscreteDynamicsWorld * world, glm::vec3 planePosition, glm::vec3 planeNormal, float planeConstant) {
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
