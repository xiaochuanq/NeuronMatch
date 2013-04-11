#include <QFileDialog>
#include <QStringList>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    neuronWidget = new NeuronGLWidget(this);
    descWidget = new DescriptorGLWidget(this);

    splitter = new QSplitter(this);
    splitter->addWidget(neuronWidget);
    splitter->addWidget(descWidget);

    this->setCentralWidget(splitter);

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot |
                         QDir::Files | QDir::Dirs);
    QStringList *filter = new QStringList();
    filter->append("*.swc");
    filter->append("*.SWC");
    fileModel->setNameFilters(*filter);
    fileModel->setRootPath(QDir::homePath());
    ui->treeView->setModel(fileModel);
    int nCol = fileModel->columnCount();
    for( int i = 1; i < nCol; ++i)
        ui->treeView->setColumnHidden(i, true);

//    QString filename = "/Users/xiaochuanq/Documents/workspace/NeuronMatch/Data/Testing/Granule/727884.CNG.swc";
//    NeuronEntry ne = neuronMgr.add( filename.toStdString());
//    if( ne.neuron){
//        neuronWidget->setActiveNeuron( ne.neuron);
//        neuronWidget->update();
//    }
//    if( ne.desc){
//        descWidget->setActiveDescriptor(ne.desc);
//        descWidget->update();
//    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_treeView_clicked(QModelIndex index)
{
    QFileInfo fileInfo = fileModel->fileInfo(index);
    if( fileInfo.isFile() && fileInfo.suffix().toLower() == "swc"){
        NeuronEntry ne;
        string filename(fileInfo.absoluteFilePath().toStdString());
        if( neuronMgr.notContains( filename )){
            ne = neuronMgr.add( filename );
        }
        else{
            ne = neuronMgr.get(filename);
        }
        if( ne.neuron){
            neuronWidget->setActiveNeuron( ne.neuron);
            neuronWidget->update();
        }
        if( ne.desc){
            descWidget->setActiveDescriptor(ne.desc);
            descWidget->update();
        }
    }
}

void MainWindow::on_treeView_doubleClicked(QModelIndex index)
{
    QFileInfo fileInfo = fileModel->fileInfo(index);
    if( fileInfo.isFile() && fileInfo.suffix().toLower() == "swc"){
        NeuronEntry ne;
        string filename(fileInfo.absoluteFilePath().toStdString());
        if( neuronMgr.notContains(filename)){
             ne = neuronMgr.add( filename);
        }
        else{
            ne = neuronMgr.get(filename);
        }
        if( ne.neuron){
            neuronWidget->setControlNeuron( ne.neuron);
            neuronWidget->update();
        }
        if( ne.desc){
            descWidget->setControlDescriptor(ne.desc);
            descWidget->update();
        }
    }
}


void MainWindow::on_actionNeuron_triggered(bool checked)
{
    int flag = neuronWidget->getToDraw();
    if( checked){
        flag ^= NeuronGLWidget::SHOWNEURON;
    }
    else{
        flag |= NeuronGLWidget::SHOWNEURON;
    }
    neuronWidget->setToDraw(flag);
}

void MainWindow::on_actionSample_triggered(bool checked)
{
    int flag = neuronWidget->getToDraw();
    if( checked){
        flag ^= NeuronGLWidget::SHOWSAMPLE;
    }
    else{
        flag |= NeuronGLWidget::SHOWSAMPLE;
    }
    neuronWidget->setToDraw(flag);
}

void MainWindow::on_actionAxis_triggered(bool checked)
{
    int flag = neuronWidget->getToDraw();
    if( checked){
        flag ^= NeuronGLWidget::SHOWAXIS;
    }
    else{
        flag |= NeuronGLWidget::SHOWAXIS;
    }
    neuronWidget->setToDraw(flag);
}

void MainWindow::on_actionMesh_triggered(bool checked)
{
    int flag = neuronWidget->getToDraw();
    if( checked){
        flag ^= NeuronGLWidget::SHOWMESH;
    }
    else{
        flag |= NeuronGLWidget::SHOWMESH;
    }
    neuronWidget->setToDraw(flag);
}

void MainWindow::on_actionDeselect_Control_triggered()
{
    neuronWidget->clearControlNeuron();
    descWidget->clearControlDescriptor();
}

void MainWindow::on_actionHigher_Histogram_triggered()
{
    descWidget->setHeightZoom( descWidget->hZoom()* 2);
}

void MainWindow::on_actionLower_Histogram_triggered()
{
    descWidget->setHeightZoom( descWidget->hZoom()*0.5);
}

void MainWindow::on_actionZoom_In_triggered()
{
    neuronWidget->setZoomFactor( neuronWidget->zoomFactor() * 1.1);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    neuronWidget->setZoomFactor( neuronWidget->zoomFactor() / 1.1);
}
