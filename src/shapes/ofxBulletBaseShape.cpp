/*
 *  ofxBulletBaseCollisionShape.cpp
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/18/11.
 *
 */

#include "ofxBulletBaseShape.h"

//--------------------------------------------------------------
ofxBulletBaseShape::ofxBulletBaseShape() {
	_bCreated		= false;
	_bInited		= false;
	_bAdded			= false;
	_userPointer	= NULL;
	_object			= NULL;
    _bUserDataCreatedInternally = true;
}

//--------------------------------------------------------------
ofxBulletBaseShape::~ofxBulletBaseShape() {

}

//--------------------------------------------------------------
void ofxBulletBaseShape::setCreated(btCollisionObject* object)
{
    _object = object;
    _bCreated = true;
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setRemoved()
{

	_object = NULL;
    _bCreated = false;
}

//--------------------------------------------------------------
bool ofxBulletBaseShape::operator==( const void* a_userData) const {
	return getData() == a_userData;
}

//--------------------------------------------------------------
bool ofxBulletBaseShape::operator==( const ofxBulletCollisionData& a_collisionData) const {
	return (getData() == a_collisionData.userData1) || (getData() == a_collisionData.userData2);
}

//--------------------------------------------------------------
bool ofxBulletBaseShape::operator!=( const ofxBulletCollisionData& a_collisionData) const {
	return (getData() != a_collisionData.userData1) && (getData() != a_collisionData.userData2);
}

//--------------------------------------------------------------
bool ofxBulletBaseShape::operator==( const ofxBulletMousePickEvent& a_e ) const {
	return getData() == a_e.userData;
}
//--------------------------------------------------------------
bool ofxBulletBaseShape::operator!=( const ofxBulletMousePickEvent& a_e ) const {
	return !(getData() == a_e.userData);
}

//--------------------------------------------------------------
bool ofxBulletBaseShape::operator==( const ofxBulletRaycastData& a_e ) const {
	return getData() == a_e.userData;
}
//--------------------------------------------------------------
bool ofxBulletBaseShape::operator!=( const ofxBulletRaycastData& a_e ) const {
	return !(getData() == a_e.userData);
}




// GETTERS //

//--------------------------------------------------------------
btCollisionObject* ofxBulletBaseShape::getCollisionObject() {
    return _object;
}

//--------------------------------------------------------------
void* ofxBulletBaseShape::getData() const {
	return _object->getUserPointer();
}

//--------------------------------------------------------------
int ofxBulletBaseShape::getActivationState() {
	// returns OF_BT_ACTIVATION_STATE_ACTIVE || OF_BT_ACTIVATION_ISLAND_SLEEPING
	return _object->getActivationState();
}

//--------------------------------------------------------------
int ofxBulletBaseShape::getType() {
	return _type;
}

//--------------------------------------------------------------
ofMatrix4x4 ofxBulletBaseShape::getTransformationMatrix() const {
    const btTransform& tr = _object->getWorldTransform();
	btScalar	ATTRIBUTE_ALIGNED16(m[16]);
    tr.getOpenGLMatrix(m);
	return ofMatrix4x4(m);
}

//--------------------------------------------------------------
glm::vec3 ofxBulletBaseShape::getPosition() const {
    return getTransformationMatrix().getTranslation();
}

// returns yaw, pitch, roll //
//--------------------------------------------------------------
glm::vec3 ofxBulletBaseShape::getRotation( ) const {
    return glm::eulerAngles( getRotationQuat() );
//    return getRotationQuat().getEuler();
}

//--------------------------------------------------------------
glm::vec3 ofxBulletBaseShape::getRotationAxis() const {
	btQuaternion rotQuat		= _object->getWorldTransform().getRotation();
	btVector3 btaxis			= rotQuat.getAxis();
	return glm::vec3( btaxis.getX(), btaxis.getY(), btaxis.getZ() );
}

//--------------------------------------------------------------
float ofxBulletBaseShape::getRotationAngle() const {
	btQuaternion rotQuat		= _object->getWorldTransform().getRotation();
	return rotQuat.getAngle();
}

//--------------------------------------------------------------
glm::quat ofxBulletBaseShape::getRotationQuat() const {
    return getTransformationMatrix().getRotate();
}




//--------------------------------------------------------------
float ofxBulletBaseShape::getRestitution() const {
	return (float)_object->getRestitution();
}

//--------------------------------------------------------------
float ofxBulletBaseShape::getFriction() const {
	return _object->getFriction();
}



/**************************************************************/
// PROPERTY SETTERS, must be called after create() and before add() //

//--------------------------------------------------------------
void ofxBulletBaseShape::setProperties(float a_restitution, float a_friction) {
	setRestitution(a_restitution);
	setFriction(a_friction);
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setRestitution( float a_res ) {
	if(checkCreate()) {
		_object->setRestitution( btScalar(a_res) );
	}
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setFriction( float a_friction ) {
	if(checkCreate()) {
		_object->setFriction( btScalar(a_friction) );
	}
}

//--------------------------------------------------------------
void ofxBulletBaseShape::setActivationState( int a_state ) {
	if(checkCreate()) {
		_object->setActivationState( a_state );
	}
}
/**************************************************************/




// SETTERS, may be called after create() //

//--------------------------------------------------------------
void ofxBulletBaseShape::setData(void* userPointer) {
	// Remove old data first.
	if(_userPointer != NULL) {
		if(_bUserDataCreatedInternally) {
            delete ((ofxBulletUserData*)_userPointer);
        }
    }

	_userPointer = userPointer;
	_object->setUserPointer( _userPointer );
	_bUserDataCreatedInternally = false;
}

//--------------------------------------------------------------
void ofxBulletBaseShape::createInternalUserData() {
	setData( new ofxBulletUserData() );
	_bUserDataCreatedInternally = true;
}

// CHECKERS //

//--------------------------------------------------------------
bool ofxBulletBaseShape::checkInit() {
	if(!_bInited) {
		ofLog(OF_LOG_WARNING, "ofxBulletBaseShape :: must call init() first!");
		return false;
	}
	return true;
}

//--------------------------------------------------------------
bool ofxBulletBaseShape::checkPropCreate() {
	if(_bCreated) {
		ofLog(OF_LOG_WARNING, "ofxBulletBaseShape :: these property changes will not affect the shape, must call before create()!");
		return true;
	}
	return false;
}
		 
//--------------------------------------------------------------
bool ofxBulletBaseShape::checkCreate() {
	if(!_bCreated) {
		ofLog(OF_LOG_WARNING, "ofxBulletBaseShape :: must call create() first");
		return false;
	}
	return true;
}




//--------------------------------------------------------------
void ofxBulletBaseShape::activate() {
	((btCollisionObject *)_object->getCollisionShape())->activate( true );
	//_object->activate( true );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::enableKinematic() {
	_object->setCollisionFlags( _object->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::enableStatic() {
    _object->setCollisionFlags( _object->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::transformGL() {
	ofPushMatrix();
    ofMultMatrix( getTransformationMatrix() );
}

//--------------------------------------------------------------
void ofxBulletBaseShape::restoreTransformGL() {
    ofPopMatrix();
}








