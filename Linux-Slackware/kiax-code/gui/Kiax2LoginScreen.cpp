#include "Kiax2LoginScreen.h"

Kiax2LoginScreen::Kiax2LoginScreen(QWidget* parent) : QFrame(parent)
{
	ui.setupUi(this);
	connect(ui.getAccountButton, SIGNAL(clicked()), this, SLOT(getAccount()) );
	connect(ui.loginButton, SIGNAL(clicked()), this, SLOT(loginPressed()) );
	connect(ui.usernameLineEdit, SIGNAL(returnPressed()), this, SLOT(loginPressed()) );
	connect(ui.passwordLineEdit, SIGNAL(returnPressed()), this, SLOT(loginPressed()) );
}

Kiax2LoginScreen::~Kiax2LoginScreen()
{
} 

void Kiax2LoginScreen::getAccount()
{
	emit account();
}

void Kiax2LoginScreen::loginPressed()
{
	emit login(ui.usernameLineEdit->text(), ui.passwordLineEdit->text());
}

void Kiax2LoginScreen::setUsername(QString username)
{
	ui.usernameLineEdit->setText(username);
}

void Kiax2LoginScreen::setLoginStatus(QString status)
{
	ui.loginStatusLabel->setText(status);
}

void Kiax2LoginScreen::setPassword(QString password)
{
	ui.passwordLineEdit->setText(password);
}
