/*
 *  ofxBulletSphere.cpp
 *  ofxBullet_v3
 *
 *  Created by Nick Hardeman on 5/18/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#include "ofxBulletSphere.h"

//--------------------------------------------------------------
ofxBulletSphere::ofxBulletSphere() {
	setRenderMode( OFX_BT_RENDER_TRIS );
	setSphereResolution( 16 );
	
	_type = OFX_BULLET_SPHERE_SHAPE;
}

//--------------------------------------------------------------
ofxBulletSphere::~ofxBulletSphere() {
	//destroy();
}

//--------------------------------------------------------------
void ofxBulletSphere::init(float a_radius) {
	_shape		= (btCollisionShape*)new btSphereShape( a_radius );
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletSphere::init( btSphereShape* a_colShape ) {
	_shape		= (btCollisionShape*)a_colShape;
	_bInited	= true;
}

//--------------------------------------------------------------
void ofxBulletSphere::create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, float a_mass, float a_radius ) {
	create(a_world, ofGetBtTransformFromVec3f( a_loc ), a_mass, a_radius);
}

//--------------------------------------------------------------
void ofxBulletSphere::create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, ofQuaternion a_rot, float a_mass, float a_radius ) {
	btTransform tr	= ofGetBtTransformFromVec3f( a_loc );
	tr.setRotation( btQuaternion(btVector3(a_rot.x(), a_rot.y(), a_rot.z()), a_rot.w()) );
	
	create( a_world, tr, a_mass, a_radius );
}

//--------------------------------------------------------------
void ofxBulletSphere::create( btDiscreteDynamicsWorld* a_world, btTransform a_bt_tr, float a_mass, float a_radius ) {
	if(!_bInited || _shape == NULL) {
		ofxBulletBaseShape::create( a_world, (btCollisionShape*)new btSphereShape( a_radius ), a_bt_tr, a_mass );
	} else {
		ofxBulletBaseShape::create( a_world, _shape, a_bt_tr, a_mass );
	}
	setData( new ofxBulletUserData() );
}

//--------------------------------------------------------------
// TODO: add vbo for drawing //
void ofxBulletSphere::draw() {
	if(!_bCreated || _rigidBody == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBulletSphere :: draw : must call create() first and add() after");
		return;
	}
	// createSphere(ofVec3f c, double r, int n, int method, double theta1, double theta2, double phi1, double phi2);
	btScalar	m[16];
	ofGetOpenGLMatrixFromRigidBody( _rigidBody, m );
	glPushMatrix(); 
	glMultMatrixf( m );
	//createSphere( ofGetVec3fPosFromRigidBody( _rigidBody ), getRadius(), _sphereResolution, _renderMode, 0., TWO_PI, -HALF_PI, HALF_PI );
	createSphere(ofVec3f(0, 0, 0), getRadius(), _sphereResolution, _renderMode, 0., TWO_PI, -HALF_PI, HALF_PI );
	glPopMatrix();
}

//--------------------------------------------------------------
void ofxBulletSphere::setRenderMode(int a_mode) {
	// either OFX_BULLET_RENDER_QUADS || OFX_BULLET_RENDER_TRIS //
	_renderMode			= a_mode;
}

//--------------------------------------------------------------
void ofxBulletSphere::setSphereResolution( int a_res ) {
	_sphereResolution	= a_res;
}

//--------------------------------------------------------------
float ofxBulletSphere::getRadius() {
	return ((btSphereShape*)_rigidBody->getCollisionShape())->getRadius();
}




/*
 Original code by Paul Bourke
 A more efficient contribution by Federico Dosil (below)
 Create a sphere centered at c, with radius r, and precision n
 Draw a point for zero radius spheres
 Use CCW facet ordering
 "method" is 0 for quads, 1 for triangles
 (quads look nicer in wireframe mode)
 Partial spheres can be created using theta1->theta2, phi1->phi2
 in radians 0 < theta < 2pi, -pi/2 < phi < pi/2
 http://paulbourke.net/texture_colour/texturemap/
*/
//--------------------------------------------------------------
void ofxBulletSphere::createSphere(ofVec3f c, double r, int n, int method, double theta1, double theta2, double phi1, double phi2) {
	int i, j;
	double jdivn,j1divn,idivn,dosdivn,unodivn=1/(double)n,ndiv2=(double)n/2,t1,t2,t3,cost1,cost2,cte1,cte3;
	cte3 = (theta2-theta1)/n;
	cte1 = (phi2-phi1)/ndiv2;
	dosdivn = 2*unodivn;
	ofVec3f e,p,e2,p2;
	// Handle special cases //
	if (r < 0)
		r = -r;
	if (n < 0){
		n = -n;
		ndiv2 = -ndiv2;
	}
	if (n < 4 || r <= 0) {
		glBegin(GL_POINTS);
		glVertex3f(c.x,c.y,c.z);
		glEnd();
		return;
	}
	
	t2=phi1;
	cost2=cos(phi1);
	j1divn=0;
	
	//if(bUseVerts) { ofVec3f* verts = new ofVec3f[ndiv2*n]; }
	
	for (j=0;j<ndiv2;j++) {
		t1 = t2;//t1 = phi1 + j * cte1;
		t2 += cte1;//t2 = phi1 + (j + 1) * cte1;
		t3 = theta1 - cte3;
		cost1 = cost2;//cost1=cos(t1);
		cost2 = cos(t2);
		e.y = sin(t1);
		e2.y = sin(t2);
		p.y = c.y + r * e.y;
		p2.y = c.y + r * e2.y;
		
		if (method == 0)
			glBegin(GL_QUAD_STRIP);
		else
			glBegin(GL_TRIANGLE_STRIP);
		
		idivn=0;
		jdivn=j1divn;
		j1divn+=dosdivn;//=2*(j+1)/(double)n;
		for (i=0;i<=n;i++) {
			//t3 = theta1 + i * (theta2 - theta1) / n;
			t3 += cte3;
			e.x = cost1 * cos(t3);
			//e.y = sin(t1);
			e.z = cost1 * sin(t3);
			p.x = c.x + r * e.x;
			//p.y = c.y + r * e.y;
			p.z = c.z + r * e.z;
			glNormal3f(e.x,e.y,e.z);
			glTexCoord2f(idivn,jdivn);
			glVertex3f(p.x,p.y,p.z);
			
			//verts[i*ndiv2 + j];
			
			e2.x = cost2 * cos(t3);
			//e.y = sin(t2);
			e2.z = cost2 * sin(t3);
			p2.x = c.x + r * e2.x;
			//p.y = c.y + r * e.y;
			p2.z = c.z + r * e2.z;
			glNormal3f(e2.x,e2.y,e2.z);
			glTexCoord2f(idivn,j1divn);
			glVertex3f(p2.x,p2.y,p2.z);
			idivn += unodivn;
			
			//verts[(i*ndiv2 + j)+1];
		}
		glEnd();
	}
	// set the data once, for drawing use the gl matrix //
	//vbo.setVertexData( verts, ndiv2*n, GL_STATIC_DRAW_ARB);
}



