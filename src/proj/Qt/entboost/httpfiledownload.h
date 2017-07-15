#ifndef HTTPFILEDOWNLOAD_H
#define HTTPFILEDOWNLOAD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

class HttpFileDownload : public QObject
{
    Q_OBJECT
public:
    HttpFileDownload(QObject *parent = 0);
    virtual ~HttpFileDownload(void);

signals:
    void downloadHttpFileOk(const QString& saveToFilePath, const QString& lastModified);
    void downloadHttpFileError(const QString& saveToFilePath, QNetworkReply::NetworkError errorCode);

public slots:
    void downloadHttpFile( const QString& url, const QString& saveToFilePath, const QString& oldLastModifited, int nWaitSeconds=10 );
    void doDownloadHttpFile(void);
    const QString& getSaveToFilePath(void) const {return m_saveToFilePath;}
    bool getDownloadFinished(void) const {return m_downloadFinished;}
    QNetworkReply::NetworkError getLastErrorCode(void) const {return m_lastErrorCode;}
    const QString& getLastModified(void) const {return m_lastModified;}

    void replyReadyRead(void);
    void replyFinished(void);
    void replyError(QNetworkReply::NetworkError code);

private:
    QNetworkAccessManager * m_networkManager;
    QNetworkReply * m_reply;
    QFile * m_file;

    QString m_downloadUrl;
    QString m_oldLastModifited;
    QString m_saveToFilePath;
    bool m_downloadFinished;
    QNetworkReply::NetworkError m_lastErrorCode;
    QString m_lastModified;
};

#endif // HTTPFILEDOWNLOAD_H
