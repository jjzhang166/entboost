#include "ebwebenginedownloaditem.h"


EbWebEngineDownloadItem::EbWebEngineDownloadItem(const QWebEngineDownloadItem *download, QObject * parent)
    : QObject(parent)
    , m_id(0)
    , m_state(QWebEngineDownloadItem::DownloadRequested)
    , m_totalBytes(0)
    , m_receivedBytes(0)
{
    m_id = download->id();
    m_state = download->state();
    m_totalBytes = download->totalBytes();
    m_receivedBytes = download->receivedBytes();
    m_url = download->url();
    m_mimeType = download->mimeType();
    m_path = download->path();
}
EbWebEngineDownloadItem::pointer EbWebEngineDownloadItem::create(const QWebEngineDownloadItem * download,QObject * parent)
{
    return EbWebEngineDownloadItem::pointer( new EbWebEngineDownloadItem(download,parent) );
}

void EbWebEngineDownloadItem::onDownloadFinished()
{
    if ( m_state!=QWebEngineDownloadItem::DownloadCompleted ) {
        m_state = QWebEngineDownloadItem::DownloadCompleted;
    }
    emit downloadFinished(this);
}

void EbWebEngineDownloadItem::onDownLoadStateChanged(QWebEngineDownloadItem::DownloadState state)
{
    m_state = state;
}

void EbWebEngineDownloadItem::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal>0 && m_totalBytes!=bytesTotal) {
        m_totalBytes = bytesTotal;
    }
    m_receivedBytes = bytesReceived;
    emit downloadProgress(this);
}
