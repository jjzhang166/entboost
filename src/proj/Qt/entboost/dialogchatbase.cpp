#include "dialogchatbase.h"
#include "ui_dialogchatbase.h"
#include "dialogframelist.h"
#include "iconhelper.h"
//#include "ebwidgetchatmessage.h"
#include "ebwidgetchatinput.h"
#include "EbTextBrowser.h"
#include "ebwidgetchatright.h"
#include "ebtextblockuserdata.h"
#include "ebmessagebox.h"

DialogChatBase::DialogChatBase(const EbcCallInfo::pointer& pCallInfo,QWidget *parent) :
    EbDialogBase(parent),
    ui(new Ui::DialogChatBase)
  , m_splitterMain(NULL), m_splitterInput(NULL)
  , m_widgetChatRight(NULL)
  , m_callInfo(pCallInfo)
  , m_bOwnerCall(false)
  , m_nFromLineState(EB_LINE_STATE_UNKNOWN)
  , m_nGroupType(EB_GROUP_TYPE_DEPARTMENT)
{
    assert(m_callInfo.get()!=0);
    ui->setupUi(this);
    updateFromInfo();
    this->setObjectName("DialogChatBase");

    /// 去掉鼠标拖动 & 去掉边框
    EbDialogBase::setMouseEasyMove(false);
    EbDialogBase::setDrawBorder(false);
    /// 设置过滤 ESC/Enter 按键
    EbDialogBase::setFilterEscapeKey(true);
    EbDialogBase::setFilterEnterKey(true);

//    // 设置初始大小
//    const int const_frame_list_width = theLocales.getLocalInt("frame-list.window-size.width", 1180);
//    const int const_frame_list_height = theLocales.getLocalInt("frame-list.window-size.height", 668);
//    const QSize const_frame_list_size(const_frame_list_width,const_frame_list_height);
//    this->resize(const_frame_list_size);
    /// 去掉标题栏
    this->setWindowFlags( Qt::FramelessWindowHint );
    /// 显示右上角关闭按钮（这几个按钮还是要的，因为透明了也没用，点不到下面按钮）
    EbDialogBase::showPushButtonSysClose( theLocales.getLocalText("base-dialog.close-button.tooltip","Close"),"SysClose2Button" );
    EbDialogBase::showPushButtonSysMax( theLocales.getLocalText("base-dialog.maximize-button.tooltip","Maximize"),"SysTrans2Button" );
    EbDialogBase::showPushButtonSysMin( theLocales.getLocalText("base-dialog.minimize-button.tooltip","Minimize"),"SysTrans2Button" );

    /// 显示 名称和描述；
    ui->labelName->setObjectName("DialogChatName");
    int x = 5;
    int y = 5;
    ui->labelName->setGeometry( x,y,width()-80,22 );
    ui->labelDescription->setObjectName("DialogChatDescription");
    y += 22;
    ui->labelDescription->setGeometry( x,y,width()-x-1,22);
    /// 显示 “邀请好友/发送文件/...” 按钮
    IconHelper::Instance()->SetIcon(ui->pushButtonAddUser,QChar(0xf234),14);
    IconHelper::Instance()->SetIcon(ui->pushButtonSendFile,QChar(0xf07c),14);
    IconHelper::Instance()->SetIcon(ui->pushButtonVideo,QChar(0xf03d),14);
    IconHelper::Instance()->SetIcon(ui->pushButtonGroupShare,QChar(0xf0c2),14);
    IconHelper::Instance()->SetIcon(ui->pushButtonChatApps,QChar(0xf009),14);
    IconHelper::Instance()->SetIcon(ui->pushButtonExitChat,QChar(0xf08b),14);   // f05c
    ui->pushButtonAddUser->setToolTip( theLocales.getLocalText("chat-base-dialog.button-add-user.tooltip","add user") );
    ui->pushButtonSendFile->setToolTip( theLocales.getLocalText("chat-base-dialog.button-send-file.tooltip","send user") );
    const bool isGroupChat = true;  // ???
    if (isGroupChat) {
        ui->pushButtonVideo->setToolTip( theLocales.getLocalText("chat-base-dialog.button-group-video.tooltip","group video") );
    }
    else {
        ui->pushButtonVideo->setToolTip( theLocales.getLocalText("chat-base-dialog.button-video-chat.tooltip","video chat") );
    }
    ui->pushButtonGroupShare->setToolTip( theLocales.getLocalText("chat-base-dialog.button-group-share.tooltip","group share") );
    ui->pushButtonChatApps->setToolTip( theLocales.getLocalText("chat-base-dialog.button-chat-apps.tooltip","chat apps") );
    ui->pushButtonExitChat->setToolTip( theLocales.getLocalText("chat-base-dialog.button-exit-chat.tooltip","exit chat") );
    ui->pushButtonAddUser->setObjectName("DialogChatButton");
    ui->pushButtonSendFile->setObjectName("DialogChatButton");
    ui->pushButtonVideo->setObjectName("DialogChatButton");
    ui->pushButtonGroupShare->setObjectName("DialogChatButton");
    ui->pushButtonChatApps->setObjectName("DialogChatButton");
    ui->pushButtonExitChat->setObjectName("DialogChatButton");
    x = 5;
    y += 22;
    const QSize const_chat_base_button_size(36,26);
    ui->pushButtonAddUser->setGeometry( x,y,const_chat_base_button_size.width(),const_chat_base_button_size.height() );
    x += const_chat_base_button_size.width();
    ui->pushButtonSendFile->setGeometry( x,y,const_chat_base_button_size.width(),const_chat_base_button_size.height() );
    x += const_chat_base_button_size.width();
    ui->pushButtonVideo->setGeometry( x,y,const_chat_base_button_size.width(),const_chat_base_button_size.height() );
    x += const_chat_base_button_size.width();
    ui->pushButtonGroupShare->setGeometry( x,y,const_chat_base_button_size.width(),const_chat_base_button_size.height() );
    x += const_chat_base_button_size.width();
    ui->pushButtonChatApps->setGeometry( x,y,const_chat_base_button_size.width(),const_chat_base_button_size.height() );
    x += const_chat_base_button_size.width();
    ui->pushButtonExitChat->setGeometry( x,y,const_chat_base_button_size.width(),const_chat_base_button_size.height() );

    /// 上面边框线
    ui->widgetLine->setObjectName("LineWidget");

    /// 聊天记录
    m_textBrowserMessage = new EbTextBrowser(m_callInfo,this);
    m_textBrowserMessage->setStyleSheet("border: none;");
    m_textBrowserMessage->setReadOnly(true);
    /// ** 屏蔽打开链接（打开会刷新整个界面白屏），由程序内部处理
    m_textBrowserMessage->setOpenLinks(false);
    m_widgetChatInput = new EbWidgetChatInput(m_callInfo,this);
    connect( m_widgetChatInput,SIGNAL(clickedClose()),this,SLOT(onClickedInputClose()) );
    m_splitterInput = new QSplitter(Qt::Vertical, this);
    m_splitterInput->setHandleWidth(1);
    m_splitterInput->addWidget(m_textBrowserMessage);
    m_splitterInput->addWidget(m_widgetChatInput);
    m_splitterInput->setStretchFactor(0,4);
    m_splitterInput->setStretchFactor(1,6);
    /// 聊天右边界面
    m_widgetChatRight = new EbWidgetChatRight(m_callInfo,this);
    m_widgetChatRight->setModal(false);
    m_splitterMain = new QSplitter(Qt::Horizontal, this);
    m_splitterMain->setHandleWidth(1);
    m_splitterMain->addWidget(m_splitterInput);
    m_splitterMain->addWidget(m_widgetChatRight);
    m_splitterMain->setStretchFactor(0,8);
    m_splitterMain->setStretchFactor(1,2);

}

