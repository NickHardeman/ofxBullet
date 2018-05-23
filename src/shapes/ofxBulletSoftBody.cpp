//
//  ofxBulletSoftBody.cpp
//  ofxBulletJointsExample
//
//  Created by Elias Zananiri on 2014-11-12.
//
//

#include "ofxBulletSoftBody.h"

//--------------------------------------------------------------
ofxBulletSoftBody::ofxBulletSoftBody() : ofxBulletBaseShape() {
    _world = NULL;
    _softBody = NULL;
    _bAdded = false;
    _lastMeshUpdateFrame = -1;
}

//--------------------------------------------------------------
ofxBulletSoftBody::~ofxBulletSoftBody() {
    remove();
}

//--------------------------------------------------------------
void ofxBulletSoftBody::add() {
    _bAdded = true;
    _world->getWorld()->addSoftBody(_softBody);
    
    if( _type != OFX_BULLET_TRI_MESH_SHAPE ) {
        setMass(1);
    }
    setStiffness(0.9, 0.9, 0.9);
    
    setSolverIterations(4);
    
//    _softBody->getCollisionShape()->setMargin(getMargin() * 2);
}

//--------------------------------------------------------------
void ofxBulletSoftBody::remove() {
	setData(NULL);
    removeSoftBody();
	setRemoved();
}

//--------------------------------------------------------------
void ofxBulletSoftBody::removeSoftBody() {
	if(_softBody != NULL) {
		if(_world != NULL && _bAdded) {
			_world->getWorld()->removeSoftBody(_softBody);
			_bAdded = false;
		}
		delete _softBody;
        _softBody = NULL;
	}
}

// GETTERS //

//--------------------------------------------------------------
btSoftBody*	ofxBulletSoftBody::getSoftBody() {
    return _softBody;
}

//--------------------------------------------------------------
int	ofxBulletSoftBody::getType() {
    return _type;
}

//--------------------------------------------------------------
float ofxBulletSoftBody::getMass() const {
    return _softBody->getTotalMass();
}

//--------------------------------------------------------------
glm::vec3	ofxBulletSoftBody::getPosition() const {
    glm::vec3 avgpos;
    for ( int i = 0; i < getNumNodes(); i++ ) {
        avgpos += getNodePos(i);
    }
    avgpos /= (float)getNumNodes();
    return avgpos;
}

//--------------------------------------------------------------
size_t ofxBulletSoftBody::getNumNodes() const {
    return _softBody->m_nodes.size();
}

//--------------------------------------------------------------
size_t ofxBulletSoftBody::getNumFaces() const {
    return _softBody->m_faces.size();
}

//--------------------------------------------------------------
glm::vec3 ofxBulletSoftBody::getNodePos(int idx) const {
    if (idx >= getNumNodes()) {
        ofLogWarning("ofxBulletRope") << "getNodePos() : idx " << idx << " greater than num nodes (" << getNumNodes() << ")";
        return glm::vec3(0,0,0);
    }
    
    return glm::vec3(_softBody->m_nodes.at(idx).m_x.x(),
                   _softBody->m_nodes.at(idx).m_x.y(),
                   _softBody->m_nodes.at(idx).m_x.z());
}

//--------------------------------------------------------------
btSoftBody::tNodeArray& ofxBulletSoftBody::getBulletNodes() {
    return getSoftBody()->m_nodes;
}

// SETTERS //

