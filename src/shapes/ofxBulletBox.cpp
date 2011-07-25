/*
 *  ofxBulletBox.cpp
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/23/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#include "ofxBulletBox.h"

//--------------------------------------------------------------
ofxBulletBox::ofxBulletBox() {
	_type = OFX_BULLET_BOX_SHAPE;
}

//--------------------------------------------------------------
ofxBulletBox::~ofxBulletBox() {
	
}

//--------------------------------------------------------------
void ofxBulletBox::init(float $sizeX, float $sizeY, float $sizeZ) {
	_shape		= (btCollisionShape*)ofBtGetBoxCollisionShape( $sizeX, $sizeY, $sizeZ );
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletBox::init( btBoxShape* $colShape ) {
	_shape		= (btCollisionShape*)$colShape;
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletBox::create( btDiscreteDynamicsWorld* $world, ofVec3f $loc, float $mass, float $sizeX, float $sizeY, float $sizeZ ) {
	create( $world, ofGetBtTransformFromVec3f( $loc ), $mass, $sizeX, $sizeY, $sizeZ );
}

//--------------------------------------------------------------
void ofxBulletBox::create( btDiscreteDynamicsWorld* $world, ofVec3f $loc, ofQuaternion $rot, float $mass, float $sizeX, float $sizeY, float $sizeZ ) {
	btTransform tr	= ofGetBtTransformFromVec3f( $loc );
	tr.setRotation( btQuaternion(btVector3($rot.x(), $rot.y(), $rot.z()), $rot.w()) );
	
	create( $world, tr, $mass, $sizeX, $sizeY, $sizeZ );
}

//--------------------------------------------------------------
void ofxBulletBox::create( btDiscreteDynamicsWorld* $world, btTransform $bt_tr, float $mass, float $sizeX, float $sizeY, float $sizeZ ) {
	if(!_bInited || _shape == NULL) {
		ofxBulletBaseShape::create( $world, (btCollisionShape*)ofBtGetBoxCollisionShape( $sizeX, $sizeY, $sizeZ ), $bt_tr, $mass );
	} else {
		ofxBulletBaseShape::create( $world, _shape, $bt_tr, $mass );
	}
	setData( new ofxBulletUserData() );
}

//--------------------------------------------------------------
void ofxBulletBox::draw() {
	if(!_bCreated || _rigidBody == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBulletBox :: draw : must call create() first and add() after");
		return;
	}
	btScalar	m[16];
	ofGetOpenGLMatrixFromRigidBody( _rigidBody, m );
	glPushMatrix(); 
	glMultMatrixf( m );
	//ofBox(ofVec3f(0., 0., 0.), getSize().x);
	ofVec3f size = getSize();
	drawBox( size.x*.5, size.y*.5, size.z*.5);
	glPopMatrix();	
}


// pulled from the OF core //
//----------------------------------------
void ofxBulletBox::drawBox(float sizeX, float sizeY, float sizeZ) {
	ofPushMatrix();
	
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	if(ofGetStyle().bFill) {
		GLfloat vertices[] = {
			+sizeX,-sizeY,+sizeZ, +sizeX,-sizeY,-sizeZ, +sizeX,+sizeY,-sizeZ, +sizeX,+sizeY,+sizeZ,
			+sizeX,+sizeY,+sizeZ, +sizeX,+sizeY,-sizeZ, -sizeX,+sizeY,-sizeZ, -sizeX,+sizeY,+sizeZ,
			+sizeX,+sizeY,+sizeZ, -sizeX,+sizeY,+sizeZ, -sizeX,-sizeY,+sizeZ, +sizeX,-sizeY,+sizeZ,
			-sizeX,-sizeY,+sizeZ, -sizeX,+sizeY,+sizeZ, -sizeX,+sizeY,-sizeZ, -sizeX,-sizeY,-sizeZ,
			-sizeX,-sizeY,+sizeZ, -sizeX,-sizeY,-sizeZ, +sizeX,-sizeY,-sizeZ, +sizeX,-sizeY,+sizeZ,
			-sizeX,-sizeY,-sizeZ, -sizeX,+sizeY,-sizeZ, +sizeX,+sizeY,-sizeZ, +sizeX,-sizeY,-sizeZ
		};
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		
		static GLfloat normals[] = {
			+1,0,0, +1,0,0, +1,0,0, +1,0,0,
			0,+1,0, 0,+1,0, 0,+1,0, 0,+1,0,
			0,0,+1, 0,0,+1, 0,0,+1, 0,0,+1,
			-1,0,0, -1,0,0, -1,0,0, -1,0,0,
			0,-1,0, 0,-1,0, 0,-1,0, 0,-1,0,
			0,0,-1, 0,0,-1, 0,0,-1, 0,0,-1
		};
		glNormalPointer(GL_FLOAT, 0, normals);
		
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		static GLfloat tex[] = {
			1,0, 0,0, 0,1, 1,1,
			1,1, 1,0, 0,0, 0,1,
			0,1, 1,1, 1,0, 0,0,
			0,0, 0,1, 1,1, 1,0,
			0,0, 0,1, 1,1, 1,0,
			0,0, 0,1, 1,1, 1,0
		};
		glTexCoordPointer(2, GL_FLOAT, 0, tex);
		
		GLubyte indices[] = {
			0,1,2, // right top left
			0,2,3, // right bottom right
			4,5,6, // bottom top right
			4,6,7, // bottom bottom left	
			8,9,10, // back bottom right
			8,10,11, // back top left
			12,13,14, // left bottom right
			12,14,15, // left top left
			16,17,18, // ... etc
			16,18,19,
			20,21,22,
			20,22,23
		};
		glDrawElements(GL_TRIANGLES, 3 * 6 * 2, GL_UNSIGNED_BYTE, indices);
		
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	} else {
		GLfloat vertices[] = {
			+sizeX,+sizeY,+sizeZ,
			+sizeX,+sizeY,-sizeZ,
			+sizeX,-sizeY,+sizeZ,
			+sizeX,-sizeY,-sizeZ,
			-sizeX,+sizeY,+sizeZ,
			-sizeX,+sizeY,-sizeZ,
			-sizeX,-sizeY,+sizeZ,
			-sizeX,-sizeY,-sizeZ
		};
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		
		static float n = sqrtf(3);
		static GLfloat normals[] = {
			+n,+n,+n,
			+n,+n,-n,
			+n,-n,+n,
			+n,-n,-n,
			-n,+n,+n,
			-n,+n,-n,
			-n,-n,+n,
			-n,-n,-n
		};
		glNormalPointer(GL_FLOAT, 0, normals);
		
		static GLubyte indices[] = {
			0,1, 1,3, 3,2, 2,0,
			4,5, 5,7, 7,6, 6,4,
			0,4, 5,1, 7,3, 6,2
		};
		glDrawElements(GL_LINES, 4 * 2 * 3, GL_UNSIGNED_BYTE, indices);
	}
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	
	ofPopMatrix();
}

 
//--------------------------------------------------------------
ofVec3f ofxBulletBox::getSize() const {
	float _sx = ((btBoxShape*)_rigidBody->getCollisionShape())->getHalfExtentsWithMargin()[0]*2.f;
	float _sy = ((btBoxShape*)_rigidBody->getCollisionShape())->getHalfExtentsWithMargin()[1]*2.f;
	float _sz = ((btBoxShape*)_rigidBody->getCollisionShape())->getHalfExtentsWithMargin()[2]*2.f;
	return ofVec3f( _sx, _sy, _sz);
}

//--------------------------------------------------------------
bool ofxBulletBox::isInside(const ofVec3f& $pt, float tolerance) {
	return ((btBoxShape*)_rigidBody->getCollisionShape())->isInside( btVector3($pt.x, $pt.y, $pt.z), tolerance );
}




