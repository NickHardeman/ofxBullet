
#include "GLDebugDrawer.h"

GLDebugDrawer::GLDebugDrawer()
:m_debugMode(0)
{
    lineMesh.setMode( OF_PRIMITIVE_LINES );
}

void	GLDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor)
{
//    ofSetColor( toColor.getX()*255.f, toColor.getY()*255.f, toColor.getZ()*255.f );
//    ofDrawLine(from.x(), from.y(), from.z(), to.x(), to.y(), to.z() );
    lineMesh.addColor( ofFloatColor(fromColor.getX(), fromColor.getY(), fromColor.getZ()) );
    lineMesh.addVertex( ofVec3f(from.x(), from.y(), from.z()) );
    
    lineMesh.addColor( ofFloatColor(toColor.getX(), toColor.getY(), toColor.getZ()) );
    lineMesh.addVertex( ofVec3f(to.x(), to.y(), to.z()) );
}

void	GLDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
	drawLine(from,to,color,color);
}

void GLDebugDrawer::drawSphere (const btVector3& p, btScalar radius, const btVector3& color)
{
	ofSetColor( color.getX()*255.f, color.getY()*255.f, color.getZ()*255.f );
    ofDrawSphere( p.getX(), p.getY(), p.getZ(), radius );
}

void GLDebugDrawer::drawBox (const btVector3& boxMin, const btVector3& boxMax, const btVector3& color, btScalar alpha)
{
	btVector3 halfExtent = (boxMax - boxMin) * btScalar(0.5f);
	btVector3 center = (boxMax + boxMin) * btScalar(0.5f);
	ofSetColor( color.getX()*255.f, color.getY()*255.f, color.getZ()*255.f, alpha * 255.f);
    ofDrawBox(center.getX(), center.getY(), center.getZ(), 2*halfExtent[0], 2*halfExtent[1], 2*halfExtent[2] );
}

void	GLDebugDrawer::drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha)
{
//	if (m_debugMode > 0)
	{
//        ofSetColor( color.getX()*255.f, color.getY()*255.f, color.getZ()*255.f, alpha * 255.f);
//        ofDrawTriangle( a.getX(), a.getY(), a.getZ(), b.getX(), b.getY(), b.getZ(), c.getX(), c.getY(), c.getZ() );
        ofFloatColor tcolor( color.getX(), color.getY(), color.getZ(), alpha );
        ofVec3f p1(a.getX(), a.getY(), a.getZ());
        ofVec3f p2(b.getX(), b.getY(), b.getZ());
        ofVec3f p3(c.getX(), c.getY(), c.getZ());
        
        lineMesh.addVertex( p1 );
        lineMesh.addColor( tcolor );
        lineMesh.addVertex( p2 );
        lineMesh.addColor( tcolor );
        
        lineMesh.addVertex( p2 );
        lineMesh.addColor( tcolor );
        lineMesh.addVertex( p3 );
        lineMesh.addColor( tcolor );
        
        lineMesh.addVertex( p3 );
        lineMesh.addColor( tcolor );
        lineMesh.addVertex( p1 );
        lineMesh.addColor( tcolor );
	}
}

void	GLDebugDrawer::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;

}

void	GLDebugDrawer::draw3dText(const btVector3& location,const char* textString) {
    ofDrawBitmapString( textString, ofVec3f(location.getX(), location.getY(), location.getZ() ) );
}

void	GLDebugDrawer::reportErrorWarning(const char* warningString)
{
	printf("%s\n",warningString);
}

void	GLDebugDrawer::drawContactPoint(const btVector3& pointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{
	
	{
		btVector3 to=pointOnB+normalOnB*distance;
		const btVector3&from = pointOnB;
//		ofSetColor( color.getX()*255.f, color.getY()*255.f, color.getZ()*255.f, 255.f);
//        ofDrawLine(from.x(), from.y(), from.z(), to.x(), to.y(), to.z() );
        
        drawLine( from, to, color, color );
	}
}

void GLDebugDrawer::clear() {
    lineMesh.clear();
}
void GLDebugDrawer::render() {
    if( lineMesh.getNumVertices() ) {
        lineMesh.draw();
    }
}





