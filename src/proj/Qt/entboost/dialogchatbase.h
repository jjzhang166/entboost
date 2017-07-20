#ifndef DIALOGCHATBASE_H
#define DIALOGCHATBASE_H

#include "ebclientapp.h"
#include <QSplitter>
#include <QKeyEvent>
#include <EbDialogBase.h>

namespace Ui {
class DialogChatBase;
}
//class DialogChatInput;
//class EbWidgetChatMessage;
class EbTextBrowser;
class EbWidgetChatInput;
class EbWidgetChatRight;
class EbDialogSelectUser;

class DialogChatBase : public EbDialogBase
{
    Q_OBJECT

public:
    explicit DialogChatBase(const EbcCallInfo::pointer& pCallInfo,QWidget *parent = 0);
    virtual ~DialogChatBase();
    typedef boost::shared_ptr<DialogChatBase> pointer;
    static DialogChatBase::pointer create(const EbcCallInfo::pointer& pCallInfo,QWidget *parent = 0) {
        return DialogChatBase::pointer(new DialogChatBase(pCallInfo,parent));
    }

    void updateLocaleInfo(void);
    void timerCheckState(void);
    /// 设置输入框 focus
    void setFocusInput(void);
    void scrollToEnd(void);
    void setCallInfo(const EbcCallInfo::pointer& pCallInfo);    /// 主要用于更新 CALLID
    EbcCallInfo::pointer callInfo(void) const {return m_callInfo;}
    mycp::bigint callId(void) const {return m_callInfo->m_pCallInfo.GetCallId();}
    mycp::bigint groupId(void) const {return m_callInfo->m_pCallInfo.m_sGroupCode;}
    mycp::bigint fromUserId(void) const {return m_callInfo->m_pFromAccountInfo.GetUserId();}
    void setOwnerCall(bool bOwnerCall) {m_bOwnerCall = bOwnerCall;}
    bool isOwnerCall(void) const {return m_bOwnerCall;}
    bool clearUnreadMsg(bool bFromUserClick);
    int getUnreadMsgCount(void) const;

    void onUserExitRoom(eb::bigint nUserId, bool bExitDep);
    void onAlertingCall(void);
    void onSendRich(const CCrRichInfo * pCrMsgInfo,EB_STATE_CODE nState);
    void onReceiveRich(const CCrRichInfo * pCrMsgInfo,QString* sOutFirstMsg1,QString* sOutFirstMsg2);
    void onUserLineStateChange(eb::bigint nGroupCode, eb::bigint nUserId, EB_USER_LINE_STATE bLineState);
    void onMemberInfo(const EB_MemberInfo* pMemberInfo, bool bChangeLineState);
    bool onMemberHeadChange(const EB_MemberInfo * pMemberInfo);
    bool onContactHeadChange(const EB_ContactInfo* pContactInfo);
    void onGroupInfo(const EB_GroupInfo* pGroupInfo);
    void onRemoveGroup(const EB_GroupInfo* pGroupInfo);
    void onRemoveMember(const EB_GroupInfo* pGroupInfo, mycp::bigint nMemberId, mycp::bigint memberUserId);
    void onMsgReceipt(const CCrRichInfo * pCrMsgInfo,int nAckType);
    void updateMsgReceiptData(eb::bigint nMsgId, eb::bigint nFromUserId, int nAckType, EB_STATE_CODE nState);

    void onSendingFile(const CCrFileInfo * fileInfo);
    bool onSentFile(const CCrFileInfo * fileInfo);
    void onCancelFile(const CCrFileInfo * fileInfo, bool bChangeP2PSending);
    void onReceivingFile(const CCrFileInfo * fileInfo,QString* sOutFirstMsg=0);
    void onReceivedFile(const CCrFileInfo * fileInfo);
    void onFilePercent(const CChatRoomFilePercent * pChatRoomFilePercent);
    void onSave2Cloud(const CCrFileInfo * fileInfo);

    const QString & fromName(void) const {return m_sFromName;}
    const QString & fullName(void) const {return m_sFullName;}
    const QString & shortName(void) const {return m_sShortName;}
    const QString & fromDescription(void) const {return m_sFromDescription;}
    const QString & imageMd5(void) const {return m_imageMd5;}
    const QImage & fromImage(void) const {return m_fromImage;}
    EB_USER_LINE_STATE fromLineState(void) const {return m_nFromLineState;}
//    bool fromIsOffLineState(void) const {return m_nFromLineState==EB_LINE_STATE_OFFLINE || m_nFromLineState==EB_LINE_STATE_UNKNOWN;}
    bool isOnlineState(void) const {return m_nFromLineState>=EB_LINE_STATE_BUSY?true:false;}
    bool isGroupChat(void) const {return (m_callInfo->m_pCallInfo.m_sGroupCode>0)?true:false;}
    EB_GROUP_TYPE groupType(void) const {return m_nGroupType;}

public slots:
    void onClickedInputClose(void);
    void onClickedInputMsgRecord(void);
    void onClickedButtonAddUser(void);
    void onClickedButtonSendFile(void);
    void onTriggeredActionSendECard(void);
    void onTriggeredActionChatApps(void);
    void onClickedButtonChatApps(void);
    void exitChat(bool hangup);
    void onClickedButtonExitChat(void);

signals:
    void clickedClose(void);

protected:
    virtual bool onBeforeClickedPushButtonSysMin(void);
    virtual bool onBeforeClickedPushButtonSysMax(void);
    bool requestClose(bool checkOnly);
    virtual void reject(void);

    virtual void resizeEvent(QResizeEvent *);
    virtual void keyPressEvent(QKeyEvent *);

private:
    void updateSize(void);
    void processMsg(bool bReceive,const CCrRichInfo* pCrMsgInfo,EB_STATE_CODE nState,QString* sOutFirstMsg1=0,QString* sOutFirstMsg2=0);
    bool processFile(bool bReceive,const CCrFileInfo * fileInfo);

    void updateFromInfo(void);

private:
    Ui::DialogChatBase *ui;
    QSplitter * m_splitterMain;     /// 先把主界面，分左右二边
    QSplitter * m_splitterInput;    /// 再把左边分上下
    EbDialogSelectUser * m_dialogSelectUser;
    EbTextBrowser* m_textBrowserMessage;
    EbWidgetChatInput* m_widgetChatInput;
    EbWidgetChatRight* m_widgetChatRight;
    EbcCallInfo::pointer m_callInfo;
//    EB_AccountInfo m_pFromAccountInfo;
    bool m_bOwnerCall;
    QString m_sFromName;
    QString m_sFullName;
    QString m_sShortName;
    QString m_sFromDescription;
    QString m_imageMd5;
    QImage m_fromImage;
    EB_USER_LINE_STATE m_nFromLineState;
    EB_GROUP_TYPE m_nGroupType;
//    CLockMap<mycp::bigint,bool> m_pPrevReceivedFileMsgIdList;   /// msgid->
    QMenu * m_menuChatApps;
};
const DialogChatBase::pointer DialogChatBaseNull;

#endif // DIALOGCHATBASE_H
