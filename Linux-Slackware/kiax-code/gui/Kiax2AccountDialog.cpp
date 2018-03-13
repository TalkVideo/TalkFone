#include "Kiax2AccountDialog.h"
#include <QString>
#include "Kiax2MainWindow.h"

Kiax2AccountDialog::Kiax2AccountDialog(QWidget* parent) : QDialog(parent)
{ 
	ui.setupUi(this);
#if defined(MACXDEP)
	setWindowFlags(Qt::Sheet); 
#endif	
	connect(ui.getAccountButton, SIGNAL(clicked()), this, SLOT(getAccount()) );
	ui.serverLabel->setVisible(ENABLE_SERVER_FIELD);
	ui.serverEdit->setVisible(ENABLE_SERVER_FIELD);
	ui.getAccountButton->setVisible(!ENABLE_SERVER_FIELD);
} 

Kiax2AccountDialog::~Kiax2AccountDialog()
{
}

void Kiax2AccountDialog::getAccount()
{
	reject();
	((Kiax2MainWindow*)parent())->registerAction_activated();
}

std::string Kiax2AccountDialog::getUsernameText()
{
	return ui.usernameEdit->text().toStdString();
}
 
std::string Kiax2AccountDialog::getServerText()
{
	return ui.serverEdit->text().toStdString();
}

std::string Kiax2AccountDialog::getPasswordText()
{
	return ui.passwordEdit->text().toStdString();
}

void Kiax2AccountDialog::setUsernameText(std::string text)
{
	return ui.usernameEdit->setText(QString(text.data()));
}

void Kiax2AccountDialog::setServerText(std::string text)
{
	return ui.serverEdit->setText(QString(text.data()));
}

void Kiax2AccountDialog::setPasswordText(std::string text)
{
	return ui.passwordEdit->setText(QString(text.data()));
}
