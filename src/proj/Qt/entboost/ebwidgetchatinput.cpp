#include "ebwidgetchatinput.h"
#include "ebtextedit.h"
#include "iconhelper.h"

const QSize const_button_emotion_size(24,20);

EbWidgetChatInput::EbWidgetChatInput(const EbcCallInfo::pointer& pCallInfo,QWidget *parent)
    : QWidget(parent)
    , m_textEditInput(NULL)
    , m_pushButtonEmotion(NULL), m_pushButtonMsgRecord(NULL)
    , m_pushButtonClose(NULL), m_pushButtonSend(NULL)
    , m_callInfo(pCallInfo)
{
    assert(m_callInfo.get()!=0);
    m_textEditInput = new EbTextEdit(this);
    m_textEditInput->setStyleSheet("border: none;");
    connect( m_textEditInput,SIGNAL(dropSendFile(QString)),this,SLOT(onDropSendFileTextEdit(QString)) );

    /// for test
//    m_menuTextEdit = m_textEditInput->createStandardContextMenu();
//    m_menuTextEdit->addSeparator();
//    const QString selectText = theLocales.getLocalText("color-skin.select-color.text","选择色调");
//    m_menuTextEdit->addAction( QIcon(QStringLiteral(":/res/color_select.bmp")), selectText );

    /// 表情 & 消息记录
    m_pushButtonEmotion = new QPushButton(this);
    m_pushButtonMsgRecord = new QPushButton(this);
    m_pushButtonEmotion->resize(const_button_emotion_size);
    m_pushButtonMsgRecord->resize(const_button_emotion_size);
    m_pushButtonEmotion->setToolTip( theLocales.getLocalText("chat-input-widget.button-emotion.tooltip", "emotion") );
    m_pushButtonMsgRecord->setToolTip( theLocales.getLocalText("chat-input-widget.button-msg-record.tooltip", "msg record") );
    IconHelper::Instance()->SetIcon(m_pushButtonEmotion,QChar(0xf118),12);
    IconHelper::Instance()->SetIcon(m_pushButtonMsgRecord,QChar(0xf298),12);
    m_pushButtonEmotion->setObjectName("DialogChatButton");
    m_pushButtonMsgRecord->setObjectName("DialogChatButton");
    connect( m_pushButtonEmotion,SIGNAL(clicked()),this,SLOT(onClickedPushButtonEmotion()) );
    connect( m_pushButtonMsgRecord,SIGNAL(clicked()),this,SLOT(onClickedPushButtonMsgRecord()) );

    /// 关闭/发送按钮
    m_pushButtonClose = new QPushButton(this);
    m_pushButtonClose->setObjectName("SendButton");
    m_pushButtonClose->setText( theLocales.getLocalText("chat-input-widget.button-close.text", "close") );
    m_pushButtonClose->setToolTip( theLocales.getLocalText("chat-input-widget.button-close.tooltip", "close") );
    connect( m_pushButtonClose,SIGNAL(clicked()),this,SLOT(onClickedPushButtonClose()) );
    m_pushButtonSend = new QPushButton(this);
    m_pushButtonSend->setAutoDefault(true);
    m_pushButtonSend->setObjectName("SendButton");
    m_pushButtonSend->setText( theLocales.getLocalText("chat-input-widget.button-send1.text", "send") );
    m_pushButtonSend->setToolTip( theLocales.getLocalText("chat-input-widget.button-send1.tooltip", "send") );
    connect( m_pushButtonSend,SIGNAL(clicked()),this,SLOT(onClickedPushButtonSend()) );

}

void EbWidgetChatInput::setFocusInput()
{
    if (m_textEditInput!=0) {
        m_textEditInput->setFocus();
    }
}

void EbWidgetChatInput::setCallInfo(const EbcCallInfo::pointer &pCallInfo)
{
    m_callInfo = pCallInfo;
    assert(m_callInfo.get()!=0);
}

void EbWidgetChatInput::onUserExit(eb::bigint nFromAccount)
{
    if (m_pushButtonSend!=0) {
        m_pushButtonSend->setEnabled(false);
    }
}

void EbWidgetChatInput::onReceiveRich()
{
    if (m_pushButtonSend!=0 && !m_pushButtonSend->isEnabled()) {
        if (theApp->m_ebum.EB_IsLogonVisitor() ||
                m_callInfo->m_pFromAccountInfo.m_pFromCardInfo.m_nAccountType == EB_ACCOUNT_TYPE_VISITOR) {	// 游客不能被动邀请
            m_pushButtonSend->setEnabled(true);
        }
    }
}

