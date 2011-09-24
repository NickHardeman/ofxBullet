/*
 *  ofxBulletSphere.h
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/18/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#pragma once

#include "ofxBulletBaseShape.h"
#include "ofVbo.h"

class ofxBulletSphere : public ofxBulletBaseShape {
public:
	ofxBulletSphere();
	~ofxBulletSphere();
	
	void	init( float a_radius );
	void	init( btSphereShape* a_colShape );
	
	void	create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc=ofVec3f(0,0,0), float a_mass=1.f, float a_radius=1.f );
	void	create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, ofQuaternion a_rot, float a_mass=1.f, float a_radius=1.f );
	void	create( btDiscreteDynamicsWorld* a_world, btTransform a_bt_tr, float a_mass=1.f, float a_radius=1.f );
	
	void	draw();
	
	// SETTERS //
	void	setRenderMode(int a_mode);
	void	setSphereResolution( int a_res );
	
	// GETTERS //
	float	getRadius();
	
	//void	destroy();
	
	void	createSphere(ofVec3f c, double r, int n, int method, double theta1, double theta2, double phi1, double phi2);
	// leave public if someone wants to manipulate it themselves //
	//ofVbo*	vbo;
	//bool	bVboInited;
	
	//bool	bUseTexture;
	//bool	bUseNormals;
	//bool	bUseVerts;
protected:
	
private:
	int				_renderMode;
	int				_sphereResolution;
	
	
};