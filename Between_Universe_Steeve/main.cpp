#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w(QPoint(0,0),QSize(500,500));
	try{
		w.show();
		return a.exec();
	}
	catch(std::exception e){
		w.close();
		QMessageBox::critical(NULL,"Error!", e.what(), QMessageBox::Ok);
	}
	return 1;
}
