#include <iostream>
#include <fstream>
#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "neuronwidget.h"
#include "neuron.h"
#include "qtglut.h"

NeuronGLWidget::NeuronGLWidget(QWidget *parent)
    : QGLWidget(parent), xRot(0.0f), yRot(0.0f), zRot(0.0f), zoom(10.0),
    m_nDrawFlag(1), m_pCompareNeuron(0), m_pControlNeuron(0)
{
//     QString filename = "/Users/xiaochuanq/Documents/workspace/NeuronMatch/Data/Testing/Granule/727884.CNG.swc";

//     std::ifstream ifs(filename.toStdString().c_str());
//     if( ifs ){
//         Neuron* pn = new Neuron( ifs);
//         pn->setName(filename.toStdString());
//         Vector3 axis[3];
//         pn->decomposeAxis2(axis[0], axis[1], axis[2]);
//         pn->setType(Neuron::UnknownType);
//         pn->labelType(Neuron::UnclassifiedType);
//         pn->normalize();
//         pn->alignAxis(axis[0], axis[1], axis[2]);
//         pn->resample(0.05f);
//         m_pCompareNeuron = pn;
//     }
 }

NeuronGLWidget::~NeuronGLWidget()
{
    makeCurrent();
//    glDeleteLists(gear1, 1);

}


void NeuronGLWidget::setZoomFactor(GLfloat z){
    if( zoom != z){
        zoom = z;
        emit zoomFactorChanged(z);
        updateGL();
    }
}

void NeuronGLWidget::setXRotation(GLfloat angle)
{
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void NeuronGLWidget::setYRotation(GLfloat angle)
{
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void NeuronGLWidget::setZRotation(GLfloat angle)
{
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void NeuronGLWidget::initializeGL()
{
    static const GLfloat l0_pos[4] = { 5.0f, 5.0f, 10.0f, 1.0f };
    static const GLfloat l0_dif[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static const GLfloat l0_amb[4] = { 0.2f, 0.2f, 0.2f, 0.2f };
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, l0_dif);
    glLightfv(GL_LIGHT0, GL_AMBIENT, l0_amb);
    glLightfv(GL_LIGHT0, GL_POSITION, l0_pos);
    glEnable(GL_NORMALIZE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void NeuronGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glScaled(zoom, zoom, zoom);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    if( m_nDrawFlag & SHOWNEURON)
        drawNeuron();
    if( m_nDrawFlag & SHOWSAMPLE )
        drawCloud();
    if( m_nDrawFlag & SHOWMESH)
        drawMesh();
    if( m_nDrawFlag & SHOWAXIS)
        drawAxis();
    glPopMatrix();
}

void NeuronGLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, +1.0, -1.0, 1.0, 5.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -40.0);
}

void NeuronGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void NeuronGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}

void NeuronGLWidget::drawNeuron( )
{
    static const GLfloat reflectanceOrange[4] = { 0.8f, 0.65f, 0.0f, 1.0f };
    static const GLfloat reflectanceWhite[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    if( m_pControlNeuron){
        glPushAttrib(GL_LIGHTING_BIT);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, reflectanceWhite);
        m_pControlNeuron->traverse( drawNeuronNode);
        glPopAttrib();
    }
    if( m_pCompareNeuron){
        glPushAttrib(GL_LIGHTING_BIT);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, reflectanceOrange);
        m_pCompareNeuron->traverse( drawNeuronNode);
        glPopAttrib();
    }
}