DialogChatBase::~DialogChatBase()
{
    delete ui;
}

void DialogChatBase::setFocusInput()
{
    if (m_widgetChatInput!=0) {
        m_widgetChatInput->setFocusInput();
    }
}

void DialogChatBase::scrollToEnd()
{
    QScrollBar *scrollbar = m_textBrowserMessage->verticalScrollBar();
    scrollbar->setSliderPosition(scrollbar->maximum());
}

void DialogChatBase::setCallInfo(const EbcCallInfo::pointer &pCallInfo)
{
    m_callInfo = pCallInfo;
    assert(m_callInfo.get()!=0);
    if ( m_textBrowserMessage!=0 ) {
        m_textBrowserMessage->setCallInfo(m_callInfo);
    }
    if (m_widgetChatInput!=0) {
        m_widgetChatInput->setCallInfo(m_callInfo);
    }
    if (m_widgetChatRight!=0) {
        m_widgetChatRight->setCallInfo(m_callInfo);
    }
}

bool DialogChatBase::clearUnreadMsg(bool bFromUserClick)
{
    if (m_callInfo.get()==0) {
        return false;
    }
    char sSql[256];
    if (m_callInfo->m_pCallInfo.m_sGroupCode>0) {
        if (bFromUserClick)
            theApp->m_pAutoCallGroupIdList.remove(m_callInfo->m_pCallInfo.m_sGroupCode);
        else if (theApp->m_pAutoCallGroupIdList.exist(m_callInfo->m_pCallInfo.m_sGroupCode)) return false;
        sprintf(sSql,"UPDATE msg_record_t SET read_flag=read_flag|1 WHERE dep_code=%lld AND (read_flag&1)=0",m_callInfo->m_pCallInfo.m_sGroupCode);
    }
    else {
        if (bFromUserClick)
            theApp->m_pAutoCallFromUserIdList.remove(m_callInfo->m_pFromAccountInfo.GetUserId());
        else if (theApp->m_pAutoCallFromUserIdList.exist(m_callInfo->m_pFromAccountInfo.GetUserId())) return false;
        sprintf(sSql,"UPDATE msg_record_t SET read_flag=read_flag|1 WHERE from_uid=%lld AND dep_code=0 AND (read_flag&1)=0",m_callInfo->m_pFromAccountInfo.GetUserId());
    }
    theApp->m_sqliteUser->execute(sSql);
    return true;
}

int DialogChatBase::getUnreadMsgCount() const
{
    // for test
    if (theApp->m_sqliteUser.get()==0) return 0;
    if (m_callInfo.get()==NULL) return 0;
    char sSql[256];
    if (m_callInfo->m_pCallInfo.m_sGroupCode>0) {
        sprintf(sSql,"SELECT count(read_flag) FROM msg_record_t WHERE dep_code=%lld AND (read_flag&1)=0",m_callInfo->m_pCallInfo.m_sGroupCode);
    }
    else {
        sprintf(sSql,"SELECT count(read_flag) FROM msg_record_t WHERE from_uid=%lld AND dep_code=0 AND (read_flag&1)=0",m_callInfo->m_pFromAccountInfo.GetUserId());
    }
    int nResult = 0;
    int nCookie = 0;
    theApp->m_sqliteUser->select(sSql, nCookie);
    cgcValueInfo::pointer pRecord = theApp->m_sqliteUser->first(nCookie);
    if (pRecord.get()!=NULL) {
        nResult = pRecord->getVector()[0]->getIntValue();
        theApp->m_sqliteUser->reset(nCookie);
    }
    return nResult;
}

void DialogChatBase::onUserExitRoom(eb::bigint nUserId, bool bExitDep)
{
    if (m_widgetChatRight!=0) {
        m_widgetChatRight->onUserExit(nUserId,bExitDep);
    }
    if ( this->isGroupChat() ) {
        ///
    }
    else {
        if (theApp->m_ebum.EB_IsLogonVisitor() ||
            m_callInfo->m_pFromAccountInfo.m_pFromCardInfo.m_nAccountType == EB_ACCOUNT_TYPE_VISITOR) {	// 游客不能被动邀请
            /// 不是公司内部员工，需要重新呼叫才能通讯
            /// "对方已经退出本次会话！"
            m_textBrowserMessage->addLineString(0,theLocales.getLocalText("chat-msg-text.on-user-exit","User Exit") );
        }
        if (m_widgetChatInput!=0) {
            m_widgetChatInput->onUserExit(nUserId);
        }
        // ?
        //size_t nUserSize = m_callInfo->m_pChatRoom->GetUserSize();
        //if (nUserSize <= 1)
        //{
        //	m_btnSendFile.EnableWindow(FALSE);
        //}
    }
}

void DialogChatBase::onAlertingCall()
{
    /// "对方已经退出本次会话，正在重新邀请，请稍候...！"
    m_textBrowserMessage->addLineString(0,theLocales.getLocalText("chat-msg-text.alerting-call","Alerting Call") );
}

void DialogChatBase::onSendRich(const CCrRichInfo *pCrMsgInfo, EB_STATE_CODE nState)
{
    processMsg(false,pCrMsgInfo,nState);
}

void DialogChatBase::onReceiveRich(const CCrRichInfo *pCrMsgInfo, QString *sOutFirstMsg1, QString *sOutFirstMsg2)
{
    if (m_widgetChatInput!=0) {
        m_widgetChatInput->onReceiveRich();
    }
    processMsg(true,pCrMsgInfo,EB_STATE_OK,sOutFirstMsg1,sOutFirstMsg2);
//    if (this->isVisible()) {
//            ::FlashWindow(this->GetParent()->GetSafeHwnd(), TRUE);
    //    }
}

void DialogChatBase::onUserLineStateChange(eb::bigint nGroupCode, eb::bigint nUserId, EB_USER_LINE_STATE bLineState)
{
    if (groupId()==0 && fromUserId()==nUserId) {
        m_nFromLineState = bLineState;
    }
    else if (nGroupCode>0 && groupId()==nGroupCode) {
        /// 更新群组成员
//        if (m_pDlgChatRight.get()!=NULL)
//        {
//            m_pDlgChatRight->LineStateChange(nUserId, bLineState);
//        }
    }

}

