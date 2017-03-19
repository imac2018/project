#include <QApplication>
#include <QMessageBox>

#include "mainwindow.h"
#include "glexception.h"

#include <assimp/scene.h>
#include <iostream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow* w = NULL;
	try{
		w = new MainWindow();
		w->show();
		return a.exec();
	}
	catch(GLException e){
		QMessageBox::critical(w,e.title,e.text,QMessageBox::Ok);
		if(w)
			w->close();
		a.quit();
	}
	return 1;

}
