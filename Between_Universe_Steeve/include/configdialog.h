#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
class Game;
class Player;

class QCheckBox;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QComboBox;
class QColorDialog;

namespace Ui {
class ConfigDialog;
}

/**
 * @brief Form used for editing a player
 */
class PlayerWidget : public QWidget{

	Q_OBJECT

	QCheckBox* enableChkBox;
	QLineEdit* nameEdit;
	QPushButton* colorBtn;
	QColorDialog* colorDialog;
	QSpinBox* lifeSB;
	QComboBox* shipBox;
	QComboBox* weaponBox;

	Player* p;
	bool mandatory;
public:
	PlayerWidget(Player* p, bool enabled, bool mandatory);
	~PlayerWidget();
	void save();
public slots:
	void setAllEnabled(int state);
	void updateColor();
};

/**
 * @brief Form used for editing the players and the game
 */
class ConfigDialog : public QDialog
{
	Q_OBJECT

	friend class Game;

	QList<PlayerWidget*> widgets;

public:
	explicit ConfigDialog(QWidget *parent = 0);
	void clearPlayers();
	void addPlayer(Player *p, bool mandatory, bool enable);
	void save();
	~ConfigDialog();

	void setLevelFile(QString file);
	QString levelFile() const;
private slots:
	void on_exitBtn_clicked();

	void on_buttonBox_accepted();

	void on_buttonBox_rejected();

	void on_exitBtn_pressed();

	void on_fileBtn_clicked();

private:
	Ui::ConfigDialog *ui;

signals:
	void exitAsked();
	void accepted();
	void refused();
};

#endif // CONFIGDIALOG_H
