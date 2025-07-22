#include "Kiax2AboutDialog.h"
#include "PConfiguration.h"
#include <QDesktopServices>
#include <QUrl>

Kiax2AboutDialog::Kiax2AboutDialog(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
#if defined(MACXDEP)
	setWindowFlags(Qt::Sheet);
#endif		
	ui.productLabel->setText(PRODUCT_NAME);
	ui.licenseLabel->setText(PRODUCT_LICENSE);
	ui.urlLabel->setText(ABOUT_URL);
	connect (ui.okButton, SIGNAL(pressed()), this, SLOT(accept()));
	connect (ui.urlLabel, SIGNAL(linkActivated(const QString &)), this, SLOT(urlActivated(const QString &)));
}

Kiax2AboutDialog::~Kiax2AboutDialog()
{
} 

void Kiax2AboutDialog::urlActivated(const QString & link)
{
	QDesktopServices::openUrl(QUrl(link));	
}
