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
	
	void	init( float $radius );
	void	init( btSphereShape* $colShape );
	
	void	create( btDiscreteDynamicsWorld* $world, ofVec3f $loc=ofVec3f(0,0,0), float $mass=1.f, float $radius=1.f );
	void	create( btDiscreteDynamicsWorld* $world, ofVec3f $loc, ofQuaternion $rot, float $mass=1.f, float $radius=1.f );
	void	create( btDiscreteDynamicsWorld* $world, btTransform $bt_tr, float $mass=1.f, float $radius=1.f );
	
	void	draw();
	
	// SETTERS //
	void	setRenderMode(int $mode);
	void	setSphereResolution( int $res );
	
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