void DialogChatBase::onMemberInfo(const EB_MemberInfo *pMemberInfo, bool bSort)
{
//    if (m_pDlgSelectUser.GetSafeHwnd()!=NULL)
//        m_pDlgSelectUser.onMemberInfo(pMemberInfo);

//    if (m_pDlgChatInput.get()!=NULL && pMemberInfo->m_sGroupCode==this->m_callInfo->m_pCallInfo.m_sGroupCode)
//    {
//        m_pDlgChatInput->OnUserEmpInfo(pMemberInfo);
//    }

//    if (m_pDlgChatRight.get()!=NULL && pMemberInfo->m_sGroupCode==this->m_callInfo->m_pCallInfo.m_sGroupCode)
//    {
//        m_pDlgChatRight->OnUserEmpInfo(pMemberInfo,bSort);
    //    }
}

bool DialogChatBase::onMemberHeadChange(const EB_MemberInfo *pMemberInfo)
{
    if (this->isGroupChat()) return false;
    if (m_callInfo->m_pFromAccountInfo.GetUserId()!=pMemberInfo->m_nMemberUserId) return false;
    if (m_imageMd5==pMemberInfo->m_sHeadMd5) return false;

    if ( QFileInfo::exists(pMemberInfo->m_sHeadResourceFile.c_str()) ) {
        m_imageMd5 = pMemberInfo->m_sHeadMd5;
        m_fromImage = QImage( pMemberInfo->m_sHeadResourceFile.c_str() );
        return true;
    }
    return false;
}

bool DialogChatBase::onContactHeadChange(const EB_ContactInfo *pContactInfo)
{
    if (this->isGroupChat()) return false;
    if (m_callInfo->m_pFromAccountInfo.GetUserId()!=pContactInfo->m_nContactUserId) return false;
    if (m_imageMd5==pContactInfo->m_sHeadMd5) return false;

    if ( QFileInfo::exists(pContactInfo->m_sHeadResourceFile.c_str()) ) {
        m_imageMd5 = pContactInfo->m_sHeadMd5;
        m_fromImage = QImage( pContactInfo->m_sHeadResourceFile.c_str() );
        return true;
    }
    return false;
}

void DialogChatBase::onRemoveGroup(mycp::bigint nGroupId)
{
//    if (m_pDlgSelectUser.GetSafeHwnd()!=NULL)
//        m_pDlgSelectUser.OnRemoveGroup(nGroupId);

}

void DialogChatBase::onRemoveMember(mycp::bigint nGroupId, mycp::bigint nMemberId, mycp::bigint memberUserId)
{
//    if (m_pDlgSelectUser.GetSafeHwnd()!=NULL)
//		m_pDlgSelectUser.OnRemoveMember(nGroupId, nMemberId);
    if ( this->groupId()==nGroupId ) {
        onUserExitRoom( memberUserId,true );
    }
}

void DialogChatBase::onMsgReceipt(const CCrRichInfo *pCrMsgInfo, int nAckType)
{
    const eb::bigint nFromUserId = pCrMsgInfo->m_sSendFrom;
    const eb::bigint nMsgId = pCrMsgInfo->m_pRichMsg->GetMsgId();
    updateMsgReceiptData(nMsgId, nFromUserId, nAckType, pCrMsgInfo->GetStateCode());

    if ( m_widgetChatRight!=0 ) {
        m_widgetChatRight->onMsgReceipt(pCrMsgInfo, nAckType);
    }
}

void DialogChatBase::updateMsgReceiptData(eb::bigint nMsgId, eb::bigint nFromUserId, int nAckType, EB_STATE_CODE nState)
{
    /// nAckType=0 收到消息回执
    /// nAckType=4 请求撤回消息
    /// nAckType=6 请求“个人收藏”消息
    /// nAckType=7 请求“群收藏”消息
    /// *** read_flag=1 已经读；
    /// *** read_flag=2 对方接收回执
    if (nState==EB_STATE_OK)
        theApp->updateMsgReceiptData(nMsgId, nFromUserId, nAckType);

    if (nAckType==6 ||	/// 个人收藏
            nAckType==7) {			/// 群收藏
        QString text;
        EbMessageBox::IMAGE_TYPE imageType = EbMessageBox::IMAGE_WARNING;
        if (nState==EB_STATE_OK) {   /// 消息收藏成功！
            imageType = EbMessageBox::IMAGE_INFORMATION;
            text = theLocales.getLocalText("message-show.collect-msg-ok","");
        }
        else if (nState==EB_STATE_NOT_AUTH_ERROR)   /// 没有操作权限：\r\n消息收藏失败！
            text = theLocales.getLocalText("message-show.collect-msg-not-auth","");
        else    /// 消息不存在或者已经被删除：\r\n消息收藏失败！
            text = theLocales.getLocalText("message-show.collect-msg-error","");
        if ( !text.isEmpty() ) {
            text.replace( "[STATE_CODE]", QString::number((int)nState) );
            EbMessageBox::doShow( NULL, "", QChar::Null, text, imageType, default_warning_auto_close );
        }
    }
    else if (nAckType==4) {
        /// 请求撤回消息
        if (nState==EB_STATE_OK) {
            /// 1: 表示只更新第1行，第0行是标题，不更新；
            /// [撤回一条消息]
            const QString text = theLocales.getLocalText("withdraw-msg-response.state-ok-update","");
            const QString newText = QString("<font color=#808080>%1</font>").arg(text);
            m_textBrowserMessage->updateMsgText(nMsgId,newText,1);
//            m_pMrFrameInterface->SetMsgReceiptFlag(nMsgId, EBC_CONTRON_RECEIPT_FLAG_HIDE);
        }
        if ( nFromUserId==theApp->logonUserId() ) {
            if (nState!=EB_STATE_OK) {
                /// 发送时间超过2分钟的消息：<br>不能撤回！
                QString text = theLocales.getLocalText("withdraw-msg-response.other-error.text","");
                if ( !text.isEmpty() ) {
                    text.replace( "[STATE_CODE]", QString::number((int)nState) );
                    EbMessageBox::doShow( NULL, "", QChar::Null, text, EbMessageBox::IMAGE_WARNING, default_warning_auto_close );
                }
            }
            else {
                /// 请求撤回了一条消息！
                m_textBrowserMessage->addLineString(0, theLocales.getLocalText("withdraw-msg-response.state-ok","") );
            }
        }
        else if (m_callInfo->groupId()==0) {
            /// 对方撤回了一条消息！
            m_textBrowserMessage->addLineString(0, theLocales.getLocalText("withdraw-msg-response.msg-receipt-dest","") );
        }
        else {
            /// %s 撤回了一条消息！
            tstring sMemberUserName;
            theApp->m_ebum.EB_GetMemberNameByUserId(m_callInfo->groupId(),nFromUserId,sMemberUserName);
            QString text = theLocales.getLocalText("withdraw-msg-response.msg-receipt-group","");
            text.replace( "[USER_NAME]",sMemberUserName.c_str() );
            m_textBrowserMessage->addLineString(0,text);
        }
    }
    else if (nAckType==0) {	/// ?
        const long nReceiptFlag = EBC_CONTRON_RECEIPT_FLAG_TRUE|EBC_CONTRON_RECEIPT_FLAG_SHOW;
        m_textBrowserMessage->setMsgReceiptFlag(nMsgId, nReceiptFlag);
    }
}

