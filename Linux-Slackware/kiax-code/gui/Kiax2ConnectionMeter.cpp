
#include "Kiax2ConnectionMeter.h"
#ifdef WIN32DEP
#include <windows.h>
#endif
#ifdef LINUXDEP
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include "StorageService.h"

Kiax2ConnectionMeter::Kiax2ConnectionMeter(QWidget* parent) : QWidget(parent)
{
     progressDialog = new QProgressDialog(this);
     http = new QHttp(this);
     connect(http, SIGNAL(requestFinished(int, bool)),
             this, SLOT(httpRequestFinished(int, bool)));
     connect(http, SIGNAL(dataReadProgress(int, int)),
             this, SLOT(updateDataReadProgress(int, int)));
     connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
             this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
	 connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelDownload()));		
		startTime = 0;
		endTime = 0;
		totalBytes = 0;
}

Kiax2ConnectionMeter::~Kiax2ConnectionMeter()
{
	delete progressDialog;
	delete http;
}

void Kiax2ConnectionMeter::downloadFile()
{
	 totalBytes = 0;
	 startTime = 0;
	 endTime = 0;
     QUrl url(HTTP_DOWNLOAD_SPEEDTEST_URL);
	 std::string profileDir = StorageService::touchProfileDir() + "speedtestresult.txt";
     QString fileName (profileDir.data());
     if (QFile::exists(fileName)) {
         QFile::remove(fileName);
     }

     file = new QFile(fileName);
     if (!file->open(QIODevice::WriteOnly)) {
         QMessageBox::information(this, tr("HTTP"),
                                  tr("Unable to save the file."));
         delete file;
         file = 0;
         return;
     }

     QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
     http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());

     if (!url.userName().isEmpty())
         http->setUser(url.userName(), url.password());

     httpRequestAborted = false;
	 startTime = QDateTime::currentDateTime().toTime_t();
     httpGetId = http->get(url.path(), file);

     progressDialog->setWindowTitle(tr("Test"));
     progressDialog->setLabelText(tr("Testing connection speed.."));
	 progressDialog->show();
}

void Kiax2ConnectionMeter::cancelDownload()
{	
    httpRequestAborted = true;
	endTime = 0;
	totalBytes = 0;
	progressDialog->hide();
    http->abort();
}

void Kiax2ConnectionMeter::httpRequestFinished(int requestId, bool error)
{
     if (requestId != httpGetId)
         return;
     if (httpRequestAborted) {
         if (file) {
             file->close();
             file->remove();
             delete file;
             file = 0;
         }

         progressDialog->hide();
         return;
     }

     if (requestId != httpGetId)
         return;

	 endTime = QDateTime::currentDateTime().toTime_t();
	 Logger::log(Logger::DEBUG, "endTime = %d\n", endTime);
     progressDialog->hide();
     file->close();
	 uint speed = getDownloadSpeed();
	 emit downloadComplete(httpRequestAborted, speed);
     if (error) {
         file->remove();
         QMessageBox::information(this, tr("Test"),
                                  tr("Test failed."));
	 }
     delete file;
     file = 0;
}

void Kiax2ConnectionMeter::readResponseHeader(const QHttpResponseHeader &responseHeader)
{
    if (responseHeader.statusCode() != 200) {
         QMessageBox::information(this, tr("Test"),
                                  tr("Speed test failed."));
         httpRequestAborted = true;
         progressDialog->hide();
         http->abort();
         return;
     }
}

void Kiax2ConnectionMeter::updateDataReadProgress(int bytesRead, int totalBytes)
{
     if (httpRequestAborted)
         return;

     progressDialog->setMaximum(totalBytes);
     progressDialog->setValue(bytesRead);
	 this->totalBytes = bytesRead;
	 endTime = QDateTime::currentDateTime().toTime_t();
}

uint Kiax2ConnectionMeter::getDownloadSpeed()
{
	//Logger::log(Logger::DEBUG, "startTime = %d, endTime = %d, totalBytes = %d\n", startTime, endTime, totalBytes);
	if ((endTime!=0)&&(endTime>startTime))
	{
		uint elapsedTime = (endTime-startTime); // seconds
		if (totalBytes>0)
			return (uint) (totalBytes / elapsedTime) / 1024; // in kilobytes
	}
	return 0;
}

bool Kiax2ConnectionMeter::isAborted()
{
	return httpRequestAborted;
}
