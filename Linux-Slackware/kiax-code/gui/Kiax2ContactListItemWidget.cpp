#include "Kiax2ContactListItemWidget.h"

Kiax2ContactListItemWidget::Kiax2ContactListItemWidget(QWidget* parent) : QWidget(parent)
{
	ui.setupUi(this);
	show();
	//connect (ui.okButton, SIGNAL(pressed()), this, SLOT(accept()));
}

Kiax2ContactListItemWidget::~Kiax2ContactListItemWidget()
{
}
