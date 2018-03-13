#include "Kiax2ChangePassDialog.h"
#include <QString>

Kiax2ChangePassDialog::Kiax2ChangePassDialog(QWidget* parent, QString serviceUrl) : QDialog(parent)
{
	ui.setupUi(this);
	url = serviceUrl;

	connect(ui.registerWebView, SIGNAL(loadStarted()), this, SLOT(loadStarted()) );
	connect(ui.registerWebView, SIGNAL(loadFinished()), this, SLOT(loadFinished()) );
	//connect(ui.closeButton, SIGNAL(clicked()), this, SLOT(accept()) );
    connect(ui.registerWebView->page()->networkAccessManager(), SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),
            SLOT(slotSslErrors(QNetworkReply*, const QList<QSslError>&)));	
	
	state = STATE_CHANGE_NONE;
	resize(CHANGE_PASS_DIALOG_WIDTH, CHANGE_PASS_DIALOG_HEIGHT); 
}

Kiax2ChangePassDialog::~Kiax2ChangePassDialog()
{
}

void Kiax2ChangePassDialog::loadPage()
{
	state = STATE_CHANGE_NONE;
	url = url + "/" + username;
	Logger::log(Logger::DEBUG, "change pass URL is %s\n", url.toStdString().data());
	QUrl qurl(url);
	ui.registerWebView->load(qurl);	
}

void Kiax2ChangePassDialog::setUsername(QString userlogin)
{
	username = userlogin;
}

void Kiax2ChangePassDialog::loadStarted()
{
	if ((state == STATE_CHANGE_PROCESS_SUCCESSFUL)||(state == STATE_CHANGE_PROCESS_ERROR))
		state = STATE_CHANGE_PROCESS;
	Logger::log(Logger::DEBUG, "loadStarted: state = %d\n", state);
	switch (state)
	{
		case STATE_CHANGE_NONE:
		{
			state = STATE_CHANGE_LOAD_PROMPT;
			ui.statusLabel->setText(tr("Loading.."));
			Logger::log(Logger::DEBUG, "loadStarted: state = %d\n", state);
			break;
		}
		case STATE_CHANGE_PROMPT:
		{
			state = STATE_CHANGE_PROCESS;
			ui.statusLabel->setText(tr("Password change in progress.."));
			Logger::log(Logger::DEBUG, "loadStarted: state = %d\n", state);
			break;
		}
	}
}

void Kiax2ChangePassDialog::loadFinished()
{
	Logger::log(Logger::DEBUG, "loadFinished: state = %d\n", state);
	switch (state)
	{
		case STATE_CHANGE_LOAD_PROMPT:
		{
			state = STATE_CHANGE_PROMPT;
			ui.statusLabel->setText("");
			Logger::log(Logger::DEBUG, "loadFinished: state = %d\n", state);
			break;
		}
		case STATE_CHANGE_PROCESS:
		{
			QString result = ui.registerWebView->page()->mainFrame()->evaluateJavaScript(FORM_NAME_JS_EXPRESSION).toString();
			Logger::log(Logger::DEBUG, "Process Result = %s\n", result.toStdString().data());
			QString username = ui.registerWebView->page()->mainFrame()->evaluateJavaScript(USERNAME_JS_EXPRESSION).toString();
			QString password = ui.registerWebView->page()->mainFrame()->evaluateJavaScript(PASSWORD_JS_EXPRESSION).toString();
			Logger::log(Logger::DEBUG, "Username = %s, Password = %s\n", username.toStdString().data(), password.toStdString().data());
			state = STATE_CHANGE_PROCESS_SUCCESSFUL;
			if (result.compare(PASS_CHANGE_SUCCESS_FORM_NAME)==0)
			{
				ui.statusLabel->setText(tr("Password changed successfully."));
				emit passwordChangeSuccessful(username, password);
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

void Kiax2ChangePassDialog::slotSslErrors(QNetworkReply* replay, const QList<QSslError>& error)
{
    replay->ignoreSslErrors();
}
