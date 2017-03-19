#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glwidget.h"
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	GLWidget* w;

	QSettings settings;

protected:
	void closeEvent(QCloseEvent* e);
	virtual void keyPressEvent( QKeyEvent* e );

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void showNormal();
	void showFullScreen();
	void show();
};

#endif // MAINWINDOW_H
