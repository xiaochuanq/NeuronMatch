#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QFileSystemModel>
#include <descriptorwidget.h>
#include <neuronwidget.h>
#include "neuronmanager.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QFileSystemModel * fileModel;
    QSplitter *splitter;
    NeuronGLWidget *neuronWidget;
    DescriptorGLWidget *descWidget;
    NeuronManager neuronMgr;
private slots:
    void on_actionZoom_Out_triggered();
    void on_actionZoom_In_triggered();
    void on_actionLower_Histogram_triggered();
    void on_actionHigher_Histogram_triggered();
    void on_actionSample_triggered(bool checked);
    void on_actionNeuron_triggered(bool checked);
    void on_actionDeselect_Control_triggered();
    void on_actionMesh_triggered(bool checked);
    void on_actionAxis_triggered(bool checked);
    void on_treeView_doubleClicked(QModelIndex index);
    void on_treeView_clicked(QModelIndex index);
};

#endif // MAINWINDOW_H
