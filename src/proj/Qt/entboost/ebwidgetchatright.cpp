#include "ebwidgetchatright.h"

const int const_top_button_width = 85;
EbWidgetChatRight::EbWidgetChatRight(const EbcCallInfo::pointer& pCallInfo,QWidget *parent)
    : DialogWorkFrame(false,false,parent)
    , m_callInfo(pCallInfo)
//    , m_workFrame(0)
{
    assert(m_callInfo.get()!=0);

//    m_workFrame = new DialogWorkFrame(this);
//    m_workFrame->setModal(false);

//    this->addUrl(false,"www.entboost.com");

    if (m_callInfo->groupId()==0) {
        ///
        EbWorkItem::pointer workItem = EbWorkItem::create(EbWorkItem::WORK_ITEM_USER_INFO);
        workItem->setCallInfo( m_callInfo );
        workItem->setTopButtonWidth( const_top_button_width );
        DialogWorkFrame::addWorkItem(false,0,0,workItem,0);
    }
    else {
        ///
        EbWorkItem::pointer workItem = EbWorkItem::create(EbWorkItem::WORK_ITEM_USER_LIST);
        workItem->setCallInfo( m_callInfo );
        workItem->setTopButtonWidth( const_top_button_width );
        DialogWorkFrame::addWorkItem(false,0,0,workItem,0);
    }
}

void EbWidgetChatRight::onUserExit(eb::bigint nUserId, bool bExitDep)
{
    EbWidgetUserList * widgetUserList = DialogWorkFrame::widgetUserList();
    assert (widgetUserList!=0);
    widgetUserList->onExitUser( nUserId, bExitDep );
}

void EbWidgetChatRight::setCallInfo(const EbcCallInfo::pointer &pCallInfo)
{
    m_callInfo = pCallInfo;
    assert(m_callInfo.get()!=0);
}

void EbWidgetChatRight::onMsgReceipt(const CCrRichInfo *pCrMsgInfo, int nAckType)
{
//    if (m_pMsgRecord!=NULL)
//		m_pMsgRecord->OnMsgReceipt(pCrMsgInfo, nAckType);

}

void EbWidgetChatRight::onSendingFile(const CCrFileInfo *fileInfo)
{
    EbWidgetFileTranList * tranFile = this->openTranFile();
    tranFile->onSendingFile(fileInfo);
    if (tranFile->isEmpty()) {
        DialogWorkFrame::closeItem( 0, indexOf(EbWorkItem::WORK_ITEM_TRAN_FILE) );
    }
}

void EbWidgetChatRight::onReceivingFile(const CCrFileInfo *fileInfo)
{
    EbWidgetFileTranList * tranFile = this->openTranFile();
    tranFile->onReceivingFile(fileInfo);
}

void EbWidgetChatRight::onReceivedFile(const CCrFileInfo *fileInfo)
{
    deleteTranFile(fileInfo->m_nMsgId);
}

void EbWidgetChatRight::onFilePercent(const CChatRoomFilePercent *pChatRoomFilePercent)
{
    EbWidgetFileTranList * tranFile = widgetTranFile();
    if ( tranFile!=0 ) {
        tranFile->onFilePercent(pChatRoomFilePercent);
    }
}

void EbWidgetChatRight::deleteTranFile(eb::bigint msgId)
{
    EbWidgetFileTranList * tranFile = widgetTranFile();
    if (tranFile!=0) {
        tranFile->deleteTranFile(msgId);
        if (tranFile->isEmpty()) {
            DialogWorkFrame::closeItem( 0, indexOf(EbWorkItem::WORK_ITEM_TRAN_FILE) );
        }
    }
}

void EbWidgetChatRight::resizeEvent(QResizeEvent *e)
{

    DialogWorkFrame::resizeEvent(e);
}

EbWidgetFileTranList * EbWidgetChatRight::openTranFile()
{
    EbWidgetFileTranList * tranFile = widgetTranFile();
    if (tranFile==0) {
        EbWorkItem::pointer workItem = EbWorkItem::create(EbWorkItem::WORK_ITEM_TRAN_FILE);
        workItem->setCallInfo( m_callInfo );
        workItem->setTopButtonWidth( const_top_button_width );
        DialogWorkFrame::addWorkItem(false,0,0,workItem);
        tranFile = workItem->widgetTranFile();
    }
    return tranFile;
}

//void EbWidgetChatRight::updateGroupUsers()
//{
//    if ( m_callInfo->isGroupCall() ) {
//        int memberSize = 0;
//        int onlineSize = 0;
//        theApp->m_ebum.EB_GetGroupMemberSize(m_callInfo->groupId(),1,memberSize,onlineSize);
//        //const int nMemberSize = theEBAppClient.EB_GetGroupMemberSize(m_pCallInfo.m_sGroupCode,1);
//        //const int nOnlineSize = theEBAppClient.EB_GetGroupOnlineSize(m_pCallInfo.m_sGroupCode,1);
//        /// 群成员(%d/%d)
//        /// 群成员(%d)
//        QString text;
//        if (onlineSize>=0)
//            text = QString("%1(%2/%3)").arg(theLocales.getLocalText("user-list.title","User List")).arg(onlineSize).arg(memberSize);
//        else
//            text = QString("%1(%2)").arg(theLocales.getLocalText("user-list.title","User List")).arg(memberSize);
//        setItemText(EbWorkItem::WORK_ITEM_USER_LIST,text);
//    }
//}