void EbWidgetChatInput::onDropSendFileTextEdit(const QString &filePath)
{
    /// 支持中文路径，文件名
//    const QByteArray filePathByteArray = filePath.toLocal8Bit();
//    theApp->m_ebum.EB_SendFile( m_callInfo->callId(), filePathByteArray.constData() );
//    const std::string fileName = filePath.toStdString();
//    const bool fileExist1 = QFileInfo::exists(filePath);
//    const bool fileExist2 = QFileInfo::exists( QString::fromStdString(fileName) );
//    FILE * file = fopen( fileName1.constData(), "r");
//    if (file!=0) {
//        fclose(file);
//    }
    theApp->m_ebum.EB_SendFile( m_callInfo->callId(), filePath.toStdString().c_str() );
}

void EbWidgetChatInput::onClickedPushButtonEmotion()
{
    const QPoint pt = cursor().pos();
    theApp->showDialogEmotionSelect( pt,this );
}

void EbWidgetChatInput::onClickedPushButtonMsgRecord()
{

}

void EbWidgetChatInput::onClickedPushButtonClose()
{
    emit clickedClose();
}

void EbWidgetChatInput::onClickedPushButtonSend()
{
    EB_ChatRoomRichMsg pRichMsg;
    if ( !m_textEditInput->formatInputMsg(&pRichMsg)) {
        return;
    }
    const std::vector<EB_ChatRoomMsgItem*>& pRichMsgList = pRichMsg.GetList();
    if (pRichMsgList.empty()) {
        this->m_textEditInput->setFocus();
        return;
    }
//    const int nCurSel = m_comboSendTo.GetCurSel();
//    eb::bigint nToUserId = 0;
//    CString sToAccount;
//    const bool bIsDepDialog = m_pCallInfo.m_sGroupCode>0;
//    if (nCurSel > 0)
//    {
//        CTreeItemInfo * pTreeItemInfo = (CTreeItemInfo*)m_comboSendTo.GetItemData(nCurSel);
//        if (pTreeItemInfo==NULL) return;
//        sToAccount = pTreeItemInfo->m_sAccount.c_str();
//        nToUserId = pTreeItemInfo->m_nUserId;
//        UpdateData();
//    }

    eb::bigint nToUserId = 0;
    const bool bPrivate = false;
    if ( theApp->m_ebum.EB_SendRich(m_callInfo->m_pCallInfo.GetCallId(),&pRichMsg,nToUserId,bPrivate)==0 ) {
        m_textEditInput->clear();
    }
    m_textEditInput->setFocus();
}

void EbWidgetChatInput::onSelectedResourceEmotion(const QIcon &icon,mycp::bigint resourceId,
                                                  const QString& resourceCmInfo,const QString & resourceFilePath)
{
    QImage image;
    if (!resourceFilePath.isEmpty() && QFileInfo::exists(resourceFilePath)) {
        ///
        image = QImage(resourceFilePath);
    }
    if (image.isNull()) {
        image = icon.pixmap(24,24).toImage();
    }
    /// 下面的addResource代码非常重要, 内存增加很少
    m_textEditInput->document()->addResource(QTextDocument::ImageResource, QUrl(resourceCmInfo), image);
    m_textEditInput->textCursor().insertImage(resourceCmInfo);
}

void EbWidgetChatInput::resizeEvent(QResizeEvent * e)
{
    int x = 0;
    int y = 0;
    const QSize const_send_button_size(68,26);
    const int const_bottom_height = const_send_button_size.height()+10; // 下边“发送/关闭”工具条高度
    if (m_textEditInput!=NULL) {
        m_textEditInput->setGeometry( x,y,width(),height()-const_bottom_height );
    }
    x = 6;
    y = height()-const_bottom_height+1;
    if (m_pushButtonEmotion!=NULL) {
        m_pushButtonEmotion->move(x,y);
    }
    x += const_button_emotion_size.width();
    if (m_pushButtonMsgRecord!=NULL) {
        m_pushButtonMsgRecord->move(x,y);
    }
    x = width()-const_send_button_size.width()-8;
    if (m_pushButtonSend!=NULL) {
        m_pushButtonSend->setGeometry( x,y,const_send_button_size.width(),const_send_button_size.height());
    }
    x -= (const_send_button_size.width()+8);
    if (m_pushButtonClose!=NULL) {
        m_pushButtonClose->setGeometry( x,y,const_send_button_size.width(),const_send_button_size.height());
    }

    QWidget::resizeEvent(e);
}

void EbWidgetChatInput::contextMenuEvent(QContextMenuEvent *e)
{
//    QPoint pos; //获取按键菜单的坐标
//    // int x = pos.x();
//    int y = pos.y();
//    // pos.setX(x + this->geometry().width()/2);
//    pos.setY(y+m_pushButtonSend->geometry().height());
//    m_menuTextEdit->exec(m_pushButtonSend->mapToGlobal(pos));
}
