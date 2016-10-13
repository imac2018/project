#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "renderer.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	QWidget* w = new Renderer(this);
	this->setCentralWidget(w);
}

MainWindow::~MainWindow()
{
}
