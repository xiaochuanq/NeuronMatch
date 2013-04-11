#ifndef DESCRIPTORWIDGET_H
#define DESCRIPTORWIDGET_H
#include <QGLWidget>

class Descriptor;
class DescriptorGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    DescriptorGLWidget(QWidget *parent = 0);
    ~DescriptorGLWidget();

    void setActiveDescriptor(Descriptor* pd){ m_pCompareDesc = pd;}
    void setControlDescriptor(Descriptor* pd){ m_pControlDesc = pd;}
    void clearControlDescriptor(){ m_pControlDesc = 0; update(); }

    GLfloat xRotation() const { return xRot; }
    GLfloat yRotation() const { return yRot; }
    GLfloat zRotation() const { return zRot; }
    GLfloat hZoom() const { return heightZoom; }


public slots:
    void setXRotation(GLfloat angle);
    void setYRotation(GLfloat angle);
    void setZRotation(GLfloat angle);
    void setHeightZoom(GLfloat factor);

signals:
    void xRotationChanged(GLfloat angle);
    void yRotationChanged(GLfloat angle);
    void zRotationChanged(GLfloat angle);
    void heightZoomChanged(GLfloat factor);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void drawBar(GLfloat x, GLfloat z, GLfloat h );

    GLfloat heightZoom;
    GLfloat xRot;
    GLfloat yRot;
    GLfloat zRot;

    QPoint lastPos;
    Descriptor *m_pControlDesc;
    Descriptor *m_pCompareDesc;
};

#endif // DESCRIPTORWIDGET_H
