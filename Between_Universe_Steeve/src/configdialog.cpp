#include <QVBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include "game.h"
#include "configdialog.h"
#include "ui_configdialog.h"
#include "player.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ConfigDialog)
{
	ui->setupUi(this);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Reload");
}

void ConfigDialog::clearPlayers()
{
	foreach(PlayerWidget* pw, widgets){
		this->layout()->removeWidget(pw);
		delete pw;
	}
	widgets.clear();
}

void ConfigDialog::addPlayer(Player* p,
							 bool mandatory,
							 bool enable)
{
	PlayerWidget* pw = new PlayerWidget(p,enable,mandatory);
	ui->mainWidget->layout()->addWidget(pw);
	widgets.append(pw);

}

void ConfigDialog::setLevelFile(QString file){
	ui->levelPathEdit->setText(file);
}

QString ConfigDialog::levelFile() const{
	if(!QFile(ui->levelPathEdit->text()).exists() ||
			!ui->levelPathEdit->text().endsWith("xml")){
		return QString();
	}
	return ui->levelPathEdit->text();
}
void ConfigDialog::save()
{
	foreach(PlayerWidget* pw, widgets){
		pw->save();
	}
	if(!QFile(ui->levelPathEdit->text()).exists() ||
			!ui->levelPathEdit->text().endsWith("xml")){
		QMessageBox::warning(this,"Warning",
							 "The File " + ui->levelPathEdit->text() +
							 " is not a correct level file", QMessageBox::Ok);
	}

}

ConfigDialog::~ConfigDialog()
{
	delete ui;
}

void ConfigDialog::on_exitBtn_clicked()
{
	emit exitAsked();
}

void ConfigDialog::on_buttonBox_accepted()
{
	emit accepted();
}

void ConfigDialog::on_buttonBox_rejected()
{
	emit refused();
}

PlayerWidget::PlayerWidget(Player *p, bool enabled,
						   bool mandatory)
	: QWidget(), mandatory(mandatory), p(p)
{
	this->setLayout(new QVBoxLayout());

	enableChkBox = new QCheckBox();
	enableChkBox->setChecked(enabled || mandatory);
	enableChkBox->setEnabled(!mandatory);
	this->layout()->addWidget(enableChkBox);

	nameEdit = new QLineEdit(p->name());
	this->layout()->addWidget(nameEdit);

	colorBtn = new QPushButton();
	QPixmap pix(50,50);
	pix.fill(p->color());
	colorBtn->setIcon(pix);
	this->layout()->addWidget(colorBtn);

	colorDialog = new QColorDialog(p->color());

	QObject::connect(colorBtn,SIGNAL(clicked(bool)),
						colorDialog,SLOT(show()));
	QObject::connect(colorDialog,SIGNAL(accepted()),
						this,SLOT(updateColor()));

	lifeSB = new QSpinBox();
	lifeSB->setValue(p->life());
	this->layout()->addWidget(lifeSB);

	shipBox = new QComboBox();
	shipBox->setEditable(false);
	shipBox->addItems(Game::shipNames());
	shipBox->setCurrentText(p->shipModel->name());
	this->layout()->addWidget(shipBox);

	weaponBox = new QComboBox();
	weaponBox->setEditable(false);
	weaponBox->addItems(Game::weaponNames());
	weaponBox->setCurrentText(p->mainWeapon->name());
	this->layout()->addWidget(weaponBox);

	setAllEnabled(enableChkBox->checkState());

	QObject::connect(enableChkBox,SIGNAL(stateChanged(int)),
					 this,SLOT(setAllEnabled(int)));
}

PlayerWidget::~PlayerWidget()
{
	QWidget::~QWidget();
	delete enableChkBox;
	delete nameEdit;
	delete colorBtn;
	delete colorDialog;
	delete lifeSB;
	delete shipBox;
	delete weaponBox;
}

void PlayerWidget::save()
{
	p->setName(nameEdit->text());
	p->setColor(colorDialog->currentColor());
	p->setLife(lifeSB->value());
	p->putInGame(enableChkBox->isChecked());
	p->setShip(*(Game::shipModelFromName(shipBox->currentText())));
	p->setMainWeapon(*(Game::weaponModelFromName(weaponBox->currentText())));
}

void PlayerWidget::setAllEnabled(int state)
{
	if(mandatory)
		return;
	bool enabled = state == Qt::Checked;
	nameEdit->setEnabled(enabled);
	colorBtn->setEnabled(enabled);
	lifeSB->setEnabled(enabled);
	shipBox->setEnabled(enabled);
	weaponBox->setEnabled(enabled);
}

void PlayerWidget::updateColor()
{
	QPixmap pix(50,50);
	pix.fill(colorDialog->currentColor());
	colorBtn->setIcon(pix);
}

void ConfigDialog::on_exitBtn_pressed()
{

}


void ConfigDialog::on_fileBtn_clicked()
{
	QString path = QFileDialog::getOpenFileName(this,"Open Level",
												"","XML Files (*.xml)");
	ui->levelPathEdit->setText(path);
}
