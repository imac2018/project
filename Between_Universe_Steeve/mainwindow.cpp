#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "renderer.h"

MainWindow::MainWindow(QPoint position, QSize size, QWidget *parent) :
	QMainWindow(parent)
{
	QWidget* w = new Renderer(this);
	this->setCentralWidget(w);
	this->resize(size);
	this->move(position);
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	QWidget* w = new Renderer(this);
	this->setCentralWidget(w);
}

MainWindow::~MainWindow()
{
}
