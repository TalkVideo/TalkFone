#ifndef KIAX2CALLBUTTON_H
#define KIAX2CALLBUTTON_H

#include "ui_calldialog.h"
#include "APIBinder.h"
#include <QTimer>
#include <QDateTime>
#include <QPushButton>
#include <QRadioButton>
#include "Kiax2CallAppearance.h"

class Kiax2CallButton : public Kiax2CallAppearance
{

  Q_OBJECT

public:
  Kiax2CallButton(QWidget* parent, APIBinder* binder, Contact* contactToCall = 0, int cNum = -1);
  virtual ~Kiax2CallButton();

public slots:
  /*$PUBLIC_SLOTS$*/

	void callDurationUpdatedSlot(QString durationString);
	void callStateNoneSlot();
	void callStateCompleteSlot();
	void callStateInactiveSlot();
	void callDirectionIncomingSlot();
	void callDirectionOutgoingSlot();
	void guiHungUpSlot();
	void guiMutedSlot();
	void guiUnmutedSlot();
	void toggledSlot(bool toggled);

protected slots:
  /*$PROTECTED_SLOTS$*/

public:


private:
	QString getButtonText();
	QPushButton* buttonAppearance;
	QString firstRowText;
	QString secondRowText;
};

#endif

