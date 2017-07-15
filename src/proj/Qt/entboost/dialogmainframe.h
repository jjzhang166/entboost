#ifndef DIALOGMAINFRAME_H
#define DIALOGMAINFRAME_H

#include <EbDialogBase.h>
#include <QMenu>
#include <QSystemTrayIcon>
#include "ebwidgetmygroup.h"
//#include "dialogmygroup.h"
#include "dialogchatbase.h"
#include <eblabel.h>

namespace Ui {
class DialogMainFrame;
}
class DialogFrameList;
class DialogMessageTip;
class DialogMyCenter;
class EbWidgetAppBar;

//#define USES_EVENT_DATE_TIMER
class DialogMainFrame : public EbDialogBase
{
    Q_OBJECT
public:
    explicit DialogMainFrame(QWidget *parent = 0);
    ~DialogMainFrame();

    void updateLocaleInfo(void);
    void refreshSkin(void);

public slots:
    void onClickedPushButtonSetting(void);
    void onTriggeredActionSelectColor(void);
    void updateLineState(void);
    void onClickedLabelUserImage(void);
    void onClickedLineState(void);
    void onClickedMenuLineState(void);
    void onTriggeredActionOpenWorkFrame(void);
    void onTriggeredActionMyCollection(void);
    void onTriggeredActionLogout(void);
    void onTriggeredActionExitApp(void);
    void onClickedPushButtonMyGroup(void);
    void onClickedPushButtonMyContact(void);
    void onClickedPushButtonMySession(void);
    void onClickedPushButtonMyEnterprise(void);
    void onClickedPushButtonMyApp(void);
    void updateMyButton(const QPushButton* fromButton);

    void onClickedSubApp(const EB_SubscribeFuncInfo &);
    bool openSubscribeFuncWindow( const EB_SubscribeFuncInfo& pSubscribeFuncInfo, const std::string& sPostData="", const std::string& sParameters="" );

    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

//    void onClickedPushButtonFileManager(void);

    void processDatas(void);
protected:
    virtual void accept();
    virtual void reject(void);
    virtual void resizeEvent(QResizeEvent *);
//    virtual void closeEvent(QCloseEvent *event);

//    virtual bool event(QEvent *e);
    virtual void customEvent(QEvent *e);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *);
    virtual void contextMenuEvent(QContextMenuEvent *);
    bool eventFilter(QObject *obj, QEvent *ev);
    void timerEvent( QTimerEvent *event );

    void createMenuData(void);
    QSystemTrayIcon * m_trayIcon;
    void creatTrayIcon(void);
    void changeTrayTooltip(void);

private:
    void BuildHeadRect(void);

    void addSubUnreadMsg(mycp::bigint subId, bool sendToWorkFrame);
    void setSubUnreadMsg(mycp::bigint subId, size_t unreadMsgCount, bool sendToWorkFrame);
    void onBroadcastMsg(QEvent *e);
    void onAreaInfo(QEvent *e);

    void onUserStateChange(QEvent *e);
    void onMemberHeadChange(QEvent *e);
    void onContactHeadChange(QEvent *e);
    /// 聊天消息
    void onMsgReceipt(QEvent *e);
    void onSendRich(QEvent *e);
    bool onReceiveRich(QEvent *e);
    void onSendingFile(QEvent *e);
    void onSentFile(QEvent *e);
    void onCancelFile(QEvent *e);
    bool onReceivingFile(QEvent *e);
    void onReceivedFile(QEvent *e);
    void onFilePercent(QEvent *e);
    /// 聊天会话
    void CreateFrameList(bool bShow);
    DialogChatBase::pointer getDialogChatBase(const EbcCallInfo::pointer & pEbCallInfo,bool bShow=true,bool bOwnerCall=false);
    DialogChatBase::pointer getDialogChatBase(eb::bigint nCallId, bool bRemove=false);
    void onCallConnected(QEvent *e);
    void onCallError(QEvent *e);
    void onCallHangup(QEvent *e);
    void onCallAlerting(QEvent *e);
    void onCallIncoming(QEvent *e);
    /// 联系人
    void onContactDelete(QEvent *e);
    void onContactInfo(QEvent *e);
    void onAcceptAddContact(QEvent *e);
    void onRejectAddContact(QEvent *e);
    void onRequestAddContact(QEvent *e);
    /// 组织结构
    void onEditInfoResponse(QEvent *e);
    void onMemberEditResponse(QEvent *e);
    void onMemberDelete(QEvent *e);
    void onMemberInfo(QEvent *e);
    void onRejectAdd2Group(QEvent *e);
    void onInviteAdd2Group(QEvent *e);
    void onRequestAdd2Group(QEvent *e);
    void onExitGroup(QEvent *e);
    void onRemoveGroup(QEvent *e);
    void onGroupEditResponse(QEvent *e);
    void onGroupDelete(QEvent *e);
    void onGroupInfo(QEvent *e);
    void onEnterpriseInfo(QEvent *e);
    /// 登录
    void onLogonSuccess(QEvent *e);
    void onLogonTimeout(QEvent *e);
    void onLogonError(QEvent *e);
    void onOnlineAnother(QEvent *e);

#ifdef USES_EVENT_DATE_TIMER
    void checkEventData(void);
#else
    bool checkEventData(QEvent * e);
#endif

    void checkOneSecond(void);
    void checkCallExit(void);
private:
    Ui::DialogMainFrame *ui;
    EbLabel* m_labelUserImage;
    EbLabel* m_labelLinState;
    QMenu* m_menuSetting;
    QMenu* m_menuLineState;
    QMenu* m_menuContext;
    QAction * m_actionMyCollection;
    QRect m_rectHead;
    DialogMyCenter * m_pDlgMyCenter;
    DialogFrameList * m_pDlgFrameList;
    DialogMessageTip * m_pDlgMsgTip;
    EbWidgetMyGroup*  m_widgetMyGroup;
    EbWidgetAppBar * m_widgetMainAppBar;
#ifdef USES_EVENT_DATE_TIMER
    int m_checkEventData;
    CLockListPtr<QEvent*> m_eventList;
    CLockMap<QEvent*,bool> m_eventMap;
#endif
    int m_timerOneSecond;
};

#endif // DIALOGMAINFRAME_H