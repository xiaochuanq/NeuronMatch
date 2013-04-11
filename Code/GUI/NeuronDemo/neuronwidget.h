#ifndef NEURONWIDGET_H
#define NEURONWIDGET_H
#include <QGLWidget>
class Neuron;
class NeuronNode;

class NeuronGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    NeuronGLWidget(QWidget *parent = 0);
    ~NeuronGLWidget();
    static const int SHOWNEURON = 1;
    static const int SHOWSAMPLE = 2;
    static const int SHOWAXIS = 4;
    static const int SHOWMESH = 8;

    void setActiveNeuron(Neuron* pn){ m_pCompareNeuron = pn; }
    void setControlNeuron(Neuron* pn){ m_pControlNeuron = pn; }
    void setToDraw( int flag ){ m_nDrawFlag = flag; update(); }
    int getToDraw() { return m_nDrawFlag; }
    void clearControlNeuron(){ m_pControlNeuron = 0; update(); }

    GLfloat xRotation() const { return xRot; }
    GLfloat yRotation() const { return yRot; }
    GLfloat zRotation() const { return zRot; }
    GLfloat zoomFactor() const {return zoom; }

public slots:
    void setXRotation(GLfloat angle);
    void setYRotation(GLfloat angle);
    void setZRotation(GLfloat angle);
    void setZoomFactor(GLfloat z);

signals:
    void xRotationChanged(GLfloat angle);
    void yRotationChanged(GLfloat angle);
    void zRotationChanged(GLfloat angle);
    void zoomFactorChanged(GLfloat z);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void drawMesh();
    void drawAxis();
    void drawNeuron( );
    void drawCloud();
public:
    static void drawEdge(GLfloat x0, GLfloat y0, GLfloat z0, GLfloat r0,
            GLfloat x1, GLfloat y1, GLfloat z1, GLfloat r1);
    static void drawSphere(GLfloat x0, GLfloat y0, GLfloat z0, GLfloat r0);
    static void drawNeuronNode(const NeuronNode*, const NeuronNode*);
    static void drawRay(GLfloat x, GLfloat y, GLfloat z,
                 const GLfloat* reflectanceColor,
                 GLfloat w, GLushort pattern);
private:
    GLfloat xRot;
    GLfloat yRot;
    GLfloat zRot;
    GLfloat zoom;
    int m_nDrawFlag;
    QPoint lastPos;
    Neuron * m_pCompareNeuron;
    Neuron * m_pControlNeuron;
};

#endif // NEURONWIDGET_H
