#include "Kiax2ContactDialog.h"
#include "PConfiguration.h"

Kiax2ContactDialog::Kiax2ContactDialog(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
#if defined(MACXDEP)
	setWindowFlags(Qt::Sheet);
#endif		
	connect (ui.saveButton, SIGNAL(pressed()), this, SLOT(verifyAccept()));
	connect (ui.cancelButton, SIGNAL(pressed()), this, SLOT(reject()));
}

Kiax2ContactDialog::~Kiax2ContactDialog()
{
}

void Kiax2ContactDialog::verifyAccept()
{
#ifdef USE_DOTTEL
	if (ALLOW_ONLY_TEL&&(!ui.extensionEdit->text().endsWith(".tel")))
	{
		ui.extensionEdit->setText(ui.extensionEdit->text() + ".tel");
	}
#endif
	accept();
}

std::string Kiax2ContactDialog::getContactNameText()
{
	return ui.contactNameEdit->text().toStdString();
}

std::string Kiax2ContactDialog::getExtensionText()
{
	return ui.extensionEdit->text().toStdString();
}

void Kiax2ContactDialog::setContactNameText(std::string text)
{
	return ui.contactNameEdit->setText(QString(text.data()));
}

void Kiax2ContactDialog::setExtensionText(std::string text)
{
	return ui.extensionEdit->setText(QString(text.data()));
}
