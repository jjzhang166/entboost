#ifndef EBWIDGETCHATRIGHT_H
#define EBWIDGETCHATRIGHT_H

#include "ebclientapp.h"
//#include <QWidget>
#include <dialogworkframe.h>

class EbWidgetChatRight : public DialogWorkFrame
{
    Q_OBJECT
public:
    explicit EbWidgetChatRight(const EbcCallInfo::pointer& pCallInfo,QWidget *parent = 0);
    virtual ~EbWidgetChatRight(void);

    void exitChat(bool bHangup);
    void onUserExit(eb::bigint nUserId, bool bExitDep);
    void setCallInfo(const EbcCallInfo::pointer &pCallInfo);
    void onMsgReceipt(const CCrRichInfo* pCrMsgInfo,int nAckType);
    void onSendingFile(const CCrFileInfo * fileInfo);
    void onReceivingFile(const CCrFileInfo * fileInfo);
    void onReceivedFile(const CCrFileInfo * fileInfo);
    void onFilePercent(const CChatRoomFilePercent * pChatRoomFilePercent);
    void deleteTranFile(eb::bigint msgId);
    void onMemberInfo(const EB_MemberInfo* pMemberInfo, bool bChangeLineState);
    void getProcessing(bool& pVideoProcessing, bool& pFileProcessing, bool& pDesktopProcessing) const;
    void showMsgRecord(void);
    void triggeredApps(int index);

signals:

public slots:
    virtual void resizeEvent(QResizeEvent *);

private:
    EbWidgetFileTranList * openTranFile(void);

//    void updateGroupUsers(void);

private:
    EbcCallInfo::pointer m_callInfo;
//    DialogWorkFrame * m_workFrame;
};

#endif // EBWIDGETCHATRIGHT_H
