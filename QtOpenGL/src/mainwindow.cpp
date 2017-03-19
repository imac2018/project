#include "mainwindow.h"
#include <QSettings>
#include <QApplication>
#include <QCloseEvent>


void MainWindow::closeEvent(QCloseEvent *)
{
	settings.setValue("fullscreen",isFullScreen());
	settings.setValue("maximized",isMaximized());
	settings.setValue("width",width());
	settings.setValue("height",height());
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
	QMainWindow::keyPressEvent(e);
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	settings("PineappleSquad",
			 qAppName(),this)
{
	this->setWindowTitle("KIWK-I-MAC");
	QGLFormat glFormat;
#ifdef WIN_32
	timer->start(1000/50);
#else
	//glFormat.setVersion( 3, 3 );
#endif
    glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setSampleBuffers( true );

	w = new GLWidget(glFormat,this);
	this->setCentralWidget(w);
}

MainWindow::~MainWindow()
{}

void MainWindow::showNormal()
{
	settings.setValue("fullscreen",false);
	QMainWindow::showNormal();
}

void MainWindow::showFullScreen()
{
	settings.setValue("fullscreen",true);
	QMainWindow::showNormal();
}

void MainWindow::show()
{

	if(settings.value("fullscreen","false").toBool()){
		QMainWindow::showFullScreen();
	}
	else{
		if(!settings.value("maximized","false").toBool()){
			resize(settings.value("width","640").toInt(),
						 settings.value("height","360").toInt());
			QMainWindow::showNormal();
		}
		else{
			QMainWindow::showMaximized();
		}
	}
}
