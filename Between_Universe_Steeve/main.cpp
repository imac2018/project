#include "mainwindow.h"
#include "colliderobject.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w(QPoint(0,0),QSize(500,500));
	w.showFullScreen();/*
	ColliderLine AB(QLine(50,50,70,70));
	ColliderLine CD(QLine(30,60,100,70));
	ColliderLine EF(QLine(70,60,80,80));
	ColliderRect rect(QRect(60,50,50,70));

	std::cout << "collision :" << AB.collisionWithObject(CD) << std::endl;
	std::cout << "collision :" << CD.collisionWithObject(AB) << std::endl;

	std::cout << "collision :" << AB.collisionWithObject(rect) << std::endl;
	std::cout << "collision :" << rect.collisionWithObject(AB) << std::endl;

	std::cout << "collision :" << EF.collisionWithObject(rect) << std::endl;
	std::cout << "collision :" << rect.collisionWithObject(EF) << std::endl;*/

	return a.exec();
	return 0;
}
