#include "ebwebengineurlschemehandler.h"
#include <QWebEngineUrlRequestJob>
#include <QHostAddress>
#include <ebclientapp.h>
#include <QSound>

EbWebEngineUrlSchemeHandler * EbWebEngineUrlSchemeHandler::_instance = 0;

EbWebEngineUrlSchemeHandler *EbWebEngineUrlSchemeHandler::instance(void)
{
    static QMutex mutex;
    if ( _instance==0 ) {
        QMutexLocker locker(&mutex);
        if ( _instance==0 ) {
            _instance = new EbWebEngineUrlSchemeHandler;
        }
    }
    return _instance;
}

EbWebEngineUrlSchemeHandler::EbWebEngineUrlSchemeHandler(QObject *parent)
    : QWebEngineUrlSchemeHandler(parent)
{

}

void EbWebEngineUrlSchemeHandler::requestUrl(const QUrl &pUrl)
{
    const QString scheme = pUrl.scheme();
    /// 80->"0.0.0.80"
    /// 898542->"0.13.181.238"
    const QString host = pUrl.host();
    if ( scheme==theOpenFile ) {
        /// 打开文件
        const QString fileName = pUrl.url().mid(scheme.length()+4);
        QDesktopServices::openUrl( QUrl::fromLocalFile(fileName) );
    }
    else if ( scheme==thePlayVoice ) {
        /// 播放语音消息
        const QString fileName = pUrl.url().mid(scheme.length()+4);
        QSound::play( fileName );

//        if ( m_sounrEffect.isPlaying() ) {
//            m_sounrEffect.stop();
//        }
//        m_sounrEffect.setSource( QUrl::fromLocalFile(fileName) );
//        ///循环播放
////        effect.setLoopCount(QSoundEffect::Infinite);
//        ///设置音量，0-1
////        m_sounrEffect.setVolume(0.8f);
//        m_sounrEffect.play();
    }
    else if ( scheme==theCallAccount ) {
        const std::string callAccount( host.toStdString() );
        std::vector<tstring> list;
        if ( libEbc::ParseString( callAccount.c_str(), ".", list) == 4) {
            const unsigned int userId = QHostAddress(host).toIPv4Address();
            theApp->m_ebum.EB_CallUserId( (mycp::bigint)userId );
        }
        else {
            theApp->m_ebum.EB_CallAccount( callAccount.c_str() );
        }
    }
    else if ( scheme==theCallGroup ) {
        const std::string sGroupId( host.toStdString() );
        std::vector<tstring> list;
        if ( libEbc::ParseString( sGroupId.c_str(), ".", list) == 4) {
            const unsigned int nGroupId = QHostAddress(host).toIPv4Address();
            theApp->m_ebum.EB_CallGroup( (mycp::bigint)nGroupId );
        }
        else {
            theApp->m_ebum.EB_CallGroup( eb_atoi64(sGroupId.c_str()) );
        }
    }
    else {
        /// ??? 是否使用工作台打开？
        QDesktopServices::openUrl( pUrl );
//        const QString sUrl( pUrl.url() );
//        if (scheme=="http" || scheme=="https") {
//            if (!sUrl.isEmpty()) {
//                /// ??? 是否使用工作台打开？
//                QDesktopServices::openUrl( url );
//            }
//            return;
//        }
    }
}

void EbWebEngineUrlSchemeHandler::requestStarted( QWebEngineUrlRequestJob *requestJob )
{
    requestUrl(requestJob->requestUrl());
}
