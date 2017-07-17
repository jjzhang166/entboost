#ifndef EBWEBENGINEDOWNLOADITEM_H
#define EBWEBENGINEDOWNLOADITEM_H

#include <boost/shared_ptr.hpp>
#include <QWebEngineDownloadItem>
#include <QUrl>

class EbWebEngineDownloadItem : public QObject
{
    Q_OBJECT
public:
    typedef boost::shared_ptr<EbWebEngineDownloadItem> pointer;
    EbWebEngineDownloadItem(QWebEngineDownloadItem * download, QObject * parent);
    static EbWebEngineDownloadItem::pointer create(QWebEngineDownloadItem * download,QObject * parent=0);

    quint32 id(void) const {return m_id;}
//    void setState(QWebEngineDownloadItem::DownloadState state) {m_state=state;}
    QWebEngineDownloadItem::DownloadState state(void) const {return m_state;}
//    void setTotalBytes(qint64 v) {m_totalBytes=v;}
    qint64 totalBytes() const {return m_totalBytes;}
//    void setReceivedBytes(qint64 v) {m_receivedBytes=v;}
    qint64 receivedBytes() const {return m_receivedBytes;}
    const QUrl &url() const {return m_url;}
    const QString &mimeType() const {return m_mimeType;}
//    void setPath(const QString &v) {m_path=v;}
    const QString &path() const {return m_path;}
    bool isFinished() const {return m_state==QWebEngineDownloadItem::DownloadCompleted?true:false;}

signals:
//    void downloadFinished(const EbWebEngineDownloadItem*);
////    void downLoadStateChanged(const EbWebEngineDownloadItem*);
//    void downloadProgress(const EbWebEngineDownloadItem*);

public slots:
    void onDownloadFinished();
    void onDownLoadStateChanged(QWebEngineDownloadItem::DownloadState state);
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    QWebEngineDownloadItem * m_downloadItem;
    quint32 m_id;
    QWebEngineDownloadItem::DownloadState m_state;
    qint64 m_totalBytes;
    qint64 m_receivedBytes;
    QUrl m_url;
    QString m_mimeType;
    QString m_path;
//    bool m_sendReceiving;
    time_t m_tCreateTime;
};

#endif // EBWEBENGINEDOWNLOADITEM_H