void DialogChatBase::onSendingFile(const CCrFileInfo *pCrFileInfo)
{
    if ( pCrFileInfo->GetParam()==0 && pCrFileInfo->m_nMsgId>0 && pCrFileInfo->GetStateCode()==EB_STATE_OK ) {
        if ( theApp->isSaveConversationLocal()  && !theApp->isLogonVisitor() ) {
            //if (pCrFileInfo->m_sReceiveAccount==theEBAppClient.EB_GetLogonUserId())
            {
                tstring sInFromName;
                if (m_callInfo->m_pCallInfo.m_sGroupCode==0)
                    sInFromName = theApp->m_ebum.EB_GetUserName();
                else
                    theApp->m_ebum.EB_GetMemberNameByUserId(m_callInfo->groupId(),theApp->logonUserId(),sInFromName);
                theApp->m_sqliteUser->escape_string_in(sInFromName);
                tstring sInFileName(pCrFileInfo->m_sFileName);
                theApp->m_sqliteUser->escape_string_in(sInFileName);
                const eb::bigint sSaveDbToAccount = m_callInfo->groupId()==0?m_callInfo->fromUserId():pCrFileInfo->m_sSendTo;
                char sSql[1024];
                sprintf( sSql, "INSERT INTO msg_record_t(msg_id,dep_code,from_uid,from_name,to_uid,private,msg_type,msg_text) "
                               "VALUES(%lld,%lld,%lld,'%s',%lld,%d,%d,'%s')",
                    pCrFileInfo->m_nMsgId,m_callInfo->groupId(),pCrFileInfo->m_sSendFrom,sInFromName.c_str(),
                    sSaveDbToAccount,pCrFileInfo->m_bPrivate?1:0,MRT_FILE,sInFileName.c_str() );
                theApp->m_sqliteUser->execute(sSql);
            }
        }

        const tstring sFileName = libEbc::GetFileName(pCrFileInfo->m_sFileName);
        /// 发送文件：%s
        const QString sWindowText = QString("%1: %2").arg(theLocales.getLocalText("chat-msg-text.send-file","Send File")).arg(sFileName.c_str());
        m_textBrowserMessage->addLineString(pCrFileInfo->m_nMsgId,sWindowText);
    }
    if (m_widgetChatRight!=0) {
        m_widgetChatRight->onSendingFile(pCrFileInfo);
    }
}

bool DialogChatBase::onSentFile(const CCrFileInfo *fileInfo, EB_STATE_CODE nState)
{
    bool ret = false;
    if (fileInfo->GetParam()==0) {
        //    if (pCrFileInfo->GetParam()==0 && m_pDlgChatInput.get() != NULL && m_pDlgChatInput->GetSafeHwnd())
        if ( theApp->isSaveConversationLocal() && !theApp->isLogonVisitor() ) {
            //if (pCrFileInfo->m_sReceiveAccount==theEBAppClient.EB_GetLogonUserId())
            {
                char sSql[1024];
                if (nState==EB_STATE_FILE_ALREADY_EXIST) {
                    /// ** 保存本地数据库
                    if ( m_callInfo->isGroupCall() ) {
                        char sMsgText[64];
                        sprintf( sMsgText, "%lld,%lld",fileInfo->m_sResId,fileInfo->m_nFileSize);
                        tstring sInMemberName;
                        theApp->m_ebum.EB_GetMemberNameByUserId(m_callInfo->groupId(),fileInfo->m_sSendFrom,sInMemberName);
                        theApp->m_sqliteUser->escape_string_in(sInMemberName);
                        const tstring sFileName = libEbc::GetFileName(fileInfo->m_sFileName);
                        tstring sInFileName(sFileName);
                        theApp->m_sqliteUser->escape_string_in(sInFileName);
                        sprintf( sSql, "INSERT INTO msg_record_t(msg_id,dep_code,from_uid,from_name,to_uid,private,msg_type,msg_name,msg_text,read_flag) "
                                       "VALUES(%lld,%lld,%lld,'%s',%lld,%d,%d,'%s','%s',%d)",
                                 fileInfo->m_nMsgId,m_callInfo->groupId(),fileInfo->m_sSendFrom,sInMemberName.c_str(),
                                 fileInfo->m_sSendTo,fileInfo->m_bPrivate?1:0,MRT_RESOURCE,sInFileName.c_str(),sMsgText,(int)EBC_READ_FLAG_SENT);
                    }
                    else {
                        tstring sInFromName(theApp->m_ebum.EB_GetUserName());
                        theApp->m_sqliteUser->escape_string_in(sInFromName);
                        tstring sInFileName(fileInfo->m_sFileName);
                        theApp->m_sqliteUser->escape_string_in(sInFileName);
                        const eb::bigint sSaveDbToAccount = m_callInfo->fromUserId();
                        //const eb::bigint sSaveDbToAccount = m_callInfo->m_pCallInfo.m_sGroupCode==0?m_callInfo->m_pFromAccountInfo.GetUserId():pCrFileInfo->m_sSendTo;
                        sprintf( sSql, "INSERT INTO msg_record_t(msg_id,dep_code,from_uid,from_name,to_uid,private,msg_type,msg_text) "
                                       "VALUES(%lld,%lld,%lld,'%s',%lld,%d,%d,'%s')",
                                 fileInfo->m_nMsgId,m_callInfo->groupId(),fileInfo->m_sSendFrom,sInFromName.c_str(),
                                 sSaveDbToAccount,fileInfo->m_bPrivate?1:0,MRT_FILE,sInFileName.c_str());
                    }
                }
                else {
                    sprintf( sSql, "UPDATE msg_record_t SET read_flag=read_flag|%d WHERE msg_id=%lld AND (read_flag&%d)=0",EBC_READ_FLAG_SENT,fileInfo->m_nMsgId,EBC_READ_FLAG_SENT);
                }
                theApp->m_sqliteUser->execute(sSql);
            }
        }
        ret = processFile(false, fileInfo, nState);
    }
    if (m_widgetChatRight!=0) {
        m_widgetChatRight->deleteTranFile(fileInfo->m_nMsgId);
    }
//    if (this->GetParent()->IsWindowVisible())
//        ::FlashWindow(this->GetParent()->GetSafeHwnd(), TRUE);
    return ret;
}

