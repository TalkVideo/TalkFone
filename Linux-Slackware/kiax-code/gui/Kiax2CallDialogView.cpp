#include "Kiax2CallDialogView.h"
#include "Logger.h"

Kiax2CallDialogView::Kiax2CallDialogView(QWidget* parent) : QDialog(NULL)
{
}

Kiax2CallDialogView::~Kiax2CallDialogView()
{
	Logger::log(Logger::DEBUG, "destoroyed call view\n");
}

void Kiax2CallDialogView::showEvent ( QShowEvent * event )
{
	emit dialogShown();
}

void Kiax2CallDialogView::closeEvent(QCloseEvent* event)
{
	emit dialogClosed();
	event->accept();
}

void Kiax2CallDialogView::reject()
{
	emit dialogClosed();
	QDialog::reject();
}

void Kiax2CallDialogView::changeEvent(QEvent* event)
{
	bool active = false;
	
	if (event->type()==QEvent::ActivationChange)
		if (window()->isActiveWindow())
			active = true;
		
	emit dialogChanged(active);
}
