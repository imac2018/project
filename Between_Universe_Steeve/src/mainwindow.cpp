#include <QDockWidget>
#include <functional>
#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "renderer.h"
#include "glwidget.h"
#include "player.h"
#include "galata.h"
#include "ship.h"
#include "weapon.h"
#include "item.h"

MainWindow::MainWindow(QPoint position, QSize size, QWidget *parent) :
	MainWindow(parent)
{

	this->resize(size);
	this->move(position);
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{

	w1 = new GLWidget(game.currentRenderer(),this);
	this->setCentralWidget(w1);
	QTimer* timer = new QTimer(this);
	timer->start(1000/50);

	QObject::connect(timer,SIGNAL(timeout()),this,SLOT(updateGame()));
	QObject::connect(&game,SIGNAL(modeHasChanged()),
					 this,SLOT(updateRenderer()));

	this->installEventFilter(&game);
}

MainWindow::~MainWindow()
{
}

void MainWindow::updateGame()
{
	if(game.updateGame()){
		w1->update();
	}
}

void MainWindow::updateRenderer()
{
	w1->renderer = game.currentRenderer();
}
