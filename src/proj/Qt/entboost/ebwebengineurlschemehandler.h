#ifndef EBWEBENGINEURLSCHEMEHANDLER_H
#define EBWEBENGINEURLSCHEMEHANDLER_H

#include <QWebEngineUrlSchemeHandler>
//#include <QSoundEffect>

static const char * theViewEcard  = "eb-view-ecard";            /// "eb-view-ecard:///[ECARD_DATA]"
static const char * thePlayVoice  = "eb-play-voice";            /// "eb-play-voice:///[FILE_NAME]"
static const char * theDownloadResource  = "eb-download-resource";
static const char * theReqAddContact  = "eb-add-contact";
static const char * theCallAccount  = "ebim-call-account";
static const char * theCallGroup  = "ebim-call-group";
static const char * theOpenSubId  = "eb-open-subid";
//static const char * theOpenUrl  = "eb-open-url";
static const char * theOpenFile  = "eb-open-file";
static const char * theEBWindow2Url  = "eb-open2";
static const char * theEBWindowUrl  = "eb-open";
static const char * theClose  = "eb-close";
static const char * theExitApp  = "eb-exit-app";
static const char * theLogout  = "eb-logout";
static const char * theShowMainFrame  = "eb-show-mainframe";
static const char * theHideMainFrame  = "eb-hide-mainframe";
static const char * theOpenFileManager  = "eb-open-filemanager";
static const char * theMailTo  = "mailto";

class EbWebEngineUrlSchemeHandler : public QWebEngineUrlSchemeHandler
{
    Q_OBJECT
public:
    static EbWebEngineUrlSchemeHandler* instance(void);
    EbWebEngineUrlSchemeHandler( QObject *parent=0 );

    void requestUrl( const QUrl & url );
protected:
    virtual void requestStarted(QWebEngineUrlRequestJob*);

private:
    static EbWebEngineUrlSchemeHandler * _instance;
//    QSoundEffect m_sounrEffect;
};

#endif // EBWEBENGINEURLSCHEMEHANDLER_H