void DialogChatBase::onCancelFile(const CCrFileInfo *fileInfo, bool bChangeP2PSending)
{
    if (m_widgetChatRight!=0) {
        m_widgetChatRight->deleteTranFile(fileInfo->m_nMsgId);
    }
    if (fileInfo->GetParam()==0) {
//    if (m_pDlgChatInput.get() != NULL && m_pDlgChatInput->GetSafeHwnd() && pCrFileInfo->GetParam()==0)//m_callInfo->m_pCallInfo.m_sGroupCode==0)
        if (!bChangeP2PSending) {
            char sSql[128];
            sprintf( sSql, "DELETE FROM msg_record_t WHERE msg_id=%lld",fileInfo->m_nMsgId);
            theApp->m_sqliteUser->execute(sSql);
        }
        const tstring sFileName = libEbc::GetFileName(fileInfo->m_sFileName);
        QString sWindowText;
        if (bChangeP2PSending) {
            /// 对方在线，使用P2P点对点发送:
            sWindowText = QString("%1: %2").arg(theLocales.getLocalText("chat-msg-text.change-p2p-sending","Change P2P Sending")).arg(sFileName.c_str());
        }
        else if ( fileInfo->m_sSendFrom!=theApp->logonUserId() || fileInfo->m_sReceiveAccount==0
                 || fileInfo->m_sReceiveAccount==theApp->logonUserId() ) {
            /// 取消文件:
            sWindowText = QString("%1: %2").arg(theLocales.getLocalText("chat-msg-text.cancel-file","Cancel File")).arg(sFileName.c_str());
        }
        else if (m_callInfo->groupId()==0) {
            /// 对方拒绝接收文件：
            sWindowText = QString("%1: %2").arg(theLocales.getLocalText("chat-msg-text.dest-reject-file","Dest Reject File")).arg(sFileName.c_str());
        }
        else {
            /// %s 拒绝接收文件：%s
            CEBString sMemberName;
            theApp->m_ebum.EB_GetMemberNameByUserId(m_callInfo->groupId(),fileInfo->m_sReceiveAccount,sMemberName);
            sWindowText = QString("%1: %2 %3").arg(sMemberName.c_str()).arg(theLocales.getLocalText("chat-msg-text.reject-file","Reject File")).arg(sFileName.c_str());
        }
        m_textBrowserMessage->addLineString(fileInfo->m_nMsgId,sWindowText);
    }
}

void DialogChatBase::onReceivingFile(const CCrFileInfo *fileInfo, QString *sOutFirstMsg)
{
    const bool bOffLineUser = m_callInfo->m_bOffLineUser;
    const eb::bigint sSendFrom = fileInfo->m_sSendFrom;
    const tstring sFileName = libEbc::GetFileName(fileInfo->m_sFileName);
    QString sWindowText;
    if (this->m_callInfo->isGroupCall() && fileInfo->m_sResId>0) {

        /// ** 这里必须用 false ???
        m_textBrowserMessage->addFileMsg( false,fileInfo,fileInfo->GetStateCode() );

        CEBString sMemberName;
        theApp->m_ebum.EB_GetMemberNameByUserId(m_callInfo->groupId(),sSendFrom,sMemberName);
//        /// %1 上传群共享文件：%s // send-group-file
//        sWindowText = QString("%1 %2: %3").arg(sMemberName.c_str())
//                .arg(theLocales.getLocalText("chat-msg-text.send-group-file","Send Group File")).arg(sFileName.c_str());

//        const mycp::bigint nFileSize = fileInfo->m_nFileSize;
//        // *
//        CString sFileText;
//        if (nFileSize >= const_gb_size)
//            sFileText.Format(_T("上传群共享文件：%s(%.02fGB)"),sFileName.c_str(),(double)nFileSize/const_gb_size);
//        else if (nFileSize >= const_mb_size)
//            sFileText.Format(_T("上传群共享文件：%s(%.02fMB)"),sFileName.c_str(),(double)nFileSize/const_mb_size);
//        else if (nFileSize >= const_kb_size)
//            sFileText.Format(_T("上传群共享文件：%s(%.02fKB)"),sFileName.c_str(),(double)nFileSize/const_kb_size);
//        else if (nFileSize>0)
//            sFileText.Format(_T("上传群共享文件：%s(%lldByte)"),sFileName.c_str(),nFileSize);
//        else
//            sFileText.Format(_T("上传群共享文件：%s"), sFileName.c_str());

//        CString sText;
//        sText.Format(_T("%s"),sMemberName.c_str());
//        //this->AddLineString(0, sText, 1);	// 显示中间
//        const bool bReceive = true;
//        m_pMrFrameInterface->AddLine(fileInfo->m_nMsgId);
//        //m_pMrFrameInterface->AddLine(0);
//        //m_pMrFrameInterface->SetAlignmentFormat(1);
//        m_pMrFrameInterface->SetAlignmentFormat(bReceive?0:2);
//        m_pMrFrameInterface->WriteString((LPCTSTR)sText,theDefaultChatSystemColor);
//        m_pMrFrameInterface->WriteSpace(1);
//        m_pMrFrameInterface->WriteTime(0,"%H:%M");

//        m_pMrFrameInterface->AddLine(fileInfo->m_nMsgId);
//        //m_pMrFrameInterface->SetAlignmentFormat(1);
//        m_pMrFrameInterface->SetAlignmentFormat(bReceive?0:2);
//        m_pMrFrameInterface->SetFrameArc(theArcOffset,thePoloygonWidth,thePoloygonHeight);
//        m_pMrFrameInterface->SetFrameBorderColor(RGB(128,128,128));
//        m_pMrFrameInterface->SetFrameBackGroundColor(bReceive?theDefaultChatBackGroundColor2:theDefaultChatBackGroundColor1);
//        WriteFileHICON(sFileName.c_str(),0);
//        m_pMrFrameInterface->WriteString((LPCTSTR)sFileText,theDefaultChatSystemColor);
//        m_pMrFrameInterface->WriteSpace(1);
//        sText.Format(_T("#CTRL:%d:%d:%lld,%s#下载"),(int)(EB_MR_CTRL_TYPE_LCLICK_CB),(int)EB_MR_CTRL_DATA_TYPE_DOWNLOAD_RESOURCE,fileInfo->m_sResId,sFileName.c_str());
//        m_pMrFrameInterface->WriteString((LPCTSTR)sText,RGB(0, 0, 255));
//        if (m_pCallInfo.m_sGroupCode>0 && !theApp.GetDisableGroupSharedCloud())
//        {
//            m_pMrFrameInterface->WriteSpace(2);
//            sText.Format(_T("#CTRL:%d:%d:0#群共享"),(int)(EB_MR_CTRL_TYPE_LCLICK_CB),(int)EB_MR_CTRL_DATA_TYPE_OPEN_SHARE);
//            m_pMrFrameInterface->WriteString((LPCTSTR)sText,RGB(0, 0, 255));
//        }
//        const bool hIsScrollEnd = (!bReceive || m_pMrFrameInterface->IsScrollEnd()==VARIANT_TRUE)?true:false;
//        m_pMrFrameInterface->UpdateSize(VARIANT_TRUE);
//        if (hIsScrollEnd)
//            m_pMrFrameInterface->ScrollToPos(-1);

        /// ** 保存本地数据库
        if ( theApp->isSaveConversationLocal() && !theApp->isLogonVisitor() ) {
            char sMsgText[64];
            sprintf( sMsgText, "%lld,%lld",fileInfo->m_sResId,fileInfo->m_nFileSize);
            tstring sInMemberName(sMemberName);
            theApp->m_sqliteUser->escape_string_in(sInMemberName);
            tstring sInFileName(sFileName);
            theApp->m_sqliteUser->escape_string_in(sInFileName);
            char sSql[1024];
            sprintf( sSql, "INSERT INTO msg_record_t(msg_id,dep_code,from_uid,from_name,to_uid,private,msg_type,msg_name,msg_text) "
                           "VALUES(%lld,%lld,%lld,'%s',%lld,%d,%d,'%s','%s')",
                fileInfo->m_nMsgId,m_callInfo->groupId(),fileInfo->m_sSendFrom,sInMemberName.c_str(),
                fileInfo->m_sSendTo,fileInfo->m_bPrivate?1:0,MRT_RESOURCE,sInFileName.c_str(),sMsgText);
            theApp->m_sqliteUser->execute(sSql);
        }
    }
    else if (bOffLineUser || fileInfo->m_bOffFile) {
        //if (m_pCallInfo->m_bOffLineCall)
        //if (m_bReceiveOffLineMsg)
        /// 对方发送离线文件：%s
        sWindowText = QString("%1: %2").arg(theLocales.getLocalText("chat-msg-text.dest-send-off-file","Dest Send Off File")).arg(sFileName.c_str());
        m_textBrowserMessage->addLineString(fileInfo->m_nMsgId,sWindowText);
    }
    else {
        /// 对方发送文件：%s
        sWindowText = QString("%1: %2").arg(theLocales.getLocalText("chat-msg-text.dest-send-file","Dest Send File")).arg(sFileName.c_str());
        m_textBrowserMessage->addLineString(fileInfo->m_nMsgId,sWindowText);
    }
//    if (sOutFirstMsg!=NULL)
//    {
//        if (m_pCallInfo.m_sGroupCode>0)
//            sOutFirstMsg->Format(_T("<font color=\"#6c6c6c\">%s</font><br/><a href=\"ebim-call-group://%lld\">%s</a>"),libEbc::ACP2UTF8(sWindowText).c_str(),m_pCallInfo.m_sGroupCode,libEbc::ACP2UTF8("接收文件").c_str());
//        else
//            sOutFirstMsg->Format(_T("<font color=\"#6c6c6c\">%s</font><br/><a href=\"ebim-call-account://%lld\">%s</a>"),libEbc::ACP2UTF8(sWindowText).c_str(),m_pCallInfo.GetFromUserId(),libEbc::ACP2UTF8("接收文件").c_str());
//    }

    if ( m_widgetChatRight!=0 ) {
        if ( m_callInfo->isGroupCall() && fileInfo->m_sResId>0 &&
                fileInfo->m_sSendFrom>0 && fileInfo->m_sSendFrom!=theApp->logonUserId() ) {
            /// ** 对方发送群共享文件，在线消息不处理
            return;
        }
        m_widgetChatRight->onReceivingFile(fileInfo);
//        if (this->IsWindowVisible())
//            ::FlashWindow(this->GetParent()->GetSafeHwnd(), TRUE);
    }

}

