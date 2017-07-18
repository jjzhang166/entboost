#ifndef EBCLIENTAPP_H
#define EBCLIENTAPP_H

#include <QtGui>
#include <QColor>
#include <QHostAddress>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QListWidget>
#include <QListWidgetItem>
#include <CGCBase/cgcobject.h>
#include <CGCBase/cgcSmartString.h>
using namespace mycp;
#include "../../../include/ebc_public.h"
#include "../../../include/colorconver.h"
#include "../../../include/EBParseSetting.h"
#include "../../../libpopusermanager/EBAppClient.h"
#include "../../../include/SqliteExt.h"
#include "../include/boost_ini.h"
using namespace entboost;
#include "ebclocales.h"
#include "httpfiledownload.h"
#include "ebccallinfo.h"
#include <boost/shared_ptr.hpp>
//#include <QtWebEngine/QtWebEngine>

/// 使用这个某些事件参数，才能返回处理结果
//#define USES_CORE_EVENT

const int const_button_icon_size = 9;

const int const_common_edit_height = 22;
const int const_common_title_font_size = 11;
const int const_common_title_image_font_size = 12;
const int default_warning_auto_close = 3;       /// 警告提示，显示3秒后，自动关闭
const int default_information_auto_close = 5;   /// 消息提示，显示5秒后，自动关闭
const QSize const_tree_icon_size(32,32);

#define UISTYLE_CHAT_DEFAULT_DLG_WIDTH	288
#define UISTYLE_CHAT_DEFAULT_DLG_HEIGHT	568

#ifndef is_visitor_uid
const mycp::bigint theVisitorStartId = 0x2000000000000LL;	// =562949953421312(15位)
#define is_visitor_uid(id) (id>=theVisitorStartId)
#endif // is_visitor_uid

typedef enum EB_SEND_KEY_TYPE {
    EB_SEND_KEY_ENTER,
    EB_SEND_KEY_CTRL_ENTER
}EB_SEND_KEY_TYPE;

typedef enum EB_MSG_RECORD_TYPE
{
    MRT_UNKNOWN
    , MRT_TEXT
    , MRT_JPG
    , MRT_FILE
    , MRT_WAV
    , MRT_RESOURCE
    , MRT_MAP_POS		= 10	/// 地图位置
    , MRT_CARD_INFO				/// 名片信息，如用户名片等
    , MRT_USER_DATA	= 200
}EB_MSG_RECORD_TYPE;
inline bool isCanCollectRecordType(EB_MSG_RECORD_TYPE nType) {return (nType==MRT_FILE || nType==MRT_RESOURCE)?false:true;}

class DialogEmotionSelect;
class DialogMainFrame;

class EbClientApp : public QObject
{
//    Q_OBJECT
public:
    explicit EbClientApp(QObject *parent=0);
    ~EbClientApp(void);
    typedef boost::shared_ptr<EbClientApp> pointer;
    static EbClientApp::pointer create(QObject *parent=0) {
        return EbClientApp::pointer( new EbClientApp(parent) );
    }

    CEBParseSetting m_setting;
    CEBAppClient m_ebum;
    CSqliteCdbc::pointer m_sqliteEbc;
    CSqliteCdbc::pointer m_sqliteUser;
    /// 记录当前所有会话
    CLockMap<mycp::bigint,EbcCallInfo::pointer> m_pCallList;
    /// 记录自动 CALL group id
    CLockMap<eb::bigint,bool> m_pAutoCallGroupIdList;
    /// 记录自动 CALL from user id
    CLockMap<eb::bigint,bool> m_pAutoCallFromUserIdList;
    CLockMap<eb::bigint,bool> m_pCancelFileList;
    eb::bigint m_nSelectCallId;
    void clearSubscribeSelectInfo(void);
    void triggeredApps(int index);
    std::vector<EB_SubscribeFuncInfo> m_pSubscribeFuncList;

    void setMainWnd(DialogMainFrame * mainWnd) {m_mainWnd=mainWnd;}
    DialogMainFrame *mainWnd(void) const {return m_mainWnd;}
    bool setDevAppId(QObject* receiver);

    bool initApp(void);
    void exitApp(bool bResetEbumOnly=false);
//    bool startEBUMClient(void);
    bool onLogonSuccess(void);

