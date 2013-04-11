#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "desc.h"
#include "descriptorwidget.h"
#include "qtglut.h"

DescriptorGLWidget::DescriptorGLWidget(QWidget *parent)
    : QGLWidget(parent), heightZoom(5.0f),
    xRot(0.0f), yRot(0.0f), zRot(0.0f),
    m_pControlDesc(0), m_pCompareDesc(0)
{

}

DescriptorGLWidget::~DescriptorGLWidget()
{
    makeCurrent();
}

void DescriptorGLWidget::setXRotation(GLfloat angle)
{
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void DescriptorGLWidget::setYRotation(GLfloat angle)
{
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void DescriptorGLWidget::setZRotation(GLfloat angle)
{
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}


void DescriptorGLWidget::setHeightZoom(GLfloat factor)
{
    if (factor != heightZoom) {
        heightZoom = factor;
        emit heightZoomChanged(factor);
        updateGL();
    }
}


void DescriptorGLWidget::initializeGL()
{
    static const GLfloat l0_pos[4] = { 5.0f, 5.0f, 10.0f, 1.0f };
    static const GLfloat l0_dif[4] = { 0.8f, 0.1f, 0.0f, 1.0f };
    static const GLfloat l0_amb[4] = { 0.0f, 0.8f, 0.2f, 1.0f };


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

void DescriptorGLWidget::paintGL()
{
    static const GLfloat reflectance[] =
        {  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f };
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if( m_pCompareDesc){
        Descriptor desc;
        float diff = difference(m_pCompareDesc, m_pControlDesc, desc, chi_diff);
        this->parentWidget()->parentWidget()->setWindowTitle(QString().setNum(diff));
        glPushAttrib(GL_LIGHTING_BIT | GL_LINE_BIT | GL_ENABLE_BIT);
        glPushMatrix();
        glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
        glRotatef(yRot / 16.0 , 0.0, 1.0, 0.0);
        glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
        glScaled(0.2,heightZoom,0.2);
        int i = 0;
        for( size_t ai = 0; ai < desc.size_a(); ++ai ){
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
                         &reflectance[i*4]);
            for( size_t zi = 0; zi < desc.size_z(); ++zi){
                size_t row = ai * desc.size_z() + zi;
                for( size_t ri = 0; ri < desc.size_r(); ++ ri){
                    drawBar( row - desc.height()/2, ri - desc.width()/2, desc( row, ri, 0));
                }
            }
            i = 1 - i;
        }
        glPopMatrix();
    }
    glPopAttrib();
}

void DescriptorGLWidget::resizeGL(int width, int height)
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

void DescriptorGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void DescriptorGLWidget::mouseMoveEvent(QMouseEvent *event)
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


void DescriptorGLWidget::drawBar(GLfloat x, GLfloat z, GLfloat h)
{
    glPushMatrix();
    glTranslated(x, h/2, z);
    glScaled(0.8, h, 0.8);
    glutSolidCube(1.0);
    glPopMatrix();
}