void DialogChatBase::onReceivedFile(const CCrFileInfo *fileInfo)
{
    m_textBrowserMessage->addFileMsg( true,fileInfo,fileInfo->GetStateCode() );

    if ( fileInfo->GetParam()==0 && theApp->isSaveConversationLocal() && !theApp->isLogonVisitor() ) {
        tstring sFromName;
        eb::bigint sSaveDbToAccount = fileInfo->m_sSendTo;
        if ( !m_callInfo->isGroupCall() ) {
            sFromName = m_sFromName.toStdString();
        }
        else {
            theApp->m_ebum.EB_GetMemberNameByUserId(m_callInfo->groupId(),fileInfo->m_sSendFrom,sFromName);
        }
        theApp->m_sqliteUser->escape_string_in(sFromName);
        tstring sInFileName(fileInfo->m_sFileName);
        theApp->m_sqliteUser->escape_string_in(sInFileName);
        char sSql[1024];
        sprintf( sSql, "INSERT INTO msg_record_t(msg_id,dep_code,from_uid,from_name,to_uid,private,msg_type,msg_text) "
                       "VALUES(%lld,%lld,%lld,'%s',%lld,%d,%d,'%s')",
                    fileInfo->m_nMsgId,m_callInfo->groupId(),fileInfo->m_sSendFrom,sFromName.c_str(),
                    sSaveDbToAccount,fileInfo->m_bPrivate?1:0,MRT_FILE,sInFileName.c_str());
        theApp->m_sqliteUser->execute(sSql);
    }
    if (m_widgetChatRight!=0) {
        m_widgetChatRight->onReceivedFile(fileInfo);
    }
    //    ::FlashWindow(this->GetParent()->GetSafeHwnd(), TRUE);
}

void DialogChatBase::onFilePercent(const CChatRoomFilePercent *pChatRoomFilePercent)
{
    if (m_widgetChatRight!=0) {
        m_widgetChatRight->onFilePercent(pChatRoomFilePercent);
    }
}

void DialogChatBase::onClickedInputClose()
{
    if (!beforeClose()) {
        /// 检查不能退出会话，直接返回
        return;
    }
    emit clickedClose();
}

bool DialogChatBase::onBeforeClickedPushButtonSysMin()
{
    DialogFrameList* parent = (DialogFrameList*)this->parentWidget();
    parent->onClickedPushButtonSysMin();
//    QApplication::postEvent(this->parent(), new QEvent((QEvent::Type)EB_COMMAND_SYS_MIN));
    return false;
}

bool DialogChatBase::onBeforeClickedPushButtonSysMax()
{
    DialogFrameList* parent = (DialogFrameList*)this->parentWidget();
    parent->onClickedPushButtonSysMax();
//    QApplication::postEvent(this->parent(), new QEvent((QEvent::Type)EB_COMMAND_SYS_MAX));
    return false;
}

