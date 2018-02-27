#include "Kiax2RegisterDialog.h"
#include <QString>

Kiax2RegisterDialog::Kiax2RegisterDialog(QWidget* parent, QString serviceUrl) : QDialog(parent)
{
	ui.setupUi(this);
	//QString baseURL(serviceUrl);
	url = new QUrl(serviceUrl);
	connect(ui.registerWebView, SIGNAL(loadStarted()), this, SLOT(loadStarted()) );
	connect(ui.registerWebView, SIGNAL(loadFinished()), this, SLOT(loadFinished()) );
	//connect(ui.closeButton, SIGNAL(clicked()), this, SLOT(accept()) );
    connect(ui.registerWebView->page()->networkAccessManager(), SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),
            SLOT(slotSslErrors(QNetworkReply*, const QList<QSslError>&)));	
	state = STATE_REGISTER_NONE;
	resize(REGISTER_DIALOG_WIDTH, REGISTER_DIALOG_HEIGHT); 

}

Kiax2RegisterDialog::~Kiax2RegisterDialog()
{
	delete url;
}

void Kiax2RegisterDialog::loadPage()
{
	state = STATE_REGISTER_NONE;
	ui.registerWebView->load(*url);	
}

void Kiax2RegisterDialog::loadStarted()
{
	if ((state == STATE_REGISTER_PROCESS_SUCCESSFUL)||(state == STATE_REGISTER_PROCESS_ERROR))
		state = STATE_REGISTER_PROMPT;
	Logger::log(Logger::DEBUG, "loadStarted: state = %d\n", state);
	switch (state)
	{
		case STATE_REGISTER_NONE:
		{
			ui.statusLabel->setText(tr("Loading.."));
			state = STATE_REGISTER_LOAD_PROMPT;
			Logger::log(Logger::DEBUG, "loadStarted: state = %d\n", state);
			break;
		}
		case STATE_REGISTER_PROMPT:
		{
			state = STATE_REGISTER_PROCESS;
			ui.statusLabel->setText(tr("Registration is in progress.."));
			Logger::log(Logger::DEBUG, "loadStarted: state = %d\n", state);
			break;
		}
	}
}

void Kiax2RegisterDialog::loadFinished()
{
	Logger::log(Logger::DEBUG, "loadFinished: state = %d\n", state);
	switch (state)
	{
		case STATE_REGISTER_LOAD_PROMPT:
		{
			state = STATE_REGISTER_PROMPT;
			Logger::log(Logger::DEBUG, "loadFinished: state = %d\n", state);
			ui.statusLabel->setText("");
			break;
		}
		case STATE_REGISTER_PROCESS:
		{
			QString result = ui.registerWebView->page()->mainFrame()->evaluateJavaScript(FORM_NAME_JS_EXPRESSION).toString();
			Logger::log(Logger::DEBUG, "Process Result = %s\n", result.toStdString().data());
			QString username = ui.registerWebView->page()->mainFrame()->evaluateJavaScript(USERNAME_JS_EXPRESSION).toString();
			QString password = ui.registerWebView->page()->mainFrame()->evaluateJavaScript(PASSWORD_JS_EXPRESSION).toString();
			Logger::log(Logger::DEBUG, "Username = %s, Password = %s\n", username.toStdString().data(), password.toStdString().data());
			state = STATE_REGISTER_PROCESS_SUCCESSFUL;
			if (result.compare(REGISTRATION_SUCCESS_FORM_NAME)==0)
			{
				ui.statusLabel->setText(tr("Registration successful."));
				emit registrationSuccessful(username, password);
			}
			else
			{
				ui.statusLabel->setText(tr("Please enter correct data.."));
			}
			Logger::log(Logger::DEBUG, "loadFinished: state = %d\n", state);
			break;
		}
	}
}

void Kiax2RegisterDialog::slotSslErrors(QNetworkReply* replay, const QList<QSslError>& error)
{
    replay->ignoreSslErrors();
}