    const QString & getAppDataPath(void) const {return m_appDataPath;}
    const QString & getAppUsersPath(void) const {return m_appUsersPath;}
    const QString & getAppImgPath(void) const {return m_appImgPath;}
    const QString & getAppLocalesPath(void) const {return m_appLocalesPath;}
    const QString & getSettingFile(void) const {return m_settingFile;}
    const QString & getSettingIntFile(void) const {return m_settingIniFile;}
    const QString & getEbcIniFile(void) const {return m_ebcIniFile;}
    const QString & userMainPath(void) const {return m_userMainPath;}
    const QString & userImagePath(void) const {return m_userImagePath;}
    const QString & userFilePath(void) const {return m_userFilePath;}
    const QString & userSettingIniFile(void) const {return m_userSettingIniFile;}

//    void setMainColor(QRgb v,bool bUpdateDatabase);
    void setMainColor(unsigned char r, unsigned char g, unsigned char b, bool bUpdateDatabase);
    void setMainColor(const QColor& pColor, bool bUpdateDatabase);
    void updateStyleSheet(void);    /// 实现某些组件，隐藏变为显示，自动刷新UI界面
    const QColor& getMainColor(void) const {return this->m_mainColor;}
//    const QColor& getHotColor(void) const {return this->m_hotColor;}
//    const QColor& getPreColor(void) const {return this->m_preColor;}

    QString subscribeFuncUrl( mycp::bigint subId, const std::string &sParameters="" );
    void setEntManagerUrl(const QString& v) {m_sEntManagerUrl = v;}
    const QString& entManagerUrl(void) const {return m_sEntManagerUrl;}
    void setEnterpriseCreateUserId(eb::bigint nUserId) {m_nEnterpriseCreateUserId = nUserId;}
    eb::bigint enterpriseCreateUserId(void) const {return m_nEnterpriseCreateUserId;}
    bool isEnterpriseCreateUserId(eb::bigint nUserId) {return (nUserId>0 && nUserId==m_nEnterpriseCreateUserId)?true:false;}

    QString lineStateText(EB_USER_LINE_STATE lineState) const;
    QString lineStateText(void) const;
    bool isAppIdResponseOk(void) const {return m_isAppIdResponseOk;}
    bool isLicenseUser(void) const {return m_bLicenseUser;}
    const QString & productName(void) const {return m_sProductName;}
    bool sendRegMail(void) const {return m_bSendRegMail;}
    int saveConversations(void) const {return m_nSaveConversations;}
    bool isSaveConversationLocal(void) const {return (m_nSaveConversations&1)==1;}
    bool isSaveConversationServer(void) const {return (m_nSaveConversations&2)==2;}
    bool isAuthContact(void) const {return m_bAuthContact;}
    mycp::bigint deployId(void) const {return m_nDeployId;}
    int licenseType(void) const {return m_nLicenstType;}
    int ebServerVersion(void) const {return m_nEBServerVersion;}
    eb::bigint groupMsgSugId(void);
    eb::bigint findAppSugId(void);
    mycp::bigint autoOpenSubId(void) const {return m_nAutoOpenSubId;}
    mycp::bigint myCollectionSugId(void);
    bool isAutoHideMainFrame(void) const {return m_bAutoHideMainFrame;}
    bool isHideMainFrame(void) const {return m_bHideMainFrame;}
    bool isDisableuserCloudDrive(void) const {return m_bDisableUserCloudDrive;}
    bool isDisableGroupShareCloud(void) const {return m_bDisableGroupShareCloud;}
    bool isDisableModifyPasswd(void) const {return m_bDisableModifyPasswd;}
    bool isDisableVideo(void) const {return m_bDisableVideo;}
    bool isDisableRemoteDesktop(void) const {return m_bDisableRemoteDesktop;}
    bool isDisableAccountEdit(void) const {return m_bDisableAccountEdit;}

    const QString & defaultUrl(void) const {return m_sDefaultUrl;}
    int openRegister(void) const {return m_nOpenRegister;}
    bool isOpenVisitor(void) const {return m_bOpenVisitor;}
    const QString& forgetPwdUrl(void) const {return m_sForgetPwdUrl;}

    void setDefaultBrowserType(EB_BROWSER_TYPE newValue) {m_nDefaultBrowserType = newValue;}
    EB_BROWSER_TYPE defaultBrowserType(void) const {return m_nDefaultBrowserType;}
    bool setDefaultUIStyleType(EB_UI_STYLE_TYPE newValue);
    EB_UI_STYLE_TYPE defaultUIStyleType(void) const {return m_nDefaultUIStyleType;}
    void setSendKeyType(EB_SEND_KEY_TYPE v);
    EB_SEND_KEY_TYPE sendKeyType(void) const {return m_sendKeyType;}

    const QRect& deskRect(void) const {return m_deskRect;}
    const QRect& screenRect(void) const {return m_screenRect;}

