#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"
#include "glwidget.h"
class Mode;

using namespace std;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	GLWidget* w1;

	Game game;

public:
	explicit MainWindow( QPoint position, QSize size, QWidget *parent = 0);
	explicit MainWindow( QWidget *parent = 0);
	~MainWindow();

public slots:
	void updateGame();
	void updateRenderer();
};

#endif // MAINWINDOW_H