bool DialogChatBase::beforeClose()
{
//    if (m_pDlgChatRight.get()!=NULL)
//    {
//        bool pVideoProcessing = false;
//        bool pFileProcessing = false;
//        bool pDesktopProcessing = false;
//        m_pDlgChatRight->GetProcessing(pVideoProcessing,pFileProcessing,pDesktopProcessing);
//        if (pVideoProcessing && pFileProcessing)
//        {
//            if (CDlgMessageBox::EbDoModal(this,"退出会话",_T("正在视频通话和传输文件：\t\n确定退出吗？"),CDlgMessageBox::IMAGE_QUESTION)!=IDOK)
//            {
//                return;
//            }
//        }else if (pVideoProcessing)
//        {
//            if (CDlgMessageBox::EbDoModal(this,"退出会话",_T("正在视频通话：\t\n确定退出吗？"),CDlgMessageBox::IMAGE_QUESTION)!=IDOK)
//            {
//                return;
//            }
//        }else if (pFileProcessing)
//        {
//            if (CDlgMessageBox::EbDoModal(this,"退出会话",_T("正在传输文件：\t\n确定退出吗？"),CDlgMessageBox::IMAGE_QUESTION)!=IDOK)
//            {
//                return;
//            }
//        }
//        if (pDesktopProcessing)
//        {
//            if (CDlgMessageBox::EbDoModal(this,"退出会话",_T("正在远程桌面中：\t\n确定退出吗？"),CDlgMessageBox::IMAGE_QUESTION)!=IDOK)
//            {
//                return;
//            }
//        }
//        m_pDlgChatRight->ExitChat(false);
//    }
//    this->GetParent()->PostMessage(EB_COMMAND_RAME_WND_CLOSE,(WPARAM)(CWnd*)this,0);

    theApp->m_ebum.EB_CallExit(this->m_callInfo->m_pCallInfo.GetCallId());
    theApp->m_pCallList.remove(this->m_callInfo->m_pCallInfo.GetCallId());
    return true;
}

void DialogChatBase::reject()
{
    if (!beforeClose()) {
        /// 检查不能退出会话，直接返回
        return;
    }

    DialogFrameList* parent = (DialogFrameList*)this->parentWidget();
    parent->reject();//onClickedPushButtonSysClose();
//    QApplication::postEvent(this->parent(), new QEvent((QEvent::Type)EB_COMMAND_SYS_CLOSE));
//    return false;
}

void DialogChatBase::resizeEvent(QResizeEvent * e)
{
    updateSize();
    DialogFrameList* parent = (DialogFrameList*)this->parentWidget();
    if (parent->isMaximizedEb()) {
        // 当前窗口最大化，显示还原按钮
        EbDialogBase::setMaxRestoreIcon(false);
    }
    else {
        // 当前窗口还原，显示最大化按钮
        EbDialogBase::setMaxRestoreIcon(true);
    }
    EbDialogBase::resizeEvent(e);
}

void DialogChatBase::keyPressEvent(QKeyEvent * e)
{
    if (e->key()==Qt::Key_Escape) {
        onClickedInputClose();
        return;
    }
    EbDialogBase::keyPressEvent(e);
}

void DialogChatBase::updateSize()
{
//    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
//    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
//        ui->widgetLeftBackground->setGeometry( 0, 0, m_showedLeft?const_border_left:const_hide_border_chat, height() );
//    }
//    else {
//        ui->widgetLeftBackground->setGeometry( 0, 0, m_showedLeft?const_border_left:const_hide_border_offiice, height() );
//    }
//    ui->pushButtonShowHideLeft->move(8,height()-(8+16));
    int y = 78;
    ui->widgetLine->setGeometry( 0,y,width(),1 );
    y += 1;
    if (m_splitterMain!=NULL) {
        m_splitterMain->setGeometry( 0,y,width(),height()-y );
    }

}

const int const_max_length = 80;

void DialogChatBase::processMsg(bool bReceive,const CCrRichInfo* pCrMsgInfo,EB_STATE_CODE nState,QString* sOutFirstMsg1,QString* sOutFirstMsg2)
{
    if (nState==EB_STATE_MAX_CAPACITY_ERROR) {
        /// 超过最大消息长度！
        m_textBrowserMessage->addLineString(0,theLocales.getLocalText("chat-msg-text.max-capacity-error","Max Capacity Error") );
        return;
    }
    else if (nState==EB_STATE_GROUP_FORBID_SPEECH) {
        /// 群禁言中！
        m_textBrowserMessage->addLineString(0,theLocales.getLocalText("chat-msg-text.group-forbig-speech","Group Forbig Speech") );
        return;
    }
    else if (nState==EB_STATE_FORBIG_SPEECH) {
        /// 你被禁言中！
        m_textBrowserMessage->addLineString(0,theLocales.getLocalText("chat-msg-text.forbig-speech","Forbig Speech") );
        return;
    }
    m_textBrowserMessage->addRichMsg( true, bReceive, pCrMsgInfo, nState, sOutFirstMsg1, sOutFirstMsg2 );

//    QTextCursor cursor = this->m_textBrowserMessage->textCursor();
//    const bool hIsScrollEnd = (!bReceive || cursor.atEnd())?true:false;
//    if (hIsScrollEnd)
//        cursor.movePosition(QTextCursor::End);
    QScrollBar *scrollbar = m_textBrowserMessage->verticalScrollBar();
    scrollbar->setSliderPosition(scrollbar->maximum());

    if (nState==EB_STATE_EXCESS_QUOTA_ERROR) {
        /// 发送不成功，对方离线消息空间已满！
        m_textBrowserMessage->addLineString(0,theLocales.getLocalText("chat-msg-text.excess-quota-error","Excess Quota Error") );
    }
    else if (EB_STATE_NOT_MEMBER_ERROR==nState && m_callInfo->m_pCallInfo.m_sGroupCode>0) {
        /// 没有其他群组成员，不能发送聊天信息！
        m_textBrowserMessage->addLineString(0,theLocales.getLocalText("chat-msg-text.not-member-error","Not Member Error") );
    }
    else if (nState!=EB_STATE_OK && nState!=EB_STATE_FORWARD_MSG) {
        /// 发送失败，请重试！错误代码: %1
        QString sText = theLocales.getLocalText("chat-msg-text.send-rich-error","Send Rich Error");
        sText.replace("[STATE_CODE]",QString::number((int)nState));
        m_textBrowserMessage->addLineString(0,sText);
    }

//    if (sOutFirstMsg1!=NULL)
//    {
//        const CString sTemp(*sOutFirstMsg1);
//        if (bIsDepDialog)
//            sOutFirstMsg1->Format(_T("<font color=\"#6c6c6c\">%s</font><br/><a href=\"ebim-call-group://%lld\">%s</a>"),sTemp,m_callInfo->m_pCallInfo.m_sGroupCode,libEbc::ACP2UTF8("查看消息").c_str());
//        else
//            sOutFirstMsg1->Format(_T("<font color=\"#6c6c6c\">%s</font><br/><a href=\"ebim-call-account://%lld\">%s</a>"),sTemp,m_callInfo->m_pCallInfo.GetFromUserId(),libEbc::ACP2UTF8("查看消息").c_str());
    //    }
}

bool DialogChatBase::processFile(bool bReceive, const CCrFileInfo *fileInfo, EB_STATE_CODE nState)
{
    m_textBrowserMessage->addFileMsg( bReceive,fileInfo,nState );
    return true;
}

