/*
 *  ofxBulletBox.cpp
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/23/11.
 *
 */

#include "ofxBulletBox.h"

//--------------------------------------------------------------
ofxBulletBox::ofxBulletBox() : ofxBulletRigidBody() {
	_type = OFX_BULLET_BOX_SHAPE;
}

//--------------------------------------------------------------
ofxBulletBox::~ofxBulletBox() {
	
}

//--------------------------------------------------------------
void ofxBulletBox::init(float a_sizeX, float a_sizeY, float a_sizeZ) {
	_shape		= (btCollisionShape*)ofBtGetBoxCollisionShape( a_sizeX, a_sizeY, a_sizeZ );
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletBox::init( btBoxShape* a_colShape ) {
	_shape		= (btCollisionShape*)a_colShape;
    // shape passed in externally, so not responsible for deleteing pointer
    _bColShapeCreatedInternally = false;
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletBox::create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc, float a_mass, float a_sizeX, float a_sizeY, float a_sizeZ ) {
	btTransform tr=ofGetBtTransformFromVec3f( a_loc );
	create( a_world, tr, a_mass, a_sizeX, a_sizeY, a_sizeZ );
}

//--------------------------------------------------------------
void ofxBulletBox::create( btDiscreteDynamicsWorld* a_world, glm::vec3 a_loc, glm::quat a_rot, float a_mass, float a_sizeX, float a_sizeY, float a_sizeZ ) {
	btTransform tr	= ofGetBtTransformFromVec3f( a_loc );
	tr.setRotation( btQuaternion(a_rot.x, a_rot.y, a_rot.z, a_rot.w ) );
	
	create( a_world, tr, a_mass, a_sizeX, a_sizeY, a_sizeZ );
}

//--------------------------------------------------------------
void ofxBulletBox::create( btDiscreteDynamicsWorld* a_world, btTransform &a_bt_tr, float a_mass, float a_sizeX, float a_sizeY, float a_sizeZ ) {
	if(!_bInited || _shape == NULL) {
		ofxBulletRigidBody::create( a_world, (btCollisionShape*)ofBtGetBoxCollisionShape( a_sizeX, a_sizeY, a_sizeZ ), a_bt_tr, a_mass );
	} else {
		ofxBulletRigidBody::create( a_world, _shape, a_bt_tr, a_mass );
	}
	createInternalUserData();
}

//--------------------------------------------------------------
void ofxBulletBox::removeShape() {
    if(_bColShapeCreatedInternally) {
        if(_shape) {
            delete (btBoxShape *)_shape;
            _shape = NULL;
        }
    }
}

//--------------------------------------------------------------
void ofxBulletBox::draw() {
	if(!_bCreated || _rigidBody == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBulletBox :: draw : must call create() first and add() after");
		return;
	}
    
    transformGL();
	glm::vec3 size = getSize();
    ofDrawBox(0, 0, 0, size.x, size.y, size.z);
	restoreTransformGL();
}
 
//--------------------------------------------------------------
glm::vec3 ofxBulletBox::getSize() const {
	float _sx = ((btBoxShape*)_rigidBody->getCollisionShape())->getHalfExtentsWithMargin()[0]*2.f;
	float _sy = ((btBoxShape*)_rigidBody->getCollisionShape())->getHalfExtentsWithMargin()[1]*2.f;
	float _sz = ((btBoxShape*)_rigidBody->getCollisionShape())->getHalfExtentsWithMargin()[2]*2.f;
	return glm::vec3( _sx, _sy, _sz);
}

//--------------------------------------------------------------
float ofxBulletBox::getWidth() const {
    return getSize().x;
}

//--------------------------------------------------------------
float ofxBulletBox::getHeight() const {
    return getSize().y;
}

//--------------------------------------------------------------
float ofxBulletBox::getDepth() const {
    return getSize().z;
}

//--------------------------------------------------------------
bool ofxBulletBox::isInside(const glm::vec3& a_pt, float tolerance) {
	return ((btBoxShape*)_rigidBody->getCollisionShape())->isInside( btVector3(a_pt.x, a_pt.y, a_pt.z), tolerance );
}




