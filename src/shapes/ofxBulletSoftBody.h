//
//  ofxBulletSoftBody.h
//  ofxBulletJointsExample
//
//  Created by Elias Zananiri on 2014-11-12.
//
//

#pragma once

#include "ofMain.h"
#include "ofxBulletBaseShape.h"
#include "ofxBulletWorldSoft.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

class ofxBulletSoftBody : public ofxBulletBaseShape {
public:
    ofxBulletSoftBody();
    ~ofxBulletSoftBody();
    
    virtual void add();
    virtual void remove();
    virtual void removeSoftBody();
    
    // GETTERS //
    btSoftBody*	getSoftBody();
    
    int getType();
    
    float getMass() const;
    // returns an average position of all of the nodes //
    virtual glm::vec3	getPosition() const;

    size_t getNumNodes() const;
    glm::vec3 getNodePos(int idx) const;
    btSoftBody::tNodeArray& getBulletNodes();

	size_t getNumFaces() const;
    
    // SETTERS //
    void setMass(float a_mass, bool a_fromfaces = false);
    
    void setLinearStiffness(float v);
    void setAngularStiffness(float v);
    void setVolumeStiffness(float v);
    void setStiffness(float linear, float angular, float volume);
    
    void setDamping(float v);
    void setDrag(float v);
    void setLift(float v);
    void setPressure(float v);
    
    void setVolumeConversation(float v);
    void setDynamicFriction(float v);
    void setPoseMatching(float v);
    
    void setRigidContactsHrdness(float v);
    void setKineticContactsHrdness(float v);
    void setSoftContactsHrdness(float v);
    void setAnchorsContactsHrdness(float v);
    
    void setNodePositionAt(size_t n, const glm::vec3& pos);
    void setFixedAt(size_t n);
    void attachRigidBodyAt(size_t n, btRigidBody *rigid);
    
    void setSolverIterations(int n);
    
    
    // mesh //
    virtual void updateMesh( ofMesh& aMesh );
    ofMesh& getMesh();
    
protected:
    ofxBulletWorldSoft* _world;
    btSoftBody* _softBody;
    ofMesh _cachedMesh;
    
    bool _bAdded;
    
    int _type;
    
    int _lastMeshUpdateFrame;
};
