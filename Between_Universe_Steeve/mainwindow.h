#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class Mode;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	Mode* current;
	QWidget* w1;

public:
	explicit MainWindow( QPoint position, QSize size, QWidget *parent = 0);
	explicit MainWindow( QWidget *parent = 0);
	~MainWindow();

public slots:
	void updateGame();
protected:
	bool eventFilter(QObject *, QEvent *event);
};

#endif // MAINWINDOW_H
