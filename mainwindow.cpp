#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    world = new World("config.json");
    zoom=QPointF(1.0,1.0);
    animationTimer = new QTimer(this);
    connect(animationTimer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
}

MainWindow::~MainWindow() {
    delete ui;
    delete world;
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QBrush whiteBrush(Qt::SolidPattern);
    whiteBrush.setColor(QColor(210,210,210));
    painter.fillRect(0,0,width(),height(),whiteBrush);
    world->draw(painter);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QPointF coef=zoom;
    zoom = QPointF(width(),height());
    world->scale(zoom.x()/coef.x(),zoom.y()/coef.y());
    update();
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    auto agent = world->findAgentIn(event->pos());
    if (agent) agent->onTap();
}


void MainWindow::on_actionRun_triggered() {
    world->start();
    animationTimer->start(50);
    update();
}

void MainWindow::timerUpdate() {
    if (++blinkStep>4) {
        world->changeBlinkState();
        blinkStep=0;
    }
    world->consume();
    //if (world->consume()) animationTimer->stop();
    update();
}


void MainWindow::on_actionQuit_triggered() {
    QApplication::quit();
}


void MainWindow::on_actionLoad_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this,"Load JSON file",".","*.json");
    zoom=QPointF(1.0,1.0);
    world->updateGraph(fileName);
    ui->statusbar->showMessage("Stats: |Agents|="+QString::number(world->getNumberOfAgents())+
                               "  |Links|="+QString::number(world->getNumberOfLinks()));
    QPointF coef=zoom;
    zoom = QPointF(width(),height());
    world->scale(zoom.x()/coef.x(),zoom.y()/coef.y());
    update();
}

