#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w(QPoint(0,0),QSize(500,500));
	w.show();

	return a.exec();
}