void DialogChatBase::updateFromInfo()
{
//    CEBString sDescription;
    if (m_nFromLineState==EB_LINE_STATE_UNKNOWN)
        m_nFromLineState = EB_LINE_STATE_ONLINE_NEW;
    if ( isGroupChat() ) {
        /// 群组聊天
        EB_GroupInfo pGroupInfo;
        if (theApp->m_ebum.EB_GetGroupInfo(m_callInfo->m_pCallInfo.m_sGroupCode,&pGroupInfo)) {
            m_sFromName = pGroupInfo.m_sGroupName.c_str();
            m_sFromDescription = pGroupInfo.m_sDescription.c_str();
            m_nGroupType = pGroupInfo.m_nGroupType;
            if (m_nGroupType==EB_GROUP_TYPE_TEMP)
                m_sFullName = m_sFromName;
            else
                m_sFullName = QString("%1(%2)").arg(m_sFromName).arg(pGroupInfo.m_sGroupCode);
        }
        if ( m_sFromName.isEmpty() ) {
            char lpszBuffer[24];
            sprintf(lpszBuffer,"%lld",m_callInfo->m_pCallInfo.m_sGroupCode);
            m_sFromName = lpszBuffer;
            m_sFullName = m_sFromName;
        }

        switch (m_nGroupType)
        {
        case EB_GROUP_TYPE_DEPARTMENT:
            {
                /// 部门及项目组，不能随意添加成员，和不能随意退出
//                m_btnAddUser.ShowWindow(SW_HIDE);
//                m_btnExitChat.ShowWindow(SW_HIDE);
            m_fromImage = QImage(":/img/imgdefaultdepartment.png");
            }break;
        case EB_GROUP_TYPE_PROJECT:
            {
                /// 部门及项目组，不能随意添加成员，和不能随意退出
//                m_btnAddUser.ShowWindow(SW_HIDE);
//                m_btnExitChat.ShowWindow(SW_HIDE);
            m_fromImage = QImage(":/img/imgdefaultproject.png");
            }break;
        case EB_GROUP_TYPE_GROUP:
            {
//                m_btnExitChat.SetToolTipText(_T("退出该群组"));
//                m_btnAddUser.SetToolTipText(_T("邀请好友加入群组"));
            m_fromImage = QImage(":/img/imgdefaultgroup.png");
            }break;
        case EB_GROUP_TYPE_TEMP:
            {
//                m_btnExitChat.SetToolTipText(_T("退出该讨论组"));
//                m_btnAddUser.SetToolTipText(_T("邀请好友加入讨论组"));
            m_fromImage = QImage(":/img/imgdefaulttempgroup.png");
            }break;
        default:
            break;
        }
    }
    else {
        /// 单聊会话
        if (m_callInfo->m_pFromAccountInfo.m_pFromCardInfo.m_nAccountType==EB_ACCOUNT_TYPE_VISITOR) {
            /// 游客会话
            m_fromImage = QImage(":/img/defaultvisitor.png");
            const QString visitorText = theLocales.getLocalText("chat-msg-text.visitor","Visitor");
            m_sFromName = QString("%1%2").arg(visitorText).arg(m_callInfo->m_pFromAccountInfo.GetAccount().c_str());
            m_sFullName = m_sFromName;
            m_sShortName = visitorText;
        }
        else {
            /// 普通用户聊天
            EB_MemberInfo pMemberInfo;
            if (theApp->m_ebum.EB_GetMemberInfoByMemberCode(&pMemberInfo,m_callInfo->m_pFromAccountInfo.m_pFromCardInfo.m_sMemberCode) ||
                theApp->m_ebum.EB_GetMemberInfoByUserId2(&pMemberInfo,m_callInfo->m_pFromAccountInfo.GetUserId())) {
                m_sFromName = pMemberInfo.m_sUserName.c_str();
                m_sFullName = QString("%1(%2)").arg(m_sFromName).arg(pMemberInfo.m_nMemberUserId);
                m_nFromLineState = pMemberInfo.m_nLineState;
                m_sFromDescription = pMemberInfo.m_sDescription.c_str();
                if ( QFileInfo::exists(pMemberInfo.m_sHeadResourceFile.c_str()) ) {
                    m_imageMd5 = pMemberInfo.m_sHeadMd5;
                    m_fromImage = QImage(pMemberInfo.m_sHeadResourceFile.c_str());
                 }
            }
            else if (m_callInfo->m_bOffLineUser) {
                m_nFromLineState = EB_LINE_STATE_OFFLINE;
            }

            if (m_sFromName.isEmpty()) {
                if (!m_callInfo->m_pFromAccountInfo.m_pFromCardInfo.m_sName.empty()) {
                    m_sFromName = m_callInfo->m_pFromAccountInfo.m_pFromCardInfo.m_sName.c_str();
                }
                else {
                    m_sFromName = m_callInfo->m_pFromAccountInfo.GetUserName().c_str();
                }
                m_sFullName = QString("%1(%2)").arg(m_sFromName).arg(m_callInfo->m_pFromAccountInfo.GetUserId());
            }

//            LPCTSTR lpszCaption = m_sFromName.c_str();
//            const size_t nLen = strlen(lpszCaption);
//            if (nLen<=4)
//                m_sShortName = m_sFromName.c_str();
//            else if (nLen>0 && lpszCaption[0]>0)			// *** 英文开头取前面
//            {
//                for (size_t i=0;i<nLen;i++)
//                {
//                    m_sShortName.AppendChar(lpszCaption[i]);
//                    if ((i+1)<nLen && lpszCaption[i]<0)		// 中文，需要取二个
//                    {
//                        m_sShortName.AppendChar(lpszCaption[++i]);
//                    }
//                    if (m_sShortName.GetLength()>=4)
//                    {
//                        break;
//                    }
//                }
//            }else	// 中文开头取后面
//            {
//                for (int i=nLen-1;i>=0;i--)
//                {
//                    m_sShortName.Insert(0,lpszCaption[i]);
//                    if ((i-1)>=0 && lpszCaption[i]<0)	// 中文，需要取二个
//                    {
//                        m_sShortName.Insert(0,lpszCaption[--i]);
//                    }
//                    if (m_sShortName.GetLength()>=4)
//                    {
//                        break;
//                    }
//                }
//            }
        }
    }

    if (m_fromImage.isNull()) {
        if (QFileInfo::exists(m_callInfo->m_pFromAccountInfo.m_pFromCardInfo.m_sHeadResourceFile.c_str())) {
            m_imageMd5 = m_callInfo->m_pFromAccountInfo.m_pFromCardInfo.m_sHeadMd5;
            m_fromImage = QImage(m_callInfo->m_pFromAccountInfo.m_pFromCardInfo.m_sHeadResourceFile.c_str());
        }
        else {
            m_fromImage = QImage(":/img/defaultmember.png");
        }
    }

    ui->labelName->setText(m_sFullName);
    ui->labelDescription->setText(m_sFromDescription);
}