void NeuronGLWidget::drawCloud()
{
    static const GLfloat reflectanceOrange[4] = { 0.8f, 0.65f, 0.0f, 1.0f };
    static const GLfloat reflectanceWhite[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    if( m_pControlNeuron){
        glPushAttrib(GL_LIGHTING_BIT);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, reflectanceWhite);
        for( size_t i = 0; i < m_pControlNeuron->get_sa_num(); ++i)
        {
            GLfloat r = m_pControlNeuron->get_sa_radius().at(i)/2;
            Vector3 v = m_pControlNeuron->get_sa_points().at(i);
            drawSphere(v.x, v.y, v.z, r);
        }
        glPopAttrib();
    }
    if( m_pCompareNeuron){
        glPushAttrib(GL_LIGHTING_BIT);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, reflectanceOrange);
        for( size_t i = 0; i < m_pCompareNeuron->get_sa_num(); ++i)
        {
            GLfloat r = m_pCompareNeuron->get_sa_radius().at(i)/2;
            Vector3 v = m_pCompareNeuron->get_sa_points().at(i);
            drawSphere(v.x, v.y, v.z, r);
        }
        glPopAttrib();
    }
}

void NeuronGLWidget::drawAxis()
{
    static const GLfloat reflectanceRed[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat reflectanceGreen[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    static const GLfloat reflectanceBlue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

    drawRay(1.0f, 0.0f, 0.0f, reflectanceRed, 2, 0xFFFF);//x
    drawRay(-1.0f, 0.0f, 0.0f, reflectanceRed, 2, 0xFF00);
    drawRay(0.0f, 1.0f, 0.0f, reflectanceGreen, 2, 0xFFFF);//y
    drawRay(0.0f, -1.0f, 0.0f, reflectanceGreen, 2, 0xFF00);
    drawRay(0.0f, 0.0f, 1.0f, reflectanceBlue, 2, 0xFFFF);//z
    drawRay(0.0f, 0.0f, -1.0f, reflectanceBlue, 2, 0xFF00);
}

void NeuronGLWidget::drawRay(GLfloat x, GLfloat y, GLfloat z,
             const GLfloat* color,
             GLfloat w, GLushort pattern)
{
    glPushAttrib(GL_LIGHTING_BIT | GL_LINE_BIT | GL_ENABLE_BIT);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glLineStipple(1, pattern);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glLineWidth(w);
    glVertex3f(0, 0, 0);
    glVertex3f(x, y, z);
    glEnd();
    glPopAttrib();
}

void NeuronGLWidget::drawMesh( )
{
    glutWireSphere(1.0, 12, 24);
}

void NeuronGLWidget::drawSphere(GLfloat x0, GLfloat y0, GLfloat z0, GLfloat r0)
{
    glTranslated(x0, y0, z0);
    glutSolidSphere(r0, 24, 24);
    glTranslated(-x0, -y0, -z0);
}

void NeuronGLWidget::drawEdge(GLfloat x0, GLfloat y0, GLfloat z0, GLfloat r0,
              GLfloat x1, GLfloat y1, GLfloat z1, GLfloat r1)
{
    GLfloat x = x1 - x0;
    GLfloat y = y1 - y0;
    GLfloat z = z1 - z0;
    Vector3 v1(0, 0, 1.0f);
    Vector3 v2(x, y, z);
    Vector3 v3 = v1.crossProduct(v2);
    Vector3 v4 = v3.normalisedCopy();
    GLfloat absin = v3.dotProduct(v4);
    GLfloat abcos = v2.dotProduct(v1);
    GLfloat theta = atan2(absin, abcos)/ M_PI * 180;

    GLUquadric* qcyl = gluNewQuadric();
    GLfloat  height = v2.length();
    glPushMatrix();
        glTranslatef(x0, y0, z0);
        glRotatef( theta, v4.x, v4.y, v4.z);
        gluCylinder(qcyl, r0, r1, height, 12, 16);
    glPopMatrix();
    gluDeleteQuadric(qcyl);
}

void NeuronGLWidget::drawNeuronNode( const NeuronNode* pBase, const NeuronNode* pNode)
{
    if( !pNode)
        return;
    drawSphere(pNode->position().x, pNode->position().y,
              pNode->position().z, pNode->radius());
    if( pBase)
        drawEdge(pBase->position().x, pBase->position().y,
                 pBase->position().z, pBase->radius(),
                 pNode->position().x, pNode->position().y,
                 pNode->position().z, pNode->radius());
}
