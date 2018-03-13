#include <QListWidgetItem>
#include <QString>
#include "PConstants.h"
#include <string>
#include "Logger.h"

class Kiax2ListWidgetItem : public QListWidgetItem
{

public:

 Kiax2ListWidgetItem(QListWidget * parent = 0, int type = Type, int id = 0, std::string callState = CALL_STATE_NONE, std::string callDirection = CALL_DIRECTION_NONE);
 ~Kiax2ListWidgetItem();
 
 int objectId;
 QString row1Text;
 QString row2Text;
 
 private:

};
