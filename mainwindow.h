#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "world.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent*) override;
private slots:
    void on_actionRun_triggered();
    void timerUpdate();

    void on_actionQuit_triggered();

    void on_actionLoad_triggered();

private:
    Ui::MainWindow *ui;
    QPointF zoom={1.0,1.0};
    World *world;
    QTimer *animationTimer;
    short blinkStep=0;
};
#endif // MAINWINDOW_H
