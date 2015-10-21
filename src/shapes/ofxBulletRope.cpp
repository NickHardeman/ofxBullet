//
//  ofxBulletRope.cpp
//  ofxBullet
//
//  Created by Elias Zananiri on 2014-12-02.
//
//

#include "ofxBulletRope.h"

//--------------------------------------------------------------
ofxBulletRope::ofxBulletRope() : ofxBulletSoftBody() {
    _linkLength = 1.0;
    _type = OFX_BULLET_SOFT_ROPE;
    _cachedMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
}

//--------------------------------------------------------------
void ofxBulletRope::create(ofxBulletWorldSoft* a_world, const ofVec3f& a_from, const ofVec3f& a_to, int a_res) {
    if(a_world == NULL) {
        ofLogError("ofxBulletRope") << "create(): a_world param is NULL";
        return;
    }
    
    // Minimum 2 nodes or else we can't really do anything...
    if (a_res < 2) a_res = 2;
    
    _world = a_world;
    
    _softBody = btSoftBodyHelpers::CreateRope(_world->getInfo(), btVector3(a_from.x, a_from.y, a_from.z), btVector3(a_to.x, a_to.y, a_to.z), a_res, 0);
    setCreated(_softBody);
    
    // Add the initial vertices and indices to the mesh.
    for (int i = 0; i < getNumNodes(); i++) {
        _cachedMesh.addVertex(getNodePos(i));
    }
    
    _linkLength = a_from.distance(a_to) / a_res;
    
    
	createInternalUserData();
}

//--------------------------------------------------------------
void ofxBulletRope::draw() {
    if(!checkCreate()) {
        ofLogWarning("ofxBulletRope") << "draw() : must call create() first and add() after";
        return;
    }
    transformGL();
    getMesh().draw();
    restoreTransformGL();
}

//--------------------------------------------------------------
void ofxBulletRope::appendNode() {
    int prevIdx = getNumNodes() - 2;
    int currIdx = getNumNodes() - 1;
    int nextIdx = getNumNodes();
    
    // Calculate the next position.
    ofVec3f prevPos = getNodePos(prevIdx);
    ofVec3f currPos = getNodePos(currIdx);
    ofVec3f nextDir = currPos - prevPos;
    ofVec3f nextPos = currPos + nextDir.getNormalized() * _linkLength;
    
    // Add the new node and link.
    getSoftBody()->appendNode(btVector3(nextPos.x, nextPos.y, nextPos.z), 1);
    getSoftBody()->appendLink(currIdx, nextIdx);
    
    // Add the new vertex and indices to the mesh.
    _cachedMesh.addVertex(getNodePos(nextIdx));
}

//--------------------------------------------------------------
float ofxBulletRope::getLinkLength() const
{
    return _linkLength;
}

//--------------------------------------------------------------
ofVec3f ofxBulletRope::getPoint(float pct)
{
    pct = ofClamp(pct, 0, 1);
    int totalNodes = getNumNodes() - 1;
    float fVal = totalNodes * pct;
    int nodeIdx = fVal;
    
    if (nodeIdx == fVal) {
        // Hit a node directly, just return its position.
        return getNodePos(nodeIdx);
    }
    
    // Find the interpolated value between both nodes.
    float nodePct = fVal - nodeIdx;
    ofVec3f prevNode = getNodePos(nodeIdx);
    ofVec3f nextNode = getNodePos(nodeIdx + 1);
    return prevNode.getInterpolated(nextNode, nodePct);
}

//--------------------------------------------------------------
void ofxBulletRope::updateMesh( ofMesh& aMesh ) {
    
    int totalNodes = getNumNodes();
    vector< ofVec3f >& tverts = aMesh.getVertices();
    
    if( _cachedMesh.getMode() == OF_PRIMITIVE_LINE_STRIP ) {
        
        if( tverts.size() != totalNodes ) {
            tverts.resize( totalNodes );
        }
        
        for( int i = 0; i < totalNodes; i++ ) {
            tverts[i].x = _softBody->m_nodes[i].m_x.x();
            tverts[i].y = _softBody->m_nodes[i].m_x.y();
            tverts[i].z = _softBody->m_nodes[i].m_x.z();
        }
    }
    
    _lastMeshUpdateFrame = ofGetFrameNum();
}



