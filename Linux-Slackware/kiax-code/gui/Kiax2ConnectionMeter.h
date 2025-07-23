#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtNetwork>
#include <QtWidgets/QProgressDialog>
#include <QFile>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>

#include <QtWidgets/QMessageBox>

#ifdef WIN32DEP
#include <cstdlib.>
#endif

#include <string>
#include "Logger.h"
#include <dirent.h>
#include <unistd.h>
#include <QDateTime>
#include <QThread>
#include "PConfiguration.h"

class Kiax2ConnectionMeter : public QWidget
{

  Q_OBJECT

public:
  
	Kiax2ConnectionMeter(QWidget* parent);
	~Kiax2ConnectionMeter();
    void downloadFile();
	uint getDownloadSpeed();
	bool isAborted();
	
private slots:
    void cancelDownload();
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QNetworkReply &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);

signals:
	void downloadComplete(bool cancelled, uint downloadSpeed);
	
private:
	uint startTime, endTime, totalBytes;
	QProgressDialog *progressDialog;	
    QNetworkAccessManager *qnam;
    QNetworkRequest *qrequest;
    QNetworkReply *qreply;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;
	
};
