#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "renderer.h"
#include "renderergl.h"
#include "rendererwidget.h"
#include <QDockWidget>
#include "galata.h"
#include "ship.h"
#include <functional>
#include "player.h"
#include <QTimer>
#include <QDebug>
#include "weapon.h"

MainWindow::MainWindow(QPoint position, QSize size, QWidget *parent) :
	MainWindow(parent)
{

	this->resize(size);
	this->move(position);
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{

	Player* p1 = new Player();

	p1->addControl(QKeyEvent(QEvent::KeyRelease,Qt::Key_Down,Qt::NoModifier),
				   &ShipActions::stopDown);
	p1->addControl(QKeyEvent(QEvent::KeyRelease,Qt::Key_Up,Qt::NoModifier),
				   &ShipActions::stopUp);
	p1->addControl(QKeyEvent(QEvent::KeyRelease,Qt::Key_Left,Qt::NoModifier),
				   &ShipActions::stopBackward);
	p1->addControl(QKeyEvent(QEvent::KeyRelease,Qt::Key_Right,Qt::NoModifier),
				   &ShipActions::stopForward);

	p1->addControl(QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::NoModifier),
				   &ShipActions::moveDown);
	p1->addControl(QKeyEvent(QEvent::KeyPress,Qt::Key_Up,Qt::NoModifier),
				   &ShipActions::moveUp);
	p1->addControl(QKeyEvent(QEvent::KeyPress,Qt::Key_Left,Qt::NoModifier),
				   &ShipActions::moveBackward);
	p1->addControl(QKeyEvent(QEvent::KeyPress,Qt::Key_Right,Qt::NoModifier),
				   &ShipActions::moveForward);

	p1->addControl(QKeyEvent(QEvent::KeyRelease,Qt::Key_Space,Qt::NoModifier),
				   &ShipActions::mainWeaponStop);
	p1->addControl(QKeyEvent(QEvent::KeyPress,Qt::Key_Space,Qt::NoModifier),
				   &ShipActions::mainWeaponActivate);


	Weapon* weapon1 = new Weapon(QPointF(20,20));
	Weapon* weapon2 = new Weapon(QPointF(20,20));
	Ship* ship = new Ship(6,2,3,4,5,6,weapon1,weapon2);
	p1->setShip(ship);

	current = new Galata(p1);

	w1 = new GLWidget(*(current->renderer()),this);
	QWidget* w2 = new Widget(*(current->renderer()),this);
	Qt::WindowFlags windowFlag = Qt::FramelessWindowHint;
	QDockWidget* dockleft = new QDockWidget("Left RenderGL",0,windowFlag),
				*dockright = new QDockWidget("Right RenderWidget",0,windowFlag);
	dockleft->setWidget(w1);
	dockright->setWidget(w2);

	dynamic_cast<Galata*>(current)->addEnemy(new SquareEnemy(12,6,QPointF(100,250),
												QPair<QPointF,QPointF>(QPointF(0,0),QPointF(100,100)),QSizeF(20,20)));



	QTimer* timer = new QTimer(this);
	QObject::connect(timer,SIGNAL(timeout()),this,SLOT(updateGame()));
	timer->start(1000/50);

	this->addDockWidget(Qt::RightDockWidgetArea,dockright);
	this->addDockWidget(Qt::LeftDockWidgetArea,dockleft);


	qApp->installEventFilter(this);
}

MainWindow::~MainWindow()
{
}

void MainWindow::updateGame()
{
	if(current != NULL){
		if(current->update()){
			this->update();
			w1->update();
		}
	}
}

bool MainWindow::eventFilter(QObject *, QEvent * event)
{
	QKeyEvent* e = dynamic_cast<QKeyEvent*>(event);
	if(e != NULL){
		current->inputHandle(e);
		return true;
	}
	return false;
}