//--------------------------------------------------------------
void ofxBulletSoftBody::setMass(float a_mass, bool a_fromfaces) {
    _softBody->setTotalMass(a_mass, a_fromfaces);
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setLinearStiffness(float v) {
    v = ofClamp(v, 0, 1);
    _softBody->m_materials[0]->m_kLST = v;
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setAngularStiffness(float v) {
    v = ofClamp(v, 0, 1);
    _softBody->m_materials[0]->m_kAST = v;
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setVolumeStiffness(float v) {
    v = ofClamp(v, 0, 1);
    _softBody->m_materials[0]->m_kVST = v;
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setStiffness(float linear, float angular, float volume) {
    setLinearStiffness(linear);
    setAngularStiffness(angular);
    setVolumeStiffness(volume);
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setDamping(float v) {
    v = ofClamp(v, 0, 1);
    _softBody->m_cfg.kDP = v;
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setDrag(float v) {
    if (v < 0) v = 0;
    _softBody->m_cfg.kDG = v;
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setLift(float v) {
    if (v < 0) v = 0;
    _softBody->m_cfg.kPR = v;
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setPressure(float v) {
    _softBody->m_cfg.kLF = v;
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setVolumeConversation(float v) {
    if (v < 0) v = 0;
    _softBody->m_cfg.kVC = v;
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setDynamicFriction(float v) {
    v = ofClamp(v, 0, 1);
    _softBody->m_cfg.kVC = v;
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setPoseMatching(float v) {
    v = ofClamp(v, 0, 1);
    _softBody->m_cfg.kMT = v;
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setRigidContactsHrdness(float v) {
    v = ofClamp(v, 0, 1);
    _softBody->m_cfg.kCHR = v;
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setKineticContactsHrdness(float v) {
    v = ofClamp(v, 0, 1);
    _softBody->m_cfg.kKHR = v;
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setSoftContactsHrdness(float v) {
    v = ofClamp(v, 0, 1);
    _softBody->m_cfg.kSHR = v;
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setAnchorsContactsHrdness(float v) {
    v = ofClamp(v, 0, 1);
    _softBody->m_cfg.kAHR = v;
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setSolverIterations(int n) {
    _softBody->m_cfg.piterations = n;
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setFixedAt(size_t n) {
    _softBody->setMass(n, 0);
}

//--------------------------------------------------------------
void ofxBulletSoftBody::setNodePositionAt(size_t n, const glm::vec3& pos) {
    _softBody->m_nodes.at(n).m_x = btVector3(pos.x, pos.y, pos.z);
}

//--------------------------------------------------------------
void ofxBulletSoftBody::attachRigidBodyAt(size_t n, btRigidBody *rigid) {
    _softBody->appendAnchor(n, rigid, true);
}

//--------------------------------------------------------------
void ofxBulletSoftBody::updateMesh( ofMesh& aMesh ) {
    
    int totalNodes = getNumNodes();
    int totalFaces = getNumFaces();
    auto& tverts = aMesh.getVertices();
    
    if( _cachedMesh.getMode() == OF_PRIMITIVE_TRIANGLES ) {
        
        if( tverts.size() != totalFaces * 3 ) {
            tverts.resize( totalFaces * 3 );
        }
        
        auto& tnormals = aMesh.getNormals();
        if( aMesh.getNumNormals() != totalFaces * 3 ) {
            tnormals.resize( totalFaces * 3 );
        }
        
        for (int i = 0; i < totalFaces; i += 1 ) {
            for (int j = 0; j < 3; ++j) {
                
                tverts[ i * 3 + j ].x = _softBody->m_faces.at(i).m_n[j]->m_x.x();
                tverts[ i * 3 + j ].y = _softBody->m_faces.at(i).m_n[j]->m_x.y();
                tverts[ i * 3 + j ].z = _softBody->m_faces.at(i).m_n[j]->m_x.z();
                
                tnormals[ i * 3 + j ].x = _softBody->m_faces.at(i).m_n[j]->m_n.x();
                tnormals[ i * 3 + j ].y = _softBody->m_faces.at(i).m_n[j]->m_n.y();
                tnormals[ i * 3 + j ].z = _softBody->m_faces.at(i).m_n[j]->m_n.z();
            }
        }
        
    } else if( _cachedMesh.getMode() == OF_PRIMITIVE_LINE_STRIP ) {
        
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

//--------------------------------------------------------------
ofMesh& ofxBulletSoftBody::getMesh() {
    if( _lastMeshUpdateFrame != ofGetFrameNum() ) {
        updateMesh( _cachedMesh );
    }
    return _cachedMesh;
}





