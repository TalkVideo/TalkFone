#include "Kiax2ListWidgetItem.h"

Kiax2ListWidgetItem::Kiax2ListWidgetItem(QListWidget * parent, int type, int id, std::string callState, std::string callDirection):
						QListWidgetItem(parent, 1001)
{
	Logger::log(Logger::DEBUG, "ListWidgetItem - call state %s call direction %s\n", callState.data(), callDirection.data());
	objectId = id;
	if (callState.compare(CALL_STATE_NONE)==0)
		setIcon(QIcon("icons/contact.png"));
	else
		if (callState.compare(CALL_STATE_ANSWERED)==0)
			if (callDirection.compare(CALL_DIRECTION_OUTGOING)==0)
				setIcon(QIcon("icons/outanswered.png"));
			else
				setIcon(QIcon("icons/inanswered.png"));
		else
			if (callDirection.compare(CALL_DIRECTION_OUTGOING)==0)
				setIcon(QIcon("icons/outunanswered.png"));
			else
				setIcon(QIcon("icons/inunanswered.png"));
		
	//setSizeHint(QSize(100,50));
}

Kiax2ListWidgetItem::~Kiax2ListWidgetItem()
{
}
