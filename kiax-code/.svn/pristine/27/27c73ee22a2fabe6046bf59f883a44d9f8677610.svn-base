#include <QDialog>
#include <QDialogButtonBox>
#include <QtNetwork>
#include <QProgressDialog>
#include <QFile>
#include <QHttp>
#include <QHttpResponseHeader>
#include <QProgressDialog>
#include <QMessageBox>
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
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);

signals:
	void downloadComplete(bool cancelled, uint downloadSpeed);
	
private:
	uint startTime, endTime, totalBytes;
	QProgressDialog *progressDialog;	
    QHttp *http;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;
	
};