    DialogEmotionSelect* showDialogEmotionSelect(const QPoint& pt,QObject* receiver=0);

    bool isLogoned(void) const {return m_ebum.EB_IsLogoned();}
    bool isLogonVisitor(void) const {return m_ebum.EB_IsLogonVisitor();}
    mycp::bigint logonUserId(void) const {return m_ebum.EB_GetLogonUserId();}
    tstring logonUserAccount(void) const {return m_ebum.EB_GetLogonAccount();}
    void editContactInfo(mycp::bigint contactId,QWidget* parent=0);
    void editGroupInfo(mycp::bigint groupId,QWidget* parent=0);
    void newGroupInfo(EB_GROUP_TYPE groupType,QWidget* parent=0);
    void editMemberInfo(const EB_MemberInfo* pMemberInfo,QWidget* parent=0);
    void deleteDbRecord(eb::bigint sId, bool bIsAccount);
    void deleteDbRecord(eb::bigint sMsgId);
    void updateMsgReceiptData(eb::bigint nMsgId, eb::bigint nFromUserId, int nAckType);
    tstring userHeadFilePath(mycp::bigint nUserId,const tstring& sAccount) const;
    tstring memberHeadFilePath(const EB_MemberInfo * memberInfo) const;

protected:
//    virtual bool event(QEvent *e);
    virtual void customEvent(QEvent *e);

public slots:

private:
    void onAppIdSuccess(QEvent *e);
    void onAppIdError(QEvent *e);
    void loadUserSetting(void);

    HttpFileDownload m_httpFileDownload;
    void updateColor(bool bUpdateDatabase);

private:
    QString m_appDataPath;      // /datas
    QString m_appUsersPath;
    QString m_appImgPath;
    QString m_appLocalesPath;
    QString m_appBodbPath;
    QString m_settingFile;      // setting
    QString m_settingIniFile;   // setting.ini
    QString m_ebcIniFile;       // ebc.ini
    QString m_userMainPath;
    QString m_userImagePath;
    QString m_userFilePath;
    QString m_userSettingIniFile;
    QColor m_mainColor;
    QString m_styleSheet;
//    QColor m_hotColor;      // 按钮 hot 状态
//    QColor m_preColor;      // 按钮 pre 状态

    //
    QString m_sEntManagerUrl;
    eb::bigint m_nEnterpriseCreateUserId;

    bool m_isAppIdResponseOk;
    std::string m_sAccountPrefix;
    bool m_bLicenseUser;
    QString m_sProductName;
    bool m_bSendRegMail;
    int m_nSaveConversations;
    bool m_bAuthContact;
    mycp::bigint m_nDeployId;
    int m_nLicenstType;         // 许可类型；0=未授权；1=终身授权；2=时间授权
    int m_nEBServerVersion;
    eb::bigint m_nGroupMsgSubId;
    eb::bigint m_nFindAppSubId;
    mycp::bigint m_nAutoOpenSubId;
    eb::bigint m_myCollectionSubId;

    bool m_bAutoHideMainFrame;
    bool m_bHideMainFrame;
    bool m_bDisableUserCloudDrive;
    bool m_bDisableGroupShareCloud;
    bool m_bDisableModifyPasswd;
    bool m_bDisableVideo;
    bool m_bDisableRemoteDesktop;
    bool m_bDisableAccountEdit;
    EB_BROWSER_TYPE m_nDefaultBrowserType;
    bool m_bDisableMsgReceipt;
    bool m_bStatSubGroupMember;
    QString m_sDefaultUrl;
    int m_nOpenRegister;
    bool m_bOpenVisitor;
    QString m_sForgetPwdUrl;

    EB_UI_STYLE_TYPE m_nDefaultUIStyleType;
    EB_SEND_KEY_TYPE m_sendKeyType;

    QRect m_deskRect;
    QRect m_screenRect;

    DialogMainFrame * m_mainWnd;
    QObject* m_receiver;
    DialogEmotionSelect* m_dialogEmotionSelect;
};

extern EbClientApp::pointer theApp;
extern EbcLocales theLocales;

/// 暂时没有用
//class EbWebEngineUrlRequestInterceptor : public QWebEngineUrlRequestInterceptor
//{
//    Q_OBJECT
//public:
//    explicit EbWebEngineUrlRequestInterceptor(QObject *p = Q_NULLPTR)
//        : QWebEngineUrlRequestInterceptor (p)
//    {
//    }

//    virtual void interceptRequest(QWebEngineUrlRequestInfo &info) {
//        int i=0;
//    }
//};

#endif // EBCLIENTAPP_H
