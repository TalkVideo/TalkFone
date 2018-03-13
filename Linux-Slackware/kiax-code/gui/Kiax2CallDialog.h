#ifndef KIAX2CALLDIALOG_H
#define KIAX2CALLDIALOG_H

#include "ui_calldialog.h"
#include "APIBinder.h"
#include <QTimer>
#include <QDateTime>
#include <QDialog>
#include "Kiax2CallAppearance.h"

class Kiax2CallDialog : public Kiax2CallAppearance
{

  Q_OBJECT

public:
  Kiax2CallDialog(QWidget* parent, APIBinder* binder, Contact* contactToCall = 0, int cNum = -1);
  virtual ~Kiax2CallDialog();

public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void hideDTMF(bool hideDtmf);
  virtual void dtmf0();
  virtual void dtmf1();
  virtual void dtmf2();
  virtual void dtmf3();
  virtual void dtmf4();
  virtual void dtmf5();
  virtual void dtmf6();
  virtual void dtmf7();
  virtual void dtmf8();
  virtual void dtmf9();
  virtual void dtmfStar();
  virtual void dtmfHash();
  virtual void setMicVolume(int level);
  virtual void setAudioVolume(int level);
  virtual void setAlwaysOnTop(bool always);

	void callDurationUpdatedSlot(QString durationString);
	void callStateNoneSlot();
	void callStateCompleteSlot();
	void callStateInactiveSlot();
	void callDirectionIncomingSlot();
	void callDirectionOutgoingSlot();
	void guiHungUpSlot();
	void guiDialedSlot();
	void guiMutedSlot();
	void guiUnmutedSlot();

	void dialogClosedSlot();
	void dialogShownSlot();
	void dialogChangedSlot(bool active);

protected slots:
  /*$PROTECTED_SLOTS$*/

public:
	void setInputOutputLevels(float inputLevel, float outputlevel);
	int getMicSliderValue();
	int getAudioSliderValue();
	void setMicSliderValue(int value);
	void setAudioSliderValue(int value);


private:
    QString decodeExtension(QString number, bool decodeHost, bool decodeExtension);
	QDialog* dialogAppearance;
	Ui::CallDialog ui;
	
};

#endif

