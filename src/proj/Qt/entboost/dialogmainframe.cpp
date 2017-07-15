#include "dialogmainframe.h"
#include "ui_dialogmainframe.h"
#include "iconhelper.h"
#include "ebclientapp.h"
#include "ebmessagebox.h"
#include "dialogchatbase.h"
#include "dialogframelist.h"
#include "dialogmessagetip.h"
#include "dialogmycenter.h"
#include "dialogworkframe.h"
#include "ebwidgetappbar.h"
#include <QSound>

DialogMainFrame::DialogMainFrame(QWidget *parent) :
    EbDialogBase(parent),
    ui(new Ui::DialogMainFrame)
  , m_labelUserImage(0)
  , m_labelLinState(0)
  , m_menuSetting(0)
  , m_menuLineState(0)
  , m_menuContext(0)//, m_menuSysTray(0)
  , m_pDlgMyCenter(0)
  , m_pDlgFrameList(0)
  , m_pDlgMsgTip(0)
  , m_widgetMyGroup(0)
#ifdef USES_EVENT_DATE_TIMER
  , m_checkEventData(0)
#endif
  , m_timerOneSecond(0)
  , m_trayIcon(0)

{
    m_widgetMainAppBar = new EbWidgetAppBar(EB_FUNC_LOCATION_MAINFRAME_BTN2,this);
    connect( m_widgetMainAppBar,SIGNAL(clickedSubApp(EB_SubscribeFuncInfo)),this,SLOT(onClickedSubApp(EB_SubscribeFuncInfo)) );
    theApp->m_ebum.EB_SetMsgReceiver(this);
    QTimer::singleShot( 1, this, SLOT(processDatas()) );
    ui->setupUi(this);
    /// 设置过滤 ESC/Enter 按键
    this->setFilterEscapeKey(true);
    this->setFilterEnterKey(true);
    /// 开启双击事件最大化窗口
    setDoubleClickMaxEnable(true);

    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    BuildHeadRect();

    /// 设置初始大小
    int const_main_frame_width = 0;
    int const_main_frame_height = 0;
    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_OFFICE) {
        const_main_frame_width = theLocales.getLocalInt("main-frame.window-size.0.width", 1180);
        const_main_frame_height = theLocales.getLocalInt("main-frame.window-size.0.height", 668);
    }
    else {
        const_main_frame_width = theLocales.getLocalInt("main-frame.window1-size.1.width", 288);
        const_main_frame_height = theLocales.getLocalInt("main-frame.window1-size.1.height", 568);
    }
    this->resize( const_main_frame_width,const_main_frame_height );
    /// 去掉标题栏
    this->setWindowFlags( Qt::Window|Qt::FramelessWindowHint|Qt::WindowSystemMenuHint|Qt::WindowMinMaxButtonsHint);
    /// 设置位置，显示在上面
    this->showTitleBackground(190);


    /// 产品名称 & 登录LOGO
    this->showTitleLogoText( theApp->productName(),const_common_title_font_size );

    IconHelper::Instance()->SetIcon(ui->pushButtonSetting,QChar(0xf0d7),10);
    const int x = const_main_frame_width-this->getSysButtonWidth()-const_sys_button_size.width();
    ui->pushButtonSetting->setGeometry( x,0,const_sys_button_size.width(),const_sys_button_size.height() );
    ui->pushButtonSetting->setObjectName("SysTransButton");
    ui->pushButtonSetting->raise();

    m_labelUserImage = new EbLabel(this);
    m_labelUserImage->setCursor( QCursor(Qt::PointingHandCursor) );
    connect( m_labelUserImage,SIGNAL(clicked()),this,SLOT(onClickedLabelUserImage()) );

    /// 必须放在后面
    updateLocaleInfo();
    /// 显示头像
    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_OFFICE) {
    }
    else {
        const tstring sHeadFile = theApp->m_ebum.EB_GetMyDefaultMemberHeadFile();
        const tstring sUserName = theApp->m_ebum.EB_GetUserName();
        const tstring sOutDesc = theApp->m_ebum.EB_GetDescription();
        /// 显示用户头像
        m_labelUserImage->setGeometry(m_rectHead);
        if ( theApp->m_ebum.EB_IsLogonVisitor() ) {
            m_labelUserImage->setPixmap( QPixmap(":/img/defaultvisitor.png").scaled(m_rectHead.width(),m_rectHead.height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation) );
        }
        else {
            const QString userHeadFile = QString::fromStdString(sHeadFile.string());
            if ( !userHeadFile.isEmpty() && QFile::exists(userHeadFile)) {
                m_labelUserImage->setPixmap( QPixmap(userHeadFile).scaled(m_rectHead.width(),m_rectHead.height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation) );
            }
            else {
                m_labelUserImage->setPixmap( QPixmap(":/img/defaultmember.png").scaled(m_rectHead.width(),m_rectHead.height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation) );
            }
        }
        /// 显示在线状态按钮
        const QSize const_line_state_image_size(17,17);
        int x = m_rectHead.right() + 4;
        int y = m_rectHead.top();
        ui->labelLineState->hide();
        m_labelLinState = new EbLabel(this);
        m_labelLinState->setGeometry( x,y,const_line_state_image_size.width(),const_line_state_image_size.height() );
        m_labelLinState->setObjectName("LineState");
        updateLineState();
         m_labelLinState->raise();
        connect( m_labelLinState,SIGNAL(clicked()),this,SLOT(onClickedLineState()) );
        /// 显示用户名称 & 个人签名
        x += (const_line_state_image_size.width()+2);
        int len = const_main_frame_width-x-2;
        ui->labelUserName->setGeometry( x,y-1,len, 18);
        ui->labelUserName->setText(QString::fromStdString(sUserName));
        ui->labelUserName->setObjectName("MainUserName");
        x = m_rectHead.right() + 4;
        len = const_main_frame_width-x-2;
        y += 20;
        ui->lineEditUserDescription->installEventFilter(this);
        ui->lineEditUserDescription->setGeometry( x,y,len,20 );
        ui->lineEditUserDescription->setText(QString::fromStdString(sOutDesc));
        ui->lineEditUserDescription->setObjectName("UserDescription");
//        setStyleSheet("QLineEdit{border:1px solid #C3C3C3;background:rgba(255,255,255,0);color:#19649F;margin-left:8px;margin-right:10px;}"
//                      "QLineEdit:hover{border: 1px solid #10B9D3}"
//                      "QLineEdit:enabled{color:#19649F}"
//                      "QLineEdit:disabled{color:#666666}");
        /// 显示“个人中心/文件管理/个人云盘/集成应用”四个按钮
        IconHelper::Instance()->SetIcon(ui->pushButtonMyCenter,QChar(0xf007),11);
//        IconHelper::Instance()->SetIcon(ui->pushButtonMyCenter,QChar(0xf2c0),11);
        IconHelper::Instance()->SetIcon(ui->pushButtonFileManager,QChar(0xf019),11);
        IconHelper::Instance()->SetIcon(ui->pushButtonMyShare,QChar(0xf0c2),11);
        IconHelper::Instance()->SetIcon(ui->pushButtonApps,QChar(0xf009),11);
        ui->pushButtonMyCenter->setObjectName("MainButton");
        ui->pushButtonFileManager->setObjectName("MainButton");
        ui->pushButtonMyShare->setObjectName("MainButton");
        ui->pushButtonApps->setObjectName("MainButton");
        y += 20;
        const int const_main_frame_btn_size = 24;
        ui->pushButtonMyCenter->setGeometry( x,y,const_main_frame_btn_size,const_main_frame_btn_size );
        x += const_main_frame_btn_size;
        ui->pushButtonFileManager->setGeometry( x,y,const_main_frame_btn_size,const_main_frame_btn_size );
        x += const_main_frame_btn_size;
        ui->pushButtonMyShare->setGeometry( x,y,const_main_frame_btn_size,const_main_frame_btn_size );
        x += const_main_frame_btn_size;
        ui->pushButtonApps->setGeometry( x,y,const_main_frame_btn_size,const_main_frame_btn_size );
        /// 显示“搜索框”和搜索ICON
        ui->lineEditSearch->setObjectName("MainSearch");
        x = 0;
        y += 30;    /// 28
        ui->lineEditSearch->raise();
        ui->lineEditSearch->setGeometry( 0,y,const_main_frame_width,24 );
        /// 搜索用户、浏览记录、集成应用(/)
        IconHelper::Instance()->SetIcon(ui->labelSearchIcon,QChar(0xf002),9);
        ui->labelSearchIcon->raise();
        ui->labelSearchIcon->setGeometry( 4,y+2,18,18 );
        ui->labelSearchIcon->setObjectName("SearchIcon");

        /// 显示“我的部门/联系人/最近会话/公司组织结构/集成应用”
        IconHelper::Instance()->SetIcon(ui->pushButtonMyGroup,QChar(0xf0c0),14);
        IconHelper::Instance()->SetIcon(ui->pushButtonMyContact,QChar(0xf007),14);
        IconHelper::Instance()->SetIcon(ui->pushButtonMySession,QChar(0xf075),14);
        IconHelper::Instance()->SetIcon(ui->pushButtonMyEnterprise,QChar(0xf19c),14);
        IconHelper::Instance()->SetIcon(ui->pushButtonMyApp,QChar(0xf00a),14);
        ui->pushButtonMyGroup->setCheckable(true);
        ui->pushButtonMyContact->setCheckable(true);
        ui->pushButtonMySession->setCheckable(true);
        ui->pushButtonMyEnterprise->setCheckable(true);
        ui->pushButtonMyApp->setCheckable(true);
        onClickedPushButtonMyGroup();
        x = 1;
        y += 24;
        // 57*5=285+2=287
        const int const_my_button_width = MIN(57,(const_main_frame_width-x*2)/5);
        const int const_my_button_height = 30;
        ui->pushButtonMyGroup->setGeometry( x,y,const_my_button_width,const_my_button_height);
        x += const_my_button_width;
        ui->pushButtonMyContact->setGeometry( x,y,const_my_button_width,const_my_button_height);
        x += const_my_button_width;
        ui->pushButtonMySession->setGeometry( x,y,const_my_button_width,const_my_button_height);
        x += const_my_button_width;
        ui->pushButtonMyEnterprise->setGeometry( x,y,const_my_button_width,const_my_button_height);
        x += const_my_button_width;
        ui->pushButtonMyApp->setGeometry( x,y,const_my_button_width,const_my_button_height);
        ui->pushButtonMyGroup->setObjectName("MainCheckButton");
        ui->pushButtonMyContact->setObjectName("MainCheckButton");
        ui->pushButtonMySession->setObjectName("MainCheckButton");
        ui->pushButtonMyEnterprise->setObjectName("MainCheckButton");
        ui->pushButtonMyApp->setObjectName("MainCheckButton");
    }
    theApp->m_ebum.EB_SetMsgReceiver(this);

    /// 消息提醒界面
    m_pDlgMsgTip = new DialogMessageTip;
    m_pDlgMsgTip->setVisible(false);
    m_pDlgMsgTip->setModal(false);
    m_pDlgMsgTip->setWindowModality(Qt::WindowModal);

    onLogonSuccess(0);

    refreshSkin();

    connect( ui->pushButtonSetting,SIGNAL(clicked()),this,SLOT(onClickedPushButtonSetting()) );

//    connect( ui->pushButtonFileManager,SIGNAL(clicked()),this,SLOT(onClickedPushButtonFileManager()) );

    connect( ui->pushButtonMyGroup,SIGNAL(clicked()),this,SLOT(onClickedPushButtonMyGroup()) );
    connect( ui->pushButtonMyContact,SIGNAL(clicked()),this,SLOT(onClickedPushButtonMyContact()) );
    connect( ui->pushButtonMySession,SIGNAL(clicked()),this,SLOT(onClickedPushButtonMySession()) );
    connect( ui->pushButtonMyEnterprise,SIGNAL(clicked()),this,SLOT(onClickedPushButtonMyEnterprise()) );
    connect( ui->pushButtonMyApp,SIGNAL(clicked()),this,SLOT(onClickedPushButtonMyApp()) );
#ifdef USES_EVENT_DATE_TIMER
    m_checkEventData = this->startTimer(10);
#endif
    m_timerOneSecond = this->startTimer(1000);
}

void DialogMainFrame::refreshSkin(void)
{
    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
        const QRect& rectSearchIcon = ui->lineEditSearch->geometry();
        this->showTitleBackground(rectSearchIcon.bottom());
    }
    else {
        this->showTitleBackground( theLocales.titleBackgroundHeight() );
    }

}

DialogMainFrame::~DialogMainFrame()
{
#ifdef USES_EVENT_DATE_TIMER
    m_eventList.clear(true,false);
    if (m_checkEventData!=0) {
        killTimer(m_checkEventData);
        m_checkEventData = 0;
    }
#endif
    if (m_timerOneSecond!=0) {
        killTimer(m_timerOneSecond);
        m_timerOneSecond = 0;
    }
    if (m_pDlgMyCenter==0) {
        delete m_pDlgMyCenter;
        m_pDlgMyCenter = 0;
    }
    if (m_pDlgFrameList!=0) {
        delete m_pDlgFrameList;
        m_pDlgFrameList = 0;
    }
    if (m_pDlgMsgTip!=0) {
        delete m_pDlgMsgTip;
        m_pDlgMsgTip = 0;
    }
//    if (m_menuSysTray!=0) {
//        delete m_menuSysTray;
//        m_menuSysTray = 0;
//    }
    delete ui;
}

void DialogMainFrame::updateLocaleInfo()
{
    /// 显示右上角关闭按钮
    this->showPushButtonSysClose( theLocales.getLocalText("base-dialog.close-button.tooltip","Close") );
    this->showPushButtonSysMax( theLocales.getLocalText("base-dialog.maximize-button.tooltip","Maximize") );
    this->showPushButtonSysMin( theLocales.getLocalText("base-dialog.minimize-button.tooltip","Minimize") );

    m_labelUserImage->setToolTip( theLocales.getLocalText("main-frame.label-user-head.tooltip","My Center") );
    ui->pushButtonSetting->setToolTip(theLocales.getLocalText("base-dialog.setting-button.tooltip","Setting"));
    ui->lineEditUserDescription->setPlaceholderText( theLocales.getLocalText("main-frame.edit-user-description.bg-text","") );
    ui->lineEditUserDescription->setToolTip( theLocales.getLocalText("main-frame.edit-user-description.tooltip","") );
    ui->pushButtonMyCenter->setToolTip( theLocales.getLocalText("main-frame.up-button-my-center.tooltip","my center") );
    ui->pushButtonFileManager->setToolTip( theLocales.getLocalText("main-frame.up-button-file-manager.tooltip","file manager") );
    ui->pushButtonMyShare->setToolTip( theLocales.getLocalText("main-frame.up-button-my-share.tooltip","my share") );
    ui->pushButtonApps->setToolTip( theLocales.getLocalText("main-frame.up-button-apps.tooltip","apps") );
    ui->lineEditSearch->setPlaceholderText( theLocales.getLocalText("main-frame.edit-search.bg-text","search") );
    ui->lineEditSearch->setToolTip( theLocales.getLocalText("main-frame.edit-search.tooltip","") );
    ui->pushButtonMyGroup->setToolTip( theLocales.getLocalText("main-frame.mid-button-my-group.tooltip","group") );
    ui->pushButtonMyContact->setToolTip( theLocales.getLocalText("main-frame.mid-button-my-contact.tooltip","contact") );
    ui->pushButtonMySession->setToolTip( theLocales.getLocalText("main-frame.mid-button-my-session.tooltip","session") );
    ui->pushButtonMyEnterprise->setToolTip( theLocales.getLocalText("main-frame.mid-button-my-ent.tooltip","enterprise") );
    ui->pushButtonMyApp->setToolTip( theLocales.getLocalText("main-frame.mid-button-my-app.tooltip","app") );

}

void DialogMainFrame::resizeEvent(QResizeEvent *e)
{
    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
        const QRect& rectSearchIcon = ui->labelSearchIcon->geometry();
        ui->lineEditSearch->setGeometry( 0,rectSearchIcon.top()-2,e->size().width(),22 );
    }else {

    }

    if (ui->pushButtonSetting!=NULL) {
        const int x = e->size().width()-this->getSysButtonWidth()-const_sys_button_size.width();
        ui->pushButtonSetting->setGeometry( x,0,const_sys_button_size.width(),const_sys_button_size.height() );
    }

    const int const_bottom_app_bar_height = 29;
    ///
    if (m_widgetMyGroup!=0) {
        const QRect & buttonRect = ui->pushButtonMyGroup->geometry();
        const int y = buttonRect.bottom()+1;
        const int h = height()-y-const_bottom_app_bar_height;
        m_widgetMyGroup->setGeometry( 1,y,width()-2,h );
    }

    m_widgetMainAppBar->setGeometry( 1,height()-const_bottom_app_bar_height,width(),const_bottom_app_bar_height-1);

    EbDialogBase::resizeEvent(e);
}

//void DialogMainFrame::closeEvent(QCloseEvent *event)
//{
//    /// 没有到这里来
//    if (m_trayIcon!=0 && m_trayIcon->isVisible()) {
//        m_trayIcon->showMessage("SystemTrayIcon","hide trayIcon",QSystemTrayIcon::Information,5000);
////        hide();     //最小化
//        event->ignore();
//    }
//    else {
//        event->accept();
//    }
////    EbDialogBase::closeEvent(event);
//}
void DialogMainFrame::customEvent(QEvent *e)
{
    if (e->type()>=QEvent::User) {
#ifdef USES_EVENT_DATE_TIMER
        m_eventList.add(e);
        bool result = false;
        while (!m_eventMap.find(e,result)) {
            QCoreApplication::processEvents();
            QThread::msleep (3);
        }
        return;
#else   /// USES_EVENT_DATE_TIMER

        checkEventData(e);

#endif /// USES_EVENT_DATE_TIMER
    }
}

void DialogMainFrame::onContactDelete(QEvent *e)
{

}

void DialogMainFrame::onContactInfo(QEvent *e)
{

}
void DialogMainFrame::onAcceptAddContact(QEvent *e)
{

}

void DialogMainFrame::onRejectAddContact(QEvent *e)
{

}

void DialogMainFrame::onRequestAddContact(QEvent *e)
{

}

void DialogMainFrame::onEditInfoResponse(QEvent *e)
{
    const EB_Event * pEvent = (EB_Event*)e;
    const EB_STATE_CODE nState = (EB_STATE_CODE)pEvent->GetEventParameter();
    if (EB_STATE_APPID_KEY_ERROR==nState || EB_STATE_APP_ONLINE_KEY_TIMEOUT==nState) {
        return;
    }
    /// nFlag 1=修改密码返回
    const int nEditInfoFlag = (int)pEvent->GetEventBigParameter();
    if (nState==EB_STATE_ACC_PWD_ERROR) {
        /// 密码错误：\r\n请重新输入！
        QString text = theLocales.getLocalText("on-editinfo-response.acc-pwd-error.text","Password Error");
        text.replace( "[STATE_CODE]", QString::number((int)nState) );
        const QString title = theLocales.getLocalText("on-editinfo-response.acc-pwd-error.title","");
        EbMessageBox::doShow( NULL, title, QChar::Null, text, EbMessageBox::IMAGE_INFORMATION,default_warning_auto_close );
        return;
    }
    else if (nEditInfoFlag==1) {	/// *** 修改密码
        if (nState==EB_STATE_OK) {
            char sSql[256];
            sprintf(sSql,"update user_login_record_t set password='' where user_id=%lld",theApp->logonUserId());
            theApp->m_sqliteEbc->execute(sSql);
            /// 密码修改成功：\r\n请重新登录。
            const QString text = theLocales.getLocalText("on-editinfo-response.modify-password-ok.text","Edit Password Ok");
            QString title = theLocales.getLocalText("on-editinfo-response.modify-password-ok.title","");
            if (title.isEmpty())
                title = theApp->productName();
            EbMessageBox::doExec( NULL, title, QChar::Null, text, EbMessageBox::IMAGE_INFORMATION,0,QMessageBox::Ok );
            this->accept();
//            OnLogout();
            return;
        }
        else {
            /// 密码修改失败，请重试！<br>错误代码: [STATE_CODE]
            QString text = theLocales.getLocalText("on-editinfo-response.modify-password-error.text","Edit Password Error");
            text.replace( "[STATE_CODE]", QString::number((int)nState) );
            const QString title = theLocales.getLocalText("on-editinfo-response.modify-password-error.title","");
            EbMessageBox::doShow( NULL, title, QChar::Null, text, EbMessageBox::IMAGE_INFORMATION,default_warning_auto_close );
        }
    }
    else if (nState==EB_STATE_OK) {
        /// 个人资料修改成功！
        const QString text = theLocales.getLocalText("on-editinfo-response.state-ok.text","Edit Info Ok");
        const QString title = theLocales.getLocalText("on-editinfo-response.state-ok.title","");
        EbMessageBox::doShow( NULL, title, QChar::Null, text, EbMessageBox::IMAGE_INFORMATION,default_warning_auto_close );
    }
    else {
        /// 个人资料修改失败，请重试！<br>错误代码: %d
        QString text = theLocales.getLocalText("on-editinfo-response.other-error.text","Edit Info Error");
        text.replace( "[STATE_CODE]", QString::number((int)nState) );
        const QString title = theLocales.getLocalText("on-editinfo-response.other-error.title","");
        EbMessageBox::doShow( NULL, title, QChar::Null, text, EbMessageBox::IMAGE_WARNING,default_warning_auto_close );
    }
    ui->lineEditUserDescription->setText( theApp->m_ebum.EB_GetDescription().c_str() );
    changeTrayTooltip();
}

void DialogMainFrame::onMemberEditResponse(QEvent *e)
{
    const EB_MemberInfo* pMemberInfo = (const EB_MemberInfo*)e;
    const EB_STATE_CODE stateCode = (EB_STATE_CODE)pMemberInfo->GetEventParameter();
    QString text;
    EbMessageBox::IMAGE_TYPE imageType = EbMessageBox::IMAGE_WARNING;
    switch (stateCode) {
    case EB_STATE_OK: {
        /// 操作成功
        const bool bNewMemberInfo = pMemberInfo->GetEventBigParameter()==1?true:false;
        imageType = EbMessageBox::IMAGE_INFORMATION;
        if (bNewMemberInfo)
            text = theLocales.getLocalText("on-member-edit-response.new-state-ok.text","");
        else
            text = theLocales.getLocalText("on-member-edit-response.edit-state-ok.text","");
        break;
    }
    case EB_STATE_OAUTH_FORWARD: {
        /// 邀请成员成功：\r\n等待对方通过验证！
        imageType = EbMessageBox::IMAGE_INFORMATION;
        text = theLocales.getLocalText("on-member-edit-response.oauth-forward.text","Oauth Forward");
        break;
    }
    case EB_STATE_NOT_AUTH_ERROR: {
        /// 没有权限：\r\n请联系管理员！
        text = theLocales.getLocalText("on-member-edit-response.not-auth-error.text","Not Auth Error");
//        if (libEbc::IsFullNumber(pMemberInfo->m_sMemberAccount.c_str(),pMemberInfo->m_sMemberAccount.size()))
//            CDlgMessageBox::EbMessageBox(this,_T(""),_T("不支持全数字帐号注册：\r\n请更换帐号后重试！"),CDlgMessageBox::IMAGE_ERROR,5);
//        else
//            CDlgMessageBox::EbMessageBox(this,_T(""),_T("没有权限：\r\n请联系管理员！"),CDlgMessageBox::IMAGE_ERROR,5);
        break;
    }
    case EB_STATE_ACCOUNT_ALREADY_EXIST:
        /// 帐号已经存在：\r\n请重新输入！
        text = theLocales.getLocalText("on-member-edit-response.account-already-exist.text","Account Already Exist");
        break;
    case EB_STATE_ANOTHER_ENT_ACCOUNT:
        /// 其他企业员工帐号：<br>请重新输入！
        text = theLocales.getLocalText("on-member-edit-response.another-ent-account.text","Another Enterprise Account");
        break;
    default: {
        /// 操作失败,请重试或联系公司客服：<br>错误代码: [STATE_CODE]
        text = theLocales.getLocalText("on-member-edit-response.other-error.text","Other Error");
        break;
    }
    }
    if (!text.isEmpty()) {
        text.replace("[STATE_CODE]",QString::number((int)stateCode));
        EbMessageBox::doShow( NULL, "", QChar::Null, text, imageType,default_warning_auto_close );
    }
}

void DialogMainFrame::onMemberDelete(QEvent *e)
{
    /// ***统一在 EB_WM_REMOVE_GROUP 消息处理
}

void DialogMainFrame::onMemberInfo(QEvent *e)
{
    const EB_MemberInfo * pMemberInfo = (EB_MemberInfo*)e;
    const bool bIsMyDefaultMember = (pMemberInfo->GetEventParameter()==1)?true:false;
    const bool bIsMyGroupMember = (pMemberInfo->GetEventBigParameter()==1)?true:false;
//    if (m_pDlgMyEnterprise != NULL)
//        m_pDlgMyEnterprise->EmployeeInfo(pMemberInfo);
    if (this->m_widgetMyGroup!=0 && bIsMyGroupMember) {
        this->m_widgetMyGroup->onMemberInfo(pMemberInfo,false);
    }
    if (bIsMyDefaultMember) {
        QString sSettingEnterprise;
        if (!theApp->productName().isEmpty())
            sSettingEnterprise = theApp->productName();
        else
            sSettingEnterprise = theApp->m_setting.GetEnterprise().c_str();
        QString sWindowText;
        tstring sEnterpriseName;
        if (theApp->m_ebum.EB_GetEnterpriseName(sEnterpriseName)) {
            sWindowText = QString("%1-%2(%3) %4").arg(sSettingEnterprise).arg(pMemberInfo->m_sUserName.c_str())
                    .arg(pMemberInfo->m_sMemberAccount.c_str()).arg(sEnterpriseName.c_str());
        }
        else {
            sWindowText = QString("%1-%2(%3)").arg(sSettingEnterprise).arg(pMemberInfo->m_sUserName.c_str())
                    .arg(pMemberInfo->m_sMemberAccount.c_str());
        }
        this->setWindowTitle(sWindowText);
    }
    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
        /// **跑下面
    }
    else if (!theApp->isHideMainFrame()) {
//        CFrameWndInfoProxy::OnUserEmpInfo(pMemberInfo,true);
//        return 0;
    }
    if (m_pDlgFrameList!=0)
        m_pDlgFrameList->onMemberInfo(pMemberInfo,true);

}
void DialogMainFrame::onRejectAdd2Group(QEvent *e)
{

}

void DialogMainFrame::onInviteAdd2Group(QEvent *e)
{

}

void DialogMainFrame::onRequestAdd2Group(QEvent *e)
{

}

void DialogMainFrame::onExitGroup(QEvent *e)
{
    const EB_GroupInfo* pGroupInfo = (const EB_GroupInfo*)e;
    const EB_MemberInfo* pMemberInfo = (const EB_MemberInfo*)pGroupInfo->GetEventData();
    const eb::bigint sExitUserId(pMemberInfo->m_nMemberUserId);

    /// 删除本地群组聊天记录
    theApp->deleteDbRecord(pGroupInfo->m_sGroupCode,false);
    /// 找到现在会话，移除用户数据
    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    if ( sExitUserId == theApp->logonUserId() )
    {
        theApp->m_pCallList.remove(pGroupInfo->m_sGroupCode);
        if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT && m_pDlgFrameList!=0) {
            m_pDlgFrameList->onRemoveGroup(pGroupInfo->m_sGroupCode);
        }
        else if (!theApp->isHideMainFrame()) {
//            CFrameWndInfoProxy::OnRemoveGroup(pGroupInfo->m_sGroupCode);
        }
        else if (m_pDlgFrameList!=0) {
            m_pDlgFrameList->onRemoveGroup(pGroupInfo->m_sGroupCode);
        }

        /// 删除会话
        getDialogChatBase(pGroupInfo->m_sGroupCode, true);

//        if (m_pDlgMyEnterprise != NULL)
//            m_pDlgMyEnterprise->DeleteEmployeeInfo(pGroupInfo,pMemberInfo->m_sMemberCode);
        if (m_widgetMyGroup!=0) {
            m_widgetMyGroup->onRemoveGroup(pGroupInfo);
//            m_widgetMyGroup->deleteGroupInfo(pGroupInfo);
        }
    }
    else {
        if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT && m_pDlgFrameList!=0) {
            m_pDlgFrameList->onRemoveMember( pGroupInfo->m_sGroupCode, pMemberInfo->m_sMemberCode, sExitUserId );
        }
        else if (!theApp->isHideMainFrame())
         {
//            CFrameWndInfoProxy::OnRemoveMember(pGroupInfo->m_sGroupCode, pMemberInfo->m_sMemberCode);
        }
        else if (m_pDlgFrameList!=0) {
            m_pDlgFrameList->onRemoveMember( pGroupInfo->m_sGroupCode, pMemberInfo->m_sMemberCode, sExitUserId );
        }

        /// 放在 m_pDlgFrameList->onRemoveMember 一起处理
//        CDlgDialog::pointer pDlgDialog = GetCallIdDialog(pGroupInfo->m_sGroupCode);
//        if (pDlgDialog.get()!=NULL)
//        {
//            pDlgDialog->UserExitRoom(sExitUserId,true);
//        }

//        if (m_pDlgMyEnterprise != NULL)
//            m_pDlgMyEnterprise->DeleteEmployeeInfo(pGroupInfo,pMemberInfo->m_sMemberCode);
        if (m_widgetMyGroup != 0) {// && theEBAppClient.EB_IsMyGroup(pMemberInfo->m_sGroupCode))
            m_widgetMyGroup->deleteMemberInfo( pGroupInfo,pMemberInfo->m_sMemberCode,sExitUserId );
        }
    }
    if ( pGroupInfo->m_nCreateUserId==theApp->logonUserId() ) {
        QString text = theLocales.getLocalText("message-show.member-exit-group","Member Exit Group");
        text.replace( "[USER_NAME]", pMemberInfo->m_sUserName.c_str() );
        text.replace( "[USER_ACCOUNT]", pMemberInfo->m_sMemberAccount.c_str() );
        text.replace( "[GROUP_NAME]", pGroupInfo->m_sGroupName.c_str() );
        text.replace( "[GROUP_ID]", QString::number(pGroupInfo->m_sGroupCode) );
        EbMessageBox::doShow( NULL, "", QChar::Null, text, EbMessageBox::IMAGE_INFORMATION,default_warning_auto_close );
    }
}
void DialogMainFrame::onRemoveGroup(QEvent *e)
{
    const EB_GroupInfo* pGroupInfo = (const EB_GroupInfo*)e;
    const EB_MemberInfo* pMemberInfo = (const EB_MemberInfo*)pGroupInfo->GetEventData();

    /// 删除本地群组聊天记录
    theApp->deleteDbRecord(pGroupInfo->m_sGroupCode,false);

    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    const eb::bigint sRemoveUserId(pMemberInfo->m_nMemberUserId);
    /// 找到现在会话，移除用户数据
    if (sRemoveUserId == theApp->logonUserId()) {
        theApp->m_pCallList.remove(pGroupInfo->m_sGroupCode);
        if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT && m_pDlgFrameList!=0) {
            m_pDlgFrameList->onRemoveGroup(pGroupInfo->m_sGroupCode);
        }
        else if (!theApp->isHideMainFrame()) {
//            CFrameWndInfoProxy::OnRemoveGroup(pGroupInfo->m_sGroupCode);
        }
        else if (m_pDlgFrameList!=0) {
            m_pDlgFrameList->onRemoveGroup(pGroupInfo->m_sGroupCode);
        }

//        DialogChatBase::pointer pDlgDialog = getDialogChatBase(pGroupInfo->m_sGroupCode,true);
        if (m_widgetMyGroup!=0) {
            m_widgetMyGroup->onRemoveGroup(pGroupInfo);
        }

        /// 你被管理员移除出：\r\n%s
        QString text = theLocales.getLocalText("message-show.remove-from-group","Remove From Group");
        text.replace( "[GROUP_NAME]", pGroupInfo->m_sGroupName.c_str() );
        text.replace( "[GROUP_ID]", QString::number(pGroupInfo->m_sGroupCode) );
        EbMessageBox::doShow( NULL, "", QChar::Null, text, EbMessageBox::IMAGE_INFORMATION,default_warning_auto_close );
    }
    else {
        if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT && m_pDlgFrameList!=0) {
            m_pDlgFrameList->onRemoveMember( pGroupInfo->m_sGroupCode,pMemberInfo->m_sMemberCode,sRemoveUserId );
        }
        else if (!theApp->isHideMainFrame()) {
//            CFrameWndInfoProxy::OnRemoveMember(pGroupInfo->m_sGroupCode, pMemberInfo->m_sMemberCode);
        }
        else if (m_pDlgFrameList!=0) {
            m_pDlgFrameList->onRemoveMember(pGroupInfo->m_sGroupCode, pMemberInfo->m_sMemberCode, sRemoveUserId);
        }

        /// 放在 m_pDlgFrameList->onRemoveMember 一起处理
//        DialogChatBase::pointer pDlgDialog = getDialogChatBase(pGroupInfo->m_sGroupCode);
//        if (pDlgDialog.get()!=0) {
//            pDlgDialog->UserExitRoom(sRemoveUserId,true);
//        }
        if (m_widgetMyGroup != 0) { /// && theEBAppClient.EB_IsMyGroup(pMemberInfo->m_sGroupCode))
            m_widgetMyGroup->deleteMemberInfo(pGroupInfo,pMemberInfo->m_sMemberCode,false);
        }
    }
//    if (pGroupInfo->m_sEnterpriseCode>0 && m_pDlgMyEnterprise != 0)
//        m_pDlgMyEnterprise->DeleteEmployeeInfo(pGroupInfo,pMemberInfo->m_sMemberCode);
}
void DialogMainFrame::onGroupEditResponse(QEvent *e)
{
    const EB_GroupInfo* pGroupInfo = (const EB_GroupInfo*)e;
    const EB_STATE_CODE stateCode = (EB_STATE_CODE)pGroupInfo->GetEventParameter();
    QString text;
    EbMessageBox::IMAGE_TYPE imageType = EbMessageBox::IMAGE_WARNING;
    switch (stateCode) {
    case EB_STATE_OK: {
        /// 操作成功
        const bool bNewGroup = pGroupInfo->GetEventBigParameter()==1?true:false;
        imageType = EbMessageBox::IMAGE_INFORMATION;
        if (bNewGroup)
            text = theLocales.getLocalText("on-group-edit-response.new-state-ok.text","");
        else
            text = theLocales.getLocalText("on-group-edit-response.edit-state-ok.text","");
        break;
    }
    case EB_STATE_NOT_AUTH_ERROR: {
        /// 没有权限：\r\n请联系管理员！
        text = theLocales.getLocalText("on-group-edit-response.not-auth-error.text","Not Auth Error");
        break;
    }
    default: {
        /// 操作失败,请重试或联系公司客服：<br>错误代码: [STATE_CODE]
        text = theLocales.getLocalText("on-group-edit-response.other-error.text","Other Error");
        break;
    }
    }
    if (!text.isEmpty()) {
        text.replace( "[GROUP_NAME]",pGroupInfo->m_sGroupName.c_str() );
        text.replace( "[GROUP_ID]",QString::number(pGroupInfo->m_sGroupCode) );
        text.replace( "[STATE_CODE]",QString::number((int)stateCode) );
        EbMessageBox::doShow( NULL, "", QChar::Null, text, imageType,default_warning_auto_close );
    }
}
void DialogMainFrame::onGroupDelete(QEvent *e)
{
    const EB_GroupInfo* pGroupInfo = (const EB_GroupInfo*)e;
    const eb::bigint sGroupCode = pGroupInfo->m_sGroupCode;
//    bool bIsMyDepartment = (pGroupInfo->GetEventParameter()==1)?true:false;
//    if (m_pDlgMyEnterprise != NULL)
//    {
//        m_pDlgMyEnterprise->DeleteDepartmentInfo(pGroupInfo->m_sGroupCode);
//    }
    if (m_widgetMyGroup != 0) {
        m_widgetMyGroup->onRemoveGroup(pGroupInfo);
//        m_widgetMyGroup->DeleteDepartmentInfo(pGroupInfo);
    }

    /// 关闭现在会话窗口
    theApp->m_pCallList.remove(pGroupInfo->m_sGroupCode);

    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT && m_pDlgFrameList!=0) {
        m_pDlgFrameList->onRemoveGroup(pGroupInfo->m_sGroupCode);
    }
    else if (!theApp->isHideMainFrame()) {
//        CFrameWndInfoProxy::OnRemoveGroup(pGroupInfo->m_sGroupCode);
    }
    else if (m_pDlgFrameList!=0) {
        m_pDlgFrameList->onRemoveGroup(pGroupInfo->m_sGroupCode);
    }

    getDialogChatBase(pGroupInfo->m_sGroupCode, true);

    /// %s\r\n已经被解散！
    QString text = theLocales.getLocalText("message-show.delete-group","Delete Group");
    text.replace( "[GROUP_NAME]", pGroupInfo->m_sGroupName.c_str() );
    text.replace( "[GROUP_ID]", QString::number(pGroupInfo->m_sGroupCode) );
    EbMessageBox::doShow( NULL, "", QChar::Null, text, EbMessageBox::IMAGE_INFORMATION,default_information_auto_close );

    theApp->deleteDbRecord(sGroupCode,false);
}

void DialogMainFrame::onGroupInfo(QEvent *e)
{
    const EB_GroupInfo * pGroupInfo = (EB_GroupInfo*)e;

//    if (m_pDlgMyEnterprise!=0) {
//        m_pDlgMyEnterprise->DepartmentInfo(pGroupInfo);
//    }
    if (pGroupInfo->m_nMyEmpId>0 && this->m_widgetMyGroup != NULL) {
        this->m_widgetMyGroup->onGroupInfo(pGroupInfo);
    }
    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
        /// **跑下面
    }
    else if ( !theApp->isHideMainFrame() ) {
//        CFrameWndInfoProxy::ChangeDepartmentInfo(pGroupInfo);
        return;
    }

//    if (m_pDlgFrameList!=NULL) {
//        m_pDlgFrameList->ChangeDepartmentInfo(pGroupInfo);
//    }
}

void DialogMainFrame::onEnterpriseInfo(QEvent *e)
{
    const EB_EnterpriseInfo * pEvent = (EB_EnterpriseInfo*)e;
    theApp->setEnterpriseCreateUserId(pEvent->m_nCreateUserId);
//    if (m_pDlgMyEnterprise != NULL)
//        m_pDlgMyEnterprise->EnterpriseInfo(pEnterpriseInfo);

//    if (m_btnMyEnterprise.GetSafeHwnd() && !m_btnMyEnterprise.IsWindowVisible())
//    {
//        m_btnMyEnterprise.ShowWindow(SW_SHOW);
//        m_btnMyEnterprise.Invalidate();
//    }

    changeTrayTooltip();
}

void DialogMainFrame::onLogonSuccess(QEvent * e)
{
    const EB_AccountInfo * pAccountInfo = (EB_AccountInfo*)e;
    if (pAccountInfo==NULL) {
        // from
    }
    else {

    }

//    if (wParam!=0)
    {
        unsigned long pEntManagerUrl = 0;
        theApp->m_ebum.EB_GetSystemParameter(EB_SYSTEM_PARAMETER_ENT_MANAGER_URL, &pEntManagerUrl);
        if (pEntManagerUrl != NULL && strlen((const char*)pEntManagerUrl)>0) {
            theApp->setEntManagerUrl( QString::fromLatin1((const char*)pEntManagerUrl));
            theApp->m_ebum.EB_FreeSystemParameter(EB_SYSTEM_PARAMETER_ENT_MANAGER_URL,pEntManagerUrl);
        }
    }
    theApp->m_ebum.EB_LoadOrg();	// 加载组织结构、表情、头像
    if (theApp->m_ebum.EB_IsLogonVisitor()) {
        //this->GetDlgItem(IDC_BUTTON_MYCONTACTS)->EnableWindow(FALSE);
    }
    else {
        theApp->m_ebum.EB_LoadContact();
    }
    const tstring sHeadFile = theApp->m_ebum.EB_GetMyDefaultMemberHeadFile();
    const QString userHeadFile = QString::fromStdString(sHeadFile.string());
    if ( !userHeadFile.isEmpty() && QFile::exists(userHeadFile)) {
        m_labelUserImage->setPixmap( QPixmap(userHeadFile).scaled(m_rectHead.width(),m_rectHead.height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation) );
    }

    /// 登录成功，先创建加载好表情，等需要的时候，直接可以用
    theApp->showDialogEmotionSelect(QPoint());

//    theUpdateResetTimer = false;
//    KillTimer(TIMERID_CHECK_UPDATE);
//    SetTimer(TIMERID_CHECK_UPDATE,(1+rand()%5)*60*1000,NULL);	// 先1-5分钟检查
//    SetTimer(TIMERID_LOAD_LOCAL_UNREAD_MSG,6000,NULL);

//    if (this->m_pDlgAppFrame!=NULL)
//    {
//        this->m_pDlgAppFrame->OnLogonSuccess();
//    }
//    if (this->m_pDlgFrameList!=NULL)
//    {
//        this->m_pDlgFrameList->OnLogonSuccess();
//    }

}

void DialogMainFrame::onLogonTimeout(QEvent *e)
{
    QString text = theLocales.getLocalText("on-logon-response.logon-timeout.text","logon timeout");
    text.replace( "[STATE_CODE]", QString::number( (int)EB_STATE_TIMEOUT_ERROR ) );
    const QString title = theLocales.getLocalText("on-logon-response.logon-timeout.title","");
//    EbMessageBox::doExec( 0,title, QChar::Null, text, EbMessageBox::IMAGE_WARNING,default_warning_auto_close,QMessageBox::Ok );
    EbMessageBox::doShow( NULL, title, QChar::Null, text, EbMessageBox::IMAGE_WARNING,default_warning_auto_close );
}
void DialogMainFrame::onLogonError(QEvent *e)
{
    const EB_AccountInfo * pAccountInfo = (EB_AccountInfo*)e;
    const EB_STATE_CODE stateCode = (EB_STATE_CODE)pAccountInfo->GetEventParameter();

    QString sErrorText;
    switch (stateCode) {
    case EB_WM_LOGON_TIMEOUT:
        // 登录超时，请重新登录！\r\n错误代码:%d
        sErrorText = theLocales.getLocalText("on-logon-response.logon-timeout.text","Logon Timeout");
        break;
    case EB_STATE_UNAUTH_ERROR:
        // 帐号未激活，请查收邮件，完成注册！\r\n错误代码:%d
        sErrorText = theLocales.getLocalText("on-logon-response.unauth-error.text","Unauth Error");
        break;
    case EB_STATE_ACCOUNT_FREEZE:
        // 帐号已经被临时冻结，请联系公司客服！\r\n错误代码:%d
        sErrorText = theLocales.getLocalText("on-logon-response.account-freeze.text","Account Freeze");
        break;
    case EB_STATE_MAX_RETRY_ERROR:
        // 错误次数太多，帐号被临时锁住，请稍候再试！\r\n错误代码:%d
        sErrorText = theLocales.getLocalText("on-logon-response.max-retry-error.text","Max Retry Error");
        break;
    case EB_STATE_NOT_AUTH_ERROR:
        // 没有权限错误！\r\n错误代码:%d
        sErrorText = theLocales.getLocalText("on-logon-response.not-auth-error.text","Not Auth Error");
        break;
    case EB_STATE_ACCOUNT_NOT_EXIST:
        // 帐号不存在，请重新输入！\r\n错误代码:%d
        sErrorText = theLocales.getLocalText("on-logon-response.account-not-exist.text","Account Not Exist");
        break;
    case EB_STATE_ACC_PWD_ERROR:
        // 帐号或密码错误，请重新输入！\r\n错误代码:%d
        sErrorText = theLocales.getLocalText("on-logon-response.acc-pwd-error.text","Account Or Password Error");
        break;
    default:
        // 登录失败，请重试！\r\n错误代码:%d
        sErrorText = theLocales.getLocalText("on-logon-response.other-error.text","Logon Error");
        break;
    }
    EbMessageBox::doShow( NULL, "", QChar::Null, sErrorText, EbMessageBox::IMAGE_WARNING,default_warning_auto_close );

//    int nErrorCode = lParam;
//	if (nErrorCode == EB_STATE_UNAUTH_ERROR)
//		CDlgMessageBox::EbMessageBox(this,_T(""),_T("该帐号未通过验证：\r\n请查收邮件，点击验证链接，完成注册！"),CDlgMessageBox::IMAGE_WARNING,5);
//	else if (nErrorCode == EB_STATE_ACCOUNT_FREEZE)
//		CDlgMessageBox::EbMessageBox(this,"","帐号已被临时冻结：\r\n请联系公司客服！",CDlgMessageBox::IMAGE_WARNING,5);
//	else if (nErrorCode == EB_STATE_MAX_RETRY_ERROR)
//		CDlgMessageBox::EbMessageBox(this,"","错误次数太频繁：\r\n帐号已被临时锁住，请稍候再试！",CDlgMessageBox::IMAGE_WARNING,5);
//	else
//	{
//		CString sText;
//		sText.Format(_T("错误代码：%d\r\n请重新登录！"),nErrorCode);
//		CDlgMessageBox::EbDoModal(this,"登录失败",sText,CDlgMessageBox::IMAGE_WARNING,true);
//		OnLogout();
//	}
}

void DialogMainFrame::onOnlineAnother(QEvent *e)
{
    const EB_Event * pEvent = (EB_Event*)e;
    const int nOnlineAnotherType = (int)pEvent->GetEventParameter();
    // 0=已经在其他地方登录，退出当前连接；
    // 1=修改密码，退出当前连接；
    //    SetTimer(TIMERID_LOGOUT,10,NULL);
}

#ifdef USES_EVENT_DATE_TIMER
void DialogMainFrame::checkEventData()
#else
bool DialogMainFrame::checkEventData(QEvent * e)
#endif
{
#ifdef USES_EVENT_DATE_TIMER
    QEvent* e = NULL;
    while ( (e=m_eventList.front())!=0) {
#endif
        bool result = true;
        const QEvent::Type eventType = e->type();
        switch ((EB_COMMAND_ID)eventType) {
        case EB_WM_BROADCAST_MSG:
            onBroadcastMsg(e);
            break;
        case EB_WM_AREA_INFO:
            onAreaInfo(e);
            break;
        case EB_WM_USER_STATE_CHANGE:
            onUserStateChange(e);
            break;
        case EB_WM_USER_HEAD_CHANGE:
            onMemberHeadChange(e);
            break;
        case EB_WM_CONTACT_HEAD_CHANGE:
            onContactHeadChange(e);
            break;
        /// 聊天消息
        case CR_WM_MSG_RECEIPT:
            onMsgReceipt(e);
            break;
        case CR_WM_SEND_RICH:
            onSendRich(e);
            break;
        case CR_WM_RECEIVE_RICH:
            result = onReceiveRich(e);
            break;
        case CR_WM_SENDING_FILE:
            onSendingFile(e);
            break;
        case CR_WM_SENT_FILE:
            onSentFile(e);
            break;
        case CR_WM_CANCEL_FILE:
            onCancelFile(e);
            break;
        case CR_WM_RECEIVING_FILE:
            result = onReceivingFile(e);
            break;
        case CR_WM_RECEIVED_FILE:
            onReceivedFile(e);
            break;
        case CR_WM_FILE_PERCENT:
            onFilePercent(e);
            break;
        /// 聊天会话
        case EB_WM_CALL_CONNECTED:
            onCallConnected(e);
            break;
        case EB_WM_CALL_ERROR:
            onCallError(e);
            break;
        case EB_WM_CALL_HANGUP:
            onCallHangup(e);
            break;
        case EB_WM_CALL_ALERTING:
            onCallAlerting(e);
            break;
        case EB_WM_CALL_INCOMING:
            onCallIncoming(e);
            break;
            ///  联系人
        case EB_WM_CONTACT_DELETE:
            onContactDelete(e);
            break;
        case EB_WM_CONTACT_INFO:
            onContactInfo(e);
            break;
        case EB_WM_ACCEPT_ADDCONTACT:
            onAcceptAddContact(e);
            break;
        case EB_WM_REJECT_ADDCONTACT:
            onRejectAddContact(e);
            break;
        case EB_WM_REQUEST_ADDCONTACT:
            onRequestAddContact(e);
            break;
            //// 组织结构
        case EB_WM_EDITINFO_RESPONSE:
            onEditInfoResponse(e);
            break;
        case EB_WM_MEMBER_EDIT_RESPONSE:
            onMemberEditResponse(e);
            break;
        case EB_WM_MEMBER_DELETE:
            onMemberDelete(e);
            break;
        case EB_WM_MEMBER_INFO:
            onMemberInfo(e);
            break;
        case EB_WM_REJECT_ADD2GROUP:
            onRejectAdd2Group(e);
            break;
        case EB_WM_INVITE_ADD2GROUP:
            onInviteAdd2Group(e);
            break;
        case EB_WM_REQUEST_ADD2GROUP:
            onRequestAdd2Group(e);
            break;
        case EB_WM_EXIT_GROUP:
            onExitGroup(e);
            break;
        case EB_WM_REMOVE_GROUP:
            onRemoveGroup(e);
            break;
        case EB_WM_GROUP_EDIT_RESPONSE:
            onGroupEditResponse(e);
            break;
        case EB_WM_GROUP_DELETE:
            onGroupDelete(e);
            break;
        case EB_WM_GROUP_INFO:
            onGroupInfo(e);
            break;
        case EB_WM_ENTERPRISE_INFO:
            onEnterpriseInfo(e);
            break;
            //// 登录
        case EB_WM_LOGON_SUCCESS:
            onLogonSuccess(e);
            break;
        case EB_WM_LOGON_TIMEOUT:
            onLogonTimeout(e);
            break;
        case EB_WM_LOGON_ERROR:
            onLogonError(e);
            break;
        case EB_WM_ONLINE_ANOTHER:
            onOnlineAnother(e);
            break;
        default:
            break;
        }

        /// 返回结果
        if (eventType>=CR_WM_ENTER_ROOM && eventType<=CR_WM_EVENT_RESULT) {
            CCrInfo * aCrInfo = (CCrInfo*)e;
            QObject * receiver = aCrInfo->receiver();
            if (receiver!=0) {
                CCrInfo * event = new CCrInfo();
                event->SetQEventType((QEvent::Type)CR_WM_EVENT_RESULT);
                event->setReceiveResult(aCrInfo->receiveKey(),result?0:-1);
                QCoreApplication::postEvent(receiver,event);
            }
        }
        else if (eventType>=EB_WM_APPID_SUCCESS && eventType<=EB_WM_EVENT_RESULT) {
            EB_Event * aEvent = (EB_Event*)e;
            QObject * receiver = aEvent->receiver();
            if (receiver!=0) {
                EB_Event * event = new EB_Event((QEvent::Type)EB_WM_EVENT_RESULT);
                event->setReceiveResult(aEvent->receiveKey(),result?0:-1);
                QCoreApplication::postEvent(receiver,event);
            }
        }
#ifdef USES_EVENT_DATE_TIMER
        m_eventMap.insert(e,result);
    }
#else
        return result;
#endif
}


void DialogMainFrame::checkOneSecond()
{
    static unsigned int theSeconedIndex = 0;
    theSeconedIndex++;

    if ((theSeconedIndex%5)==4) {   /// 5秒判断一次
        /// 判断超时无用会话，清空数据
        checkCallExit();
    }

}

void DialogMainFrame::checkCallExit()
{
    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    const time_t tNow = time(0);
    BoostReadLock rdlock(theApp->m_pCallList.mutex());
    CLockMap<eb::bigint, EbcCallInfo::pointer>::const_iterator pIterCallList = theApp->m_pCallList.begin();
    for (; pIterCallList!=theApp->m_pCallList.end(); pIterCallList++) {
        const EbcCallInfo::pointer& pEbCallInfo = pIterCallList->second;
        const eb::bigint sCallid = pEbCallInfo->m_pCallInfo.GetCallId();
        if ((tNow-pEbCallInfo->m_tLastTime)<=5*60) continue;

        if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
            /// *跑下面
        }
        else if (!theApp->isHideMainFrame()) {
//            if (!CFrameWndInfoProxy::ExistCallIdWnd(sCallid))
//            //if (CFrameWndInfoProxy::RemoveHideWnd(sCallid) || !CFrameWndInfoProxy::ExistCallIdWnd(sCallid))
//            {
//                theApp.m_pCallList.erase(pIterCallList);
//                // 这里主要用于处理自动响应，没有打开聊天界面会话
//                // *其实在OnMessageCallConnected已经有处理；
//                theEBAppClient.EB_CallExit(sCallid);
//                break;
//            }
            continue;
        }
        if (m_pDlgFrameList!=0 && !m_pDlgFrameList->existFrameItem(sCallid)) {
            theApp->m_pCallList.erase(pIterCallList);
            /// 这里主要用于处理自动响应，没有打开聊天界面会话
            /// *其实在OnMessageCallConnected已经有处理；
            theApp->m_ebum.EB_CallExit(sCallid);
            break;
        }
    }
}

void DialogMainFrame::mousePressEvent(QMouseEvent *event)
{
    if (ui->lineEditUserDescription->hasFocus() ||  // 切换实现 “个人签名” 位置刷新界面
            ui->lineEditSearch->hasFocus() ) {      // 切换实现 “搜索框” 位置刷新界面
        m_labelLinState->setFocus();
    }
    EbDialogBase::mousePressEvent(event);
}

void DialogMainFrame::mouseDoubleClickEvent(QMouseEvent *event)
{
    EbDialogBase::mouseDoubleClickEvent(event);
}

void DialogMainFrame::keyPressEvent(QKeyEvent * e)
{
    if ( e->key()==Qt::Key_Escape && ui->lineEditUserDescription->hasFocus()) {
        const QString oldDescription( theApp->m_ebum.EB_GetDescription().c_str() );
        ui->lineEditUserDescription->setText( oldDescription );
        ui->labelLineState->setFocus();
    }
    EbDialogBase::keyPressEvent(e);
}

void DialogMainFrame::contextMenuEvent(QContextMenuEvent * e)
{
    createMenuData();
    m_actionMyCollection->setEnabled( theApp->myCollectionSugId()>0 );

    m_menuContext->exec(e->globalPos());
    EbDialogBase::contextMenuEvent(e);
}

bool DialogMainFrame::eventFilter(QObject *obj, QEvent *e)
{
    // pass the event on to the parent class
    if (obj==ui->lineEditUserDescription) {
        bool checkUpdateDescription = e->type()==QEvent::FocusOut?true:false;
        if (!checkUpdateDescription && e->type()==QEvent::KeyPress) {
            const QKeyEvent * event = (QKeyEvent*)e;
            if (event->key()==Qt::Key_Return || event->key()==Qt::Key_Enter) {
                checkUpdateDescription = true;
//                ui->labelLineState->setFocus();   /// 感觉没有作用
            }
        }
        if (checkUpdateDescription)  {
            /// 判断是否修改个人签名
            const QString oldDescription( theApp->m_ebum.EB_GetDescription().c_str() );
            const QString newDescription = ui->lineEditUserDescription->text();
            if (newDescription!=oldDescription) {
                theApp->m_ebum.EB_SetDescription( newDescription.toStdString().c_str() );
            }
        }
    }
    return EbDialogBase::eventFilter(obj, e);
}

void DialogMainFrame::timerEvent(QTimerEvent *event)
{
#ifdef USES_EVENT_DATE_TIMER
    if (m_checkEventData!=0 && event->timerId()==m_checkEventData) {
        checkEventData();
    }
#endif
    if (m_timerOneSecond!=0 && event->timerId()==m_timerOneSecond) {
        checkOneSecond();
    }
    EbDialogBase::timerEvent(event);
}

void DialogMainFrame::onClickedPushButtonSetting(void)
{
    createMenuData();
    /// ** set menu checked and uncheck
    int nFindColorIndex = 0;
    const std::vector<EbColorInfo::pointer>& colors = theLocales.colors();
    for (size_t i=0; i<colors.size(); i++) {
        const EbColorInfo::pointer colorInfo = colors[i];
        if (colorInfo->color()==theApp->getMainColor()) {
            nFindColorIndex = (int)(i+1);
            break;
        }
    }
    bool bFindColoeChecked = false;
    const QList<QAction*> actionList = m_menuSetting->actions();
    for ( int i=0; i!=actionList.size(); ++i ) {
        QAction* action = actionList.at(i);
        bool bOk = false;
        if (!bFindColoeChecked && action->data().toInt(&bOk)==nFindColorIndex && bOk ) {
            action->setChecked(true);
            bFindColoeChecked = true;
        }
        else {
            action->setChecked(false);
        }
    }

    const QPoint pos(0,ui->pushButtonSetting->geometry().height());
    m_menuSetting->exec(ui->pushButtonSetting->mapToGlobal(pos));
}

void DialogMainFrame::onTriggeredActionSelectColor(void)
{
    QAction* pAction = dynamic_cast<QAction*>( sender() );
    pAction->setChecked(true);
    const int nColorIndex = pAction->data().toInt();
    const std::vector<EbColorInfo::pointer>& colors = theLocales.colors();
    if (nColorIndex>=1 && nColorIndex<=(int)colors.size()) {
        theApp->setMainColor( colors[nColorIndex-1]->color(), true );
    }
    else {
        QColor c = QColorDialog::getColor( theApp->getMainColor() );
        if ( !c.isValid() ) {
            return;
        }
        theApp->setMainColor( c,true );
    }
    refreshSkin();
}

void DialogMainFrame::updateLineState()
{
    switch (theApp->m_ebum.EB_GetLineState())
    {
    case EB_LINE_STATE_BUSY:
        m_labelLinState->setPixmap( QPixmap(":/img/btnstatebusy.png") );
        break;
    case EB_LINE_STATE_AWAY:
        m_labelLinState->setPixmap( QPixmap(":/img/btnstateaway.png") );
        break;
    default:
        m_labelLinState->setPixmap( QPixmap(":/img/btnstateonline.png") );
        break;
    }
    m_labelLinState->setToolTip( theApp->lineStateText() );
    changeTrayTooltip();
}

void DialogMainFrame::onClickedLabelUserImage()
{
    if (m_pDlgMyCenter==0) {
        m_pDlgMyCenter = new DialogMyCenter;
        m_pDlgMyCenter->setModal(false);
        m_pDlgMyCenter->setWindowModality(Qt::WindowModal);
    }
    m_pDlgMyCenter->show();
}

void DialogMainFrame::onClickedLineState()
{
    createMenuData();
    const EB_USER_LINE_STATE lineState = theApp->m_ebum.EB_GetLineState();
    bool bFindColoeChecked = false;
    const QList<QAction*> actionList = m_menuLineState->actions();
    for ( int i=0; i!=actionList.size(); ++i ) {
        QAction* action = actionList.at(i);
        bool bOk = false;
        if (!bFindColoeChecked && action->data().toInt(&bOk)==(int)lineState && bOk ) {
            action->setChecked(true);
            bFindColoeChecked = true;
        }
        else {
            action->setChecked(false);
        }
    }
//    this->setFocus(); /// 没有用

    const QPoint pos(0,m_labelLinState->geometry().height());
    m_menuLineState->exec(m_labelLinState->mapToGlobal(pos));
}

void DialogMainFrame::onClickedMenuLineState()
{
    QAction* pAction = dynamic_cast<QAction*>( sender() );
    pAction->setChecked(true);
    const EB_USER_LINE_STATE lineState = (EB_USER_LINE_STATE)pAction->data().toInt();
    if (lineState>=EB_LINE_STATE_BUSY) {
        theApp->m_ebum.EB_SetLineState(lineState);
        updateLineState();
    }
}

void DialogMainFrame::onTriggeredActionOpenWorkFrame()
{
    CreateFrameList(false);
    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
        /// 添加工作台
        m_pDlgFrameList->showWorkFrame();
//            m_pDlgFrameList->addFrameItem( EbFrameItem::create(EbFrameItem::FRAME_ITEM_WORK_FRAME),true,false );
//            DialogWorkFrame * pDialogWorkFrame = m_pDlgFrameList->getDialogWorkFrame();
//            if ( pDialogWorkFrame->isEmpty() ) {
//                pDialogWorkFrame->addUrl(false,"about:blank","");
//                /// for test
////                pDialogWorkFrame->addUrl(false,"www.baidu.com","");
//            }
    }
    else {
    }
    m_pDlgFrameList->showFrameList();
}

void DialogMainFrame::onTriggeredActionMyCollection()
{
    if ( theApp->isLogonVisitor() ) {
        return;
    }
    const mycp::bigint myCollectionSubId = theApp->myCollectionSugId();
    if (myCollectionSubId==0 ) {
        return;
    }
    EB_SubscribeFuncInfo subscribeFuncInfo;
    if ( theApp->m_ebum.EB_GetSubscribeFuncInfo( myCollectionSubId, &subscribeFuncInfo ) ) {
        openSubscribeFuncWindow( subscribeFuncInfo );
    }

}

void DialogMainFrame::onTriggeredActionLogout()
{
    this->accept();
}

void DialogMainFrame::onTriggeredActionExitApp()
{
    this->accept();
}

//void DialogMainFrame::onClickedPushButtonFileManager(void)
//{
//    ui->pushButtonMyGroup->setChecked(false);
//    ui->pushButtonMyCenter->setChecked(false);
//    ui->pushButtonFileManager->setChecked(true);
//    ui->pushButtonMyEnterprise->setChecked(false);
//}

void DialogMainFrame::onClickedPushButtonMyGroup(void)
{
    if (m_widgetMyGroup==NULL) {
        m_widgetMyGroup = new EbWidgetMyGroup(this);
        const QRect & clientRect = this->geometry();
        const QRect & buttonRect = ui->pushButtonMyGroup->geometry();
        const int y = buttonRect.bottom()+1;
        const int h = clientRect.height()-y-22;
        m_widgetMyGroup->setGeometry( 1,y,clientRect.width()-2,h );
        m_widgetMyGroup->show();
    }
    updateMyButton(ui->pushButtonMyGroup);
}

void DialogMainFrame::onClickedPushButtonMyContact(void)
{
    updateMyButton(ui->pushButtonMyContact);
}
void DialogMainFrame::onClickedPushButtonMySession(void)
{
    updateMyButton(ui->pushButtonMySession);
}

void DialogMainFrame::onClickedPushButtonMyEnterprise(void)
{
    updateMyButton(ui->pushButtonMyEnterprise);
}
void DialogMainFrame::onClickedPushButtonMyApp(void)
{
    updateMyButton(ui->pushButtonMyApp);
}

void DialogMainFrame::updateMyButton(const QPushButton* fromButton)
{
    ui->pushButtonMyGroup->setChecked(ui->pushButtonMyGroup==fromButton?true:false);
    if (m_widgetMyGroup!=NULL) {
        m_widgetMyGroup->setVisible(ui->pushButtonMyGroup->isChecked());
    }
    ui->pushButtonMyContact->setChecked(ui->pushButtonMyContact==fromButton?true:false);
    ui->pushButtonMySession->setChecked(ui->pushButtonMySession==fromButton?true:false);
    ui->pushButtonMyEnterprise->setChecked(ui->pushButtonMyEnterprise==fromButton?true:false);
    ui->pushButtonMyApp->setChecked(ui->pushButtonMyApp==fromButton?true:false);
}

void DialogMainFrame::onClickedSubApp(const EB_SubscribeFuncInfo & subFunnInfo)
{
    openSubscribeFuncWindow( subFunnInfo );
}

bool DialogMainFrame::openSubscribeFuncWindow(
            const EB_SubscribeFuncInfo &subFuncInfo, const std::string &/*postData*/, const std::string &sParameters)
{
    const QString sFullFuncurl = theApp->subscribeFuncUrl( subFuncInfo.m_nSubscribeId, sParameters );
    if ( sFullFuncurl.isEmpty() ) {
        return false;
    }
    if (subFuncInfo.m_nFunctionMode == EB_FUNC_MODE_MODAL) {
        /// 对话框模式
//        CDlgFuncWindow pFuncWindow;
//        pFuncWindow.m_nFuncBrowserType = pSubscribeFuncInfo.m_nBrowserType;
//        pFuncWindow.m_sTitle = pSubscribeFuncInfo.m_sFunctionName;
//        pFuncWindow.m_sFuncUrl = sFullFuncurl;
//        pFuncWindow.m_sPostData = sPostData;
//        pFuncWindow.m_nWidth = pSubscribeFuncInfo.m_nWindowWidth;
//        pFuncWindow.m_nHeight = pSubscribeFuncInfo.m_nWindowHeight;
//        pFuncWindow.m_bDisableContextMenu = pSubscribeFuncInfo.m_bDisableContextMenu;
//        pFuncWindow.m_bDisableScrollBar = pSubscribeFuncInfo.m_bDisableScrollBar;
//        pFuncWindow.m_bClosePrompt = pSubscribeFuncInfo.m_bClosePrompt;
//        pFuncWindow.m_bWinResizable = pSubscribeFuncInfo.m_bWinResizable;
//        pFuncWindow.DoModal();
    }
//    else if (pSubscribeFuncInfo.m_nFunctionMode == EB_FUNC_MODE_WINDOW ||	/// 窗口模式；
//             (pSubscribeFuncInfo.m_nFunctionMode == EB_FUNC_MODE_MAINFRAME && m_bHideMainFrame && m_nDefaultUIStyleType == EB_UI_STYLE_TYPE_OFFICE))
//    {
//        if (GetAutoOpenSubId()==pSubscribeFuncInfo.m_nSubscribeId)
//        {
//            if (m_pAutoOpenFuncWindow.get()!=NULL)
//            {
//                if (!m_pAutoOpenFuncWindow->IsWindowVisible())
//                    m_pAutoOpenFuncWindow->ShowWindow(SW_SHOW);
//            }else
//            {
//                CWnd * pParent = CWnd::FromHandle(::GetDesktopWindow());
//                m_pAutoOpenFuncWindow = CDlgFuncWindow::pointer(new CDlgFuncWindow(pParent,false));
//                m_pAutoOpenFuncWindow->m_nFuncBrowserType = pSubscribeFuncInfo.m_nBrowserType;
//                m_pAutoOpenFuncWindow->m_sTitle = pSubscribeFuncInfo.m_sFunctionName;
//                m_pAutoOpenFuncWindow->m_sFuncUrl = sFullFuncurl;
//                //m_pAutoOpenFuncWindow->m_sPostData = sPostData;
//                m_pAutoOpenFuncWindow->m_nWidth = pSubscribeFuncInfo.m_nWindowWidth;
//                m_pAutoOpenFuncWindow->m_nHeight = pSubscribeFuncInfo.m_nWindowHeight;
//                m_pAutoOpenFuncWindow->m_bDisableContextMenu = pSubscribeFuncInfo.m_bDisableContextMenu;
//                m_pAutoOpenFuncWindow->m_bDisableScrollBar = pSubscribeFuncInfo.m_bDisableScrollBar;
//                m_pAutoOpenFuncWindow->m_bClosePrompt = pSubscribeFuncInfo.m_bClosePrompt;
//                m_pAutoOpenFuncWindow->m_bWinResizable = pSubscribeFuncInfo.m_bWinResizable;
//                m_pAutoOpenFuncWindow->Create(CDlgFuncWindow::IDD,pParent);
//                //m_pAutoOpenFuncWindow->ShowWindow(SW_SHOW);
//            }
//        }else
//        {
//            CWnd * pParent = CWnd::FromHandle(::GetDesktopWindow());
//            CDlgFuncWindow * pFuncWindow = new CDlgFuncWindow(pParent,true);
//            pFuncWindow->m_nFuncBrowserType = pSubscribeFuncInfo.m_nBrowserType;
//            pFuncWindow->m_sTitle = pSubscribeFuncInfo.m_sFunctionName;
//            pFuncWindow->m_sFuncUrl = sFullFuncurl;
//            pFuncWindow->m_sPostData = sPostData;
//            pFuncWindow->m_nWidth = pSubscribeFuncInfo.m_nWindowWidth;
//            pFuncWindow->m_nHeight = pSubscribeFuncInfo.m_nWindowHeight;
//            pFuncWindow->m_bDisableContextMenu = pSubscribeFuncInfo.m_bDisableContextMenu;
//            pFuncWindow->m_bDisableScrollBar = pSubscribeFuncInfo.m_bDisableScrollBar;
//            pFuncWindow->m_bClosePrompt = pSubscribeFuncInfo.m_bClosePrompt;
//            pFuncWindow->m_bWinResizable = pSubscribeFuncInfo.m_bWinResizable;
//            pFuncWindow->Create(CDlgFuncWindow::IDD,pParent);
//        }
//    }
    else if ( subFuncInfo.m_nFunctionMode==EB_FUNC_MODE_MAINFRAME ) {
        /// 主面板应用；
        CreateFrameList ( false );
        DialogWorkFrame * workFrame = m_pDlgFrameList->showWorkFrame();
        workFrame->addUrl( false, sFullFuncurl, "", subFuncInfo, false );
        m_pDlgFrameList->showFrameList();
    }
    else if ( subFuncInfo.m_nFunctionMode == EB_FUNC_MODE_PROGRAM ) {
        /// 本地程序 ?
        QDesktopServices::openUrl( QUrl(sFullFuncurl) );
    }
    else {
//    else// if (pSubscribeFuncInfo.m_nFunctionMode != EB_FUNC_MODE_MAINFRAME)
        /// 除了主面板，其他都自动打开
        /// program & post data(??)
        QDesktopServices::openUrl( QUrl(sFullFuncurl) );
    }
    return true;
}

void DialogMainFrame::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger: {
        /// 单击事件，显示主界面
        if (this->isMinimized())
            this->showNormal();
        else
            this->onClickedPushButtonSysMin();
//        if (this->isVisible())
//            this->setVisible(false);
//        else
//            this->showNormal();
        break;
    }
    case QSystemTrayIcon::DoubleClick:
        /// 双击事件
        break;
    case QSystemTrayIcon::MiddleClick:
//        m_trayIcon->showMessage("SystemTrayIcon","Hi,This is my trayIcon",QSystemTrayIcon::Information,10000);
        break;
    default:
        break;
    }
}

void DialogMainFrame::createMenuData(void)
{
    if (m_menuSetting == 0) {
        m_menuSetting = new QMenu(this);
        const QString selectText = theLocales.getLocalText("color-skin.select-color.text","选择色调");
        QAction * pSelectColorAction = m_menuSetting->addAction( QIcon(":/res/color_select.bmp"), selectText );
        pSelectColorAction->setCheckable(true);
        pSelectColorAction->setToolTip( theLocales.getLocalText("color-skin.select-color.tooltip","") );
        pSelectColorAction->setData( QVariant((int)0) );
        connect( pSelectColorAction, SIGNAL(triggered()), this, SLOT(onTriggeredActionSelectColor()) );
        m_menuSetting->addSeparator();
        bool bFindMainColorChecked = false;
        const std::vector<EbColorInfo::pointer>& colors = theLocales.colors();
        for (size_t i=0; i<colors.size(); i++) {
            const EbColorInfo::pointer& colorInfo = colors[i];
            QPixmap pixmap(16,16);
            pixmap.fill( colorInfo->color() );
            QAction * pAction = m_menuSetting->addAction( QIcon(pixmap), colorInfo->name() );
            pAction->setCheckable(true);
            pAction->setData( QVariant((int)(i+1)) );
            connect( pAction, SIGNAL(triggered()), this, SLOT(onTriggeredActionSelectColor()) );
            if (!bFindMainColorChecked && colorInfo->color()==theApp->getMainColor()) {
                bFindMainColorChecked = true;
                pAction->setChecked(true);
            }
        }
        if (!bFindMainColorChecked) {
            pSelectColorAction->setChecked(true);
        }
    }
    /// 在线状态菜单
    if (m_menuLineState==0) {
        m_menuLineState = new QMenu(this);
        /// 在线
        QAction * onlineAction = m_menuLineState->addAction( QIcon(":/img/btnstateonline.png"),
                                                             theLocales.getLocalText("line-state.online.text","在线"));
        onlineAction->setCheckable(true);
        onlineAction->setData( QVariant((int)EB_LINE_STATE_ONLINE_NEW) );
        connect( onlineAction, SIGNAL(triggered()), this, SLOT(onClickedMenuLineState()) );
        /// 离开
        QAction * awayAction = m_menuLineState->addAction( QIcon(":/img/btnstateaway.png"),
                                                             theLocales.getLocalText("line-state.away.text","离开"));
        awayAction->setCheckable(true);
        awayAction->setData( QVariant((int)EB_LINE_STATE_AWAY) );
        connect( awayAction, SIGNAL(triggered()), this, SLOT(onClickedMenuLineState()) );
        /// 忙碌
        QAction * busyAction = m_menuLineState->addAction( QIcon(":/img/btnstatebusy.png"),
                                                           theLocales.getLocalText("line-state.busy.text","忙碌"));
        busyAction->setCheckable(true);
        busyAction->setData( QVariant((int)EB_LINE_STATE_BUSY) );
        connect( busyAction, SIGNAL(triggered()), this, SLOT(onClickedMenuLineState()) );
    }
    /// 鼠标右键菜单，托盘菜单
    if (m_menuContext==0) {
        m_menuContext = new QMenu(this);
//        m_menuContext->insertAction();
//        m_menuContext->removeAction();
        /// 打开工作台
        QAction * openFrameListAction = m_menuContext->addAction( theLocales.getLocalText("context-menu.open-frame-list.text","Open Frame"));
        connect( openFrameListAction, SIGNAL(triggered()), this, SLOT(onTriggeredActionOpenWorkFrame()) );
        /// ----------------------
        m_menuContext->addSeparator();
        /// 个人收藏
        m_actionMyCollection = m_menuContext->addAction( theLocales.getLocalText("context-menu.my-collection.text","My Collection"));
        connect( m_actionMyCollection, SIGNAL(triggered()), this, SLOT(onTriggeredActionMyCollection()) );
        /// ----------------------
        m_menuContext->addSeparator();
        /// 注销
        QAction * logoutAction = m_menuContext->addAction( theLocales.getLocalText("context-menu.logout.text","Logout"));
        connect( logoutAction, SIGNAL(triggered()), this, SLOT(onTriggeredActionLogout()) );
        /// 退出
        QAction * quitAction = m_menuContext->addAction( theLocales.getLocalText("context-menu.quit.text","Exit"));
        connect( quitAction, SIGNAL(triggered()), this, SLOT(onTriggeredActionExitApp()) );
    }
//    /// 系统托盘菜单
//    if (m_menuSysTray==0) {
//        m_menuSysTray = new QMenu((QWidget*)QApplication::desktop());
//        /// 注销
//        QAction * logoutAction = m_menuSysTray->addAction( theLocales.getLocalText("context-menu.logout.text","注销"));
//        logoutAction->setData( QVariant((int)EB_COMMAND_LOGOUT) );
//        connect( logoutAction, SIGNAL(triggered()), this, SLOT(onClickedMenuContext()) );
//        /// 退出
//        QAction * quitAction = m_menuSysTray->addAction( theLocales.getLocalText("context-menu.quit.text","退出"));
//        quitAction->setData( QVariant((int)EB_COMMAND_EXIT_APP) );
//        connect( quitAction, SIGNAL(triggered()), this, SLOT(onClickedMenuContext()) );
//   }
}

void DialogMainFrame::creatTrayIcon()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        /// 判断系统是否支持系统托盘图标
        return;
    }
    createMenuData();
    if (m_trayIcon==0 ){
        m_trayIcon = new QSystemTrayIcon(this);
        m_trayIcon->setIcon(QIcon(":/res/mainframe.ico"));
//        m_trayIcon->showMessage("SystemTrayIcon","Hi,This is my trayIcon",QSystemTrayIcon::Information,10000);
        m_trayIcon->setContextMenu(m_menuContext);
        m_trayIcon->show();
        connect(m_trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    }
}

void DialogMainFrame::changeTrayTooltip()
{
    creatTrayIcon();
    if (m_trayIcon==0) {
        /// 可能系统不支持托盘图标
        return;
    }
    const tstring userName = theApp->m_ebum.EB_GetUserName();
    const tstring userAccount = theApp->m_ebum.EB_GetLogonAccount();
    const QString lineStateText = theApp->lineStateText();
    QString sTrayTooltip;
    tstring sEnterpriseName;
    theApp->m_ebum.EB_GetEnterpriseName(sEnterpriseName);
    if (!theApp->productName().isEmpty())
        sEnterpriseName = theApp->productName().toStdString();
    if ( theApp->m_ebum.EB_IsLogonVisitor() ) {
        const QString visitorText = theLocales.getLocalText("name-text.visitor","Visitor");
        sTrayTooltip = QString("%1-%2-%3").arg(visitorText).arg(userAccount.c_str()).arg(lineStateText);
    }
    else if (!sEnterpriseName.empty()) {
        sTrayTooltip = QString("%1(%2)-%3\n%4").arg(userName.c_str()).arg(userAccount.c_str())
                .arg(lineStateText).arg(sEnterpriseName.c_str());
    }else {
        sTrayTooltip = QString("%1(%2)-%3").arg(userName.c_str()).arg(userAccount.c_str()).arg(lineStateText);
    }
    m_trayIcon->setToolTip(sTrayTooltip);
}

void DialogMainFrame::processDatas(void)
{
    /// 创建“setting”菜单数据
    createMenuData();
    creatTrayIcon();
}

void DialogMainFrame::accept()
{
    /// 在这里清除所有列表，避免退出关闭浏览器异常问题；
    if (m_pDlgFrameList!=0) {
        delete m_pDlgFrameList;
        m_pDlgFrameList = 0;
    }
    EbDialogBase::accept();
}

void DialogMainFrame::reject()
{
    if (m_trayIcon==0) {
        /// 系统不支持托盘图标，做最小化处理
        this->onClickedPushButtonSysMin();
    }
    else {
        /// 支持托盘图标，隐藏主界面
        this->onClickedPushButtonSysMin();
//        this->setVisible(false);
    }
}

#define POS_ADIMG_LEFT (const_border_left+198)
#define POS_ADIMG_TOP 6
#define POS_ADIMG_SIGE 30	// 128

#define UISTYLE_CHAT_POS_ADIMG_LEFT 15
#define UISTYLE_CHAT_POS_ADIMG_TOP 35
#define UISTYLE_CHAT_POS_ADIMG_SIGE 64
#define UISTYLE_CHAT_POS_LINESTATE_LEFT	(UISTYLE_CHAT_POS_ADIMG_LEFT+UISTYLE_CHAT_POS_ADIMG_SIGE+3)
#define UISTYLE_CHAT_POS_LINESTATE_TOP	(UISTYLE_CHAT_POS_ADIMG_TOP)
#define UISTYLE_CHAT_POS_STA_FIRST_Y		(UISTYLE_CHAT_POS_ADIMG_TOP+UISTYLE_CHAT_POS_ADIMG_SIGE+10)

void DialogMainFrame::BuildHeadRect(void)
{
    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT)
        m_rectHead = QRect(UISTYLE_CHAT_POS_ADIMG_LEFT, UISTYLE_CHAT_POS_ADIMG_TOP, UISTYLE_CHAT_POS_ADIMG_SIGE, UISTYLE_CHAT_POS_ADIMG_SIGE);
    else
        m_rectHead = QRect(POS_ADIMG_LEFT, POS_ADIMG_TOP, POS_ADIMG_SIGE, POS_ADIMG_SIGE);
}

void DialogMainFrame::addSubUnreadMsg( mycp::bigint subId, bool sendToWorkFrame )
{
    m_widgetMainAppBar->addSubscribeMsg( subId );
    if (sendToWorkFrame && m_pDlgFrameList!=0 ) {
        DialogWorkFrame * workFrame = m_pDlgFrameList->getWorkFrame();
        if ( workFrame!=0 ) {
            workFrame->addSubUnreadMsg(subId);
        }
    }
}

void DialogMainFrame::setSubUnreadMsg( mycp::bigint subId, size_t unreadMsgCount, bool sendToWorkFrame )
{
    m_widgetMainAppBar->setSubscribeMsgCount( subId, unreadMsgCount );
    if (sendToWorkFrame && m_pDlgFrameList!=0 ) {
        DialogWorkFrame * workFrame = m_pDlgFrameList->getWorkFrame();
        if ( workFrame!=0 ) {
            workFrame->setSubUnreadMsg(subId, unreadMsgCount);
        }
    }
}

void DialogMainFrame::onBroadcastMsg(QEvent *e)
{
    const EB_AccountInfo* pAccountInfo = (const EB_AccountInfo*)e;
    const eb::bigint nFromUserid = pAccountInfo->GetUserId();
    const tstring sFromAccount(pAccountInfo->GetAccount());
    const EB_APMsgInfo* pApMsgInfo = (const EB_APMsgInfo*)pAccountInfo->GetEventData();
    const eb::bigint nMsgId = pApMsgInfo->m_nMsgId;
    const int nBCMsgSubType = pApMsgInfo->m_nMsgType;
    const tstring sMsgName(pApMsgInfo->m_sMsgName);
    const tstring sMsgContent(pApMsgInfo->m_sMsgContent);
    switch (nBCMsgSubType) {
    case EB_BROADCAST_SUB_TYPE_NEW_EMAIL: {
            /// 邮件消息通知；
            const tstring theEBSParseString0_from(" ");
            const tstring theEBSParseString0_to("&nbsp;");
            std::vector<tstring> pList;
            if (libEbc::ParseString(sMsgContent.c_str(),theEBSParseString0_from.c_str(),pList)<10) {
                return;
            }
            const eb::bigint nUserId = eb_atoi64(pList[0].c_str());
            const eb::bigint nMailAddressId = eb_atoi64(pList[1].c_str());
            const eb::bigint nMailContentId = eb_atoi64(pList[2].c_str());
            const int nMailSize = atoi(pList[3].c_str());
            const int nAttachCount = atoi(pList[4].c_str());
            tstring sMailDate(pList[5]);
            libEbc::replace(sMailDate,theEBSParseString0_to,theEBSParseString0_from);
            tstring sFromName(pList[6]);
            libEbc::replace(sFromName,theEBSParseString0_to,theEBSParseString0_from);
            const tstring sFromMail(pList[7]);
            tstring sSubject(pList[8]);
            libEbc::replace(sSubject,theEBSParseString0_to,theEBSParseString0_from);
            const tstring sParam(pList[9]);

            if ( m_pDlgMsgTip!=0 ) {
                const eb::bigint nEmailSubId = eb_atoi64(sMsgName.c_str());
                /// 新邮件:%s<%s> %s:\n%s
                const QString newEmailTip = theLocales.getLocalText( "message-show.new-email-tip", "New Email:" );
                const QString msgTip = QString("%1%2<%3> %4:<br>%5").arg(newEmailTip).arg(sFromName.c_str())
                        .arg(sFromMail.c_str()).arg(sMailDate.c_str()).arg(sSubject.c_str());
                m_pDlgMsgTip->addEmailMsgTip(nMailContentId, nEmailSubId, msgTip, sParam.c_str());
                addSubUnreadMsg( nEmailSubId,true );
            }
            return;
        }
    case EB_BROADCAST_SUB_TYPE_UNREAD_EMAIL: {
            if ( m_pDlgMsgTip!=0 ) {
                const int unreadEmailCount = atoi(sMsgContent.c_str());
                const eb::bigint emailSubId = eb_atoi64(sMsgName.c_str());
                /// 你一共有 %s 封未读邮件！\n点击查看！
                QString msgTip = theLocales.getLocalText( "message-show.unread-email-tip", "Unread Email" );
                msgTip.replace( "[%1]", QString::number(unreadEmailCount) );
                m_pDlgMsgTip->addEmailMsgTip( 0, emailSubId, msgTip );
                setSubUnreadMsg( emailSubId, unreadEmailCount, true );
            }
            return;
        }
    case EB_BROADCAST_SUB_TYPE_TW_MSG: {
            /// 应用消息通知；
            //const tstring sMsgTip(sMsgName);
            const tstring theEBSParseString0_from(" ");
            //const tstring theEBSParseString0_to("&nbsp;");
            std::vector<tstring> pList;
            if (libEbc::ParseString(sMsgContent.c_str(),theEBSParseString0_from.c_str(),pList)<2) {
                theApp->m_ebum.EB_AckMsg(nMsgId,5);	/// * 删除无用数据
                return;
            }
            const eb::bigint nId = eb_atoi64(pList[0].c_str());			/// id->planid,taskid,...
            const eb::bigint subId = eb_atoi64(pList[1].c_str());		/// subid
            const tstring sParam = pList.size()>=3?pList[2]:"";
            if ( !sMsgName.empty() && m_pDlgMsgTip!=0 ) {
                //CString sMsgTip;
                //sMsgTip.Format(_T("新消息:%s<%s> %s:\n%s"),sFromName.c_str(),sFromMail.c_str(),sMailDate.c_str(),sSubject.c_str());
                m_pDlgMsgTip->addSubMsgTip(nId, subId, sMsgName.c_str(), sParam.c_str(), nMsgId);
            }
            addSubUnreadMsg(subId,true);
            return;
        }
    case EB_BROADCAST_SUB_TYPE_SUBID_UNREAD_MSG: {
            std::vector<tstring> pList;
            if (libEbc::ParseString(sMsgContent.c_str(),",",pList)<2)
            {
                theApp->m_ebum.EB_AckMsg(nMsgId,5);     /// * 删除无用数据
                return;
            }
            const eb::bigint subId = eb_atoi64(pList[0].c_str());
            const size_t unreadMsg = atoi(pList[1].c_str());
            setSubUnreadMsg( subId,unreadMsg,true );
            return;
        }
    default:
        break;
    }

    /// emp_code=-1,from_type=msg-type
    const int nMsgType = EBC_MSG_TYPE_BC_MSG;
    char sql[1024];
    sprintf( sql, "INSERT INTO call_record_t(call_id,dep_code,emp_code,from_uid,from_account,from_type,dep_name) "
                   "VALUES(%lld,0,-1,%lld,'%s',%d,'%s')",
             pApMsgInfo->m_nMsgId, nFromUserid, sFromAccount.c_str(), nMsgType, sMsgName.c_str() );
    theApp->m_sqliteUser->execute(sql);
//    if (this->m_pDlgMySession != NULL && m_pDlgMySession->GetSafeHwnd())
//    {
//        CCallRecordInfo::pointer pCallRecordInfo = CCallRecordInfo::create();
//        pCallRecordInfo->m_sCallId = pApMsgInfo->m_nMsgId;
//        pCallRecordInfo->m_sGroupName = sMsgName;	// *
//        pCallRecordInfo->m_sGroupCode = 0;
//        pCallRecordInfo->m_sMemberCode = -1;
//        pCallRecordInfo->m_nFromUserId = nFromUserid;
//        pCallRecordInfo->m_sFromAccount = sFromAccount;
//        pCallRecordInfo->m_nFromType = nMsgType;
//        pCallRecordInfo->m_tTime = time(0);
//        pCallRecordInfo->m_bRead = false;
//        m_pDlgMySession->InsertCallRecord(pCallRecordInfo,true);
//    }

//    //CString sContent;
//    //if (theApp.GetGroupMsgSugId()>0)
//    //	sContent.Format(_T("%s<br/><a href=\"eb-open-subid://%lld,tab_type=bc_msg\">查看我的消息</a>"),sMsgContent.c_str(),theApp.GetGroupMsgSugId());
//    //if (theApp.GetGroupMsgSugId()>0)
//    //	m_pDlgBroadcastMsg->UpdateBroadcastMsg(sMsgName,libEbc::ACP2UTF8(sContent),nMsgId);
//    //else
//    //	m_pDlgBroadcastMsg->UpdateBroadcastMsg(sMsgName,libEbc::ACP2UTF8(sMsgContent.c_str()),nMsgId);
//    if (m_pDlgBroadcastMsg->GetShowMyMsgSubId()==0 && theApp.GetGroupMsgSugId()>0)
//    {
//        m_pDlgBroadcastMsg->SetShowMyMsgSubId(theApp.GetGroupMsgSugId(),"tab_type=bc_msg");
//    }
//    m_pDlgBroadcastMsg->UpdateBroadcastMsg(sMsgName,libEbc::ACP2UTF8(sMsgContent.c_str()),nMsgId);
//    m_pDlgBroadcastMsg->ShowWindow(SW_SHOW);

}

void DialogMainFrame::onAreaInfo(QEvent *e)
{
    const EB_AreaInfo* pAreaInfo = (const EB_AreaInfo*)e;
    const int nParameter = (int)pAreaInfo->GetEventParameter();
    switch (nParameter) {
    case 1:
    case 2:
    case 3:
    case 4: {
        if (m_pDlgMyCenter!=0) {
            m_pDlgMyCenter->onAreaInfo(pAreaInfo,nParameter);
        }
        break;
    }
    default:
        break;
    }
}

void DialogMainFrame::onUserStateChange(QEvent *e)
{
    const EB_MemberInfo * pEvent = (EB_MemberInfo*)e;
//    const bool bIsOwnerMember = pEvent->GetEventParameter()==1?true:false;

    /// 更新界面用户状况改变
//    if (pMemberInfo != NULL && m_pDlgMyEnterprise != NULL)
//    {
//        m_pDlgMyEnterprise->EmployeeInfo(pMemberInfo,true);
//        //m_pDlgMyEnterprise->ChangeEmployeeInfo(pMemberInfo);
//    }
    if ( m_widgetMyGroup!=0 ) {
        m_widgetMyGroup->onMemberInfo(pEvent,true);
    }
    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
        /// **跑下面
    }
    else if (!theApp->isHideMainFrame()) {
//        CFrameWndInfoProxy::UserLineStateChange(pMemberInfo->m_sGroupCode, pMemberInfo->m_nMemberUserId, pMemberInfo->m_nLineState);
//        return 0;
    }
    if (m_pDlgFrameList!=0) {
        m_pDlgFrameList->onUserLineStateChange(pEvent->m_sGroupCode, pEvent->m_nMemberUserId, pEvent->m_nLineState);
    }
}

void DialogMainFrame::onMemberHeadChange(QEvent *e)
{
    const EB_MemberInfo * pMemberInfo = (const EB_MemberInfo*)e;
    const bool bIsOwnerMember = (pMemberInfo->GetEventParameter()==1)?true:false;

    // 更新聊天会话列表图标
    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
        if (m_pDlgFrameList!=0) {
            m_pDlgFrameList->onMemberHeadChange(pMemberInfo);
        }
    }
    else if (!theApp->isHideMainFrame()) {
//        CFrameWndInfoProxy::MemberHeadChange(pMemberInfo);
    }
    /// 更新界面用户状况改变
//    if (m_pDlgMyEnterprise!=0) {
//        m_pDlgMyEnterprise->EmployeeInfo(pMemberInfo,false);
//    }
    if (m_widgetMyGroup!=0) {
        m_widgetMyGroup->onMemberInfo(pMemberInfo,false);
    }
    if (bIsOwnerMember) {
        eb::bigint nDefaultMemberCode = 0;
        if (theApp->m_ebum.EB_GetMyDefaultMemberCode(nDefaultMemberCode)
                && nDefaultMemberCode==pMemberInfo->m_sMemberCode) {
            /// 更新头像
            const QString userHeadFile = QString::fromStdString(pMemberInfo->m_sHeadResourceFile.string());
            if ( !userHeadFile.isEmpty() && QFile::exists(userHeadFile)) {
                m_labelUserImage->setPixmap( QPixmap(userHeadFile).scaled(m_rectHead.width(),m_rectHead.height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation) );
            }
        }
    }
}

void DialogMainFrame::onContactHeadChange(QEvent *e)
{
    const EB_ContactInfo* pContactInfo = (const EB_ContactInfo*)e;
//    if (m_pDlgMyContacts!=NULL)
//    {
//        m_pDlgMyContacts->ContactHeadChange(pContactInfo);
//    }
    if (pContactInfo->m_nContactUserId>0) {
        const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
        if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
            if (m_pDlgFrameList!=0) {
                m_pDlgFrameList->onContactHeadChange(pContactInfo);
            }
        }
        else if (!theApp->isHideMainFrame()) {
//            CFrameWndInfoProxy::UserHeadChange(pContactInfo);
        }
    }
}

void DialogMainFrame::onMsgReceipt(QEvent *e)
{
    const CCrRichInfo * pCrMsgInfo = (const CCrRichInfo*)e;
    const eb::bigint sCallId = pCrMsgInfo->GetCallId();
    /// 0:成功 4:撤回消息，6:个人收藏，7:群收藏
    const int nAckType = (int)pCrMsgInfo->GetEventParameter();

    //const eb::bigint nFromUserId = pCrMsgInfo->m_sSendFrom;
    //const eb::bigint nMsgId = pCrMsgInfo->m_pRichMsg->GetMsgId();

    //EbcCallInfo::pointer pEbCallInfo;
    //if (!theApp.m_pCallList.find(sCallId,pEbCallInfo))
    //{
    //	return 1;
    //}
    DialogChatBase::pointer pDlgDialog = getDialogChatBase(sCallId);
    if (pDlgDialog.get()==0) {
        const eb::bigint nFromUserId = pCrMsgInfo->m_sSendFrom;
        const eb::bigint nMsgId = pCrMsgInfo->m_pRichMsg->GetMsgId();
        if (pCrMsgInfo->GetStateCode()==EB_STATE_OK) {
            theApp->updateMsgReceiptData(nMsgId, nFromUserId, nAckType);
        }
    }
    else {
        pDlgDialog->onMsgReceipt(pCrMsgInfo, nAckType);
    }
}

void DialogMainFrame::onSendRich(QEvent *e)
{
    const CCrRichInfo * pCrMsgInfo = (const CCrRichInfo*)e;
    const EB_STATE_CODE nState = pCrMsgInfo->GetStateCode();
    if (EB_STATE_APPID_KEY_ERROR==nState || EB_STATE_APP_ONLINE_KEY_TIMEOUT==nState)
        return; // 0;

    const eb::bigint sCallId = pCrMsgInfo->GetCallId();
    EbcCallInfo::pointer pEbCallInfo;
    if (!theApp->m_pCallList.find(sCallId,pEbCallInfo)) {
        return; // 1;
    }
    pEbCallInfo->m_tLastTime = time(0);
    DialogChatBase::pointer pDlgDialog = getDialogChatBase(sCallId);
    if (pDlgDialog.get()==NULL) {
        return; // 1;
    }
    pDlgDialog->onSendRich(pCrMsgInfo,nState);
    if (m_pDlgMsgTip!=0) {
        m_pDlgMsgTip->delMsgTip(pEbCallInfo->m_pCallInfo.m_sGroupCode,pEbCallInfo->m_pFromAccountInfo.GetUserId());
    }
}

bool DialogMainFrame::onReceiveRich(QEvent *e)
{
    const CCrRichInfo * pCrMsgInfo = (const CCrRichInfo*)e;
    const eb::bigint nFromUserid = pCrMsgInfo->m_sSendFrom;
    const eb::bigint sCallId = pCrMsgInfo->GetCallId();
    EbcCallInfo::pointer pEbCallInfo;
    if (!theApp->m_pCallList.find(sCallId,pEbCallInfo)) {
        return false;
    }
    pEbCallInfo->m_tLastTime = time(0);
    DialogChatBase::pointer pDlgDialog = getDialogChatBase(pEbCallInfo,false);
    QString sFirstMsg1;
    QString sFirstMsg2;
    pDlgDialog->onReceiveRich(pCrMsgInfo,&sFirstMsg1,&sFirstMsg2);
    const eb::bigint nMsgId = pCrMsgInfo->m_pRichMsg->GetMsgId();
    //CEBCMsgInfo::pointer pEbcMsgInfo = CEBCMsgInfo::create(CEBCMsgInfo::MSG_TYPE_RECEIVE_RICH);
    //pEbcMsgInfo->m_pEbCallInfo = pEbCallInfo;
    //m_pEbcMsgList.add(pEbcMsgInfo);

    if (!pDlgDialog->isVisible()) {
        QSound::play( ":/wav/msg.wav" );

        bool bRet = false;
        const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
        if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT && m_pDlgFrameList!=0) {
            bRet = m_pDlgFrameList->addUnreadMsg(sCallId,nMsgId);
        }
        else if (!theApp->isHideMainFrame()) {
//            const bool bRet = CFrameWndInfoProxy::AddUnreadMsg(sCallId, nMsgId);
//            if (bRet && this->IsWindowVisible())
//            {
//                ::FlashWindow(this->GetSafeHwnd(), TRUE);
//                return 0;
//            }
        }
        else if (m_pDlgFrameList!=0) {
            bRet = m_pDlgFrameList->addUnreadMsg(sCallId,nMsgId);
        }

        if (bRet && m_pDlgFrameList!=0 && m_pDlgFrameList->isVisible()) {
//            ::FlashWindow(m_pDlgFrameList->GetSafeHwnd(), TRUE);
            return true;
        }
        //m_btnMySession.SetWindowText(_T("！"));
        if (m_pDlgMsgTip!=0) {
//            pDlgDialog->formImage()
            m_pDlgMsgTip->addMsgTip(pDlgDialog->fromImage(), pEbCallInfo->m_pCallInfo.m_sGroupCode,nFromUserid,sFirstMsg2);
        }
    }
    return true;
}

void DialogMainFrame::onSendingFile(QEvent *e)
{
    const CCrFileInfo * pCrFileInfo = (const CCrFileInfo*)e;
    const EB_STATE_CODE state = (EB_STATE_CODE)pCrFileInfo->GetStateCode();
    if (EB_STATE_APPID_KEY_ERROR==state || EB_STATE_APP_ONLINE_KEY_TIMEOUT==state) {
        return;
    }
    switch (state) {
    case EB_STATE_GROUP_FORBID_SPEECH: {
        /// 群禁言中：\r\n不能发送群文件！
        QString text = theLocales.getLocalText("on-send-file.group-forbid-speech.text","Group Forbid Speech");
        text.replace( "[STATE_CODE]", QString::number((int)state) );
        EbMessageBox::doShow( NULL, "", QChar::Null, text, EbMessageBox::IMAGE_WARNING,default_warning_auto_close );
        return;
    }
    case EB_STATE_FORBIG_SPEECH: {
        /// 你被禁言中：<br>不能发送群文件！
        QString text = theLocales.getLocalText("on-send-file.forbid-speech.text","Forbid Speech");
        text.replace( "[STATE_CODE]", QString::number((int)state) );
        EbMessageBox::doShow( NULL, "", QChar::Null, text, EbMessageBox::IMAGE_WARNING,default_warning_auto_close );
        return;
    }
    case EB_STATE_FILE_ALREADY_EXIST:
            //return 0;	/// 不能返回，后面处理
    case EB_STATE_OK:
    case EB_STATE_WAITING_PROCESS:	/// ** 等待处理
            break;
    case EB_STATE_NOT_AUTH_ERROR: {
        /// 没有权限：\r\n请联系管理员！
        QString text = theLocales.getLocalText("on-send-file.not-auth-error.text","Not Auth Error");
        text.replace( "[STATE_CODE]", QString::number((int)state) );
        EbMessageBox::doShow( NULL, "", QChar::Null, text, EbMessageBox::IMAGE_WARNING,default_warning_auto_close );
        return;
    }
    case EB_STATE_EXCESS_QUOTA_ERROR:
    case EB_STATE_FILE_BIG_LONG: {
        /// 当前文件大小超出系统最大配额：\r\n请发送更小文件，或联系公司客服！
        QString text = theLocales.getLocalText("on-send-file.file-big-long.text","File Big Long");
        text.replace( "[STATE_CODE]", QString::number((int)state) );
        EbMessageBox::doShow( NULL, "", QChar::Null, text, EbMessageBox::IMAGE_WARNING,default_warning_auto_close );
        return;
    }
    case EB_STATE_TIMEOUT_ERROR: {
        /// 文件发送超时：\r\n请重新发送！
        QString text = theLocales.getLocalText("on-send-file.timeout-error.text","Timeout Error");
        text.replace( "[STATE_CODE]", QString::number((int)state) );
        EbMessageBox::doShow( NULL, "", QChar::Null, text, EbMessageBox::IMAGE_WARNING,default_warning_auto_close );
        return;
    }
    case EB_STATE_NOT_MEMBER_ERROR: {
        /// 没有其他群组成员：\r\n不能发送文件！
        QString text = theLocales.getLocalText("on-send-file.not-member-error.text","Not Member Error");
        text.replace( "[STATE_CODE]", QString::number((int)state) );
        EbMessageBox::doShow( NULL, "", QChar::Null, text, EbMessageBox::IMAGE_WARNING,default_warning_auto_close );
        return;
    }
    default: {
        /// 文件发送失败，错误代码：%d\r\n请检查后重试！
        QString text = theLocales.getLocalText("on-send-file.other-error.text","Send File Error");
        text.replace( "[STATE_CODE]", QString::number((int)state) );
        EbMessageBox::doShow( NULL, "", QChar::Null, text, EbMessageBox::IMAGE_WARNING,default_warning_auto_close );
        return;
    }
    }

    const eb::bigint sCallId = pCrFileInfo->GetCallId();
    const eb::bigint sResId = pCrFileInfo->m_sResId;
    if (sResId>0 && sCallId==0) {
//            ShowDlgFileManager();
//            m_pDlgFileManager->OnSendingFile(pCrFileInfo);
//            m_pDlgFileManager->OnBnClickedButtonTraning();
    }
    EbcCallInfo::pointer pEbCallInfo;
    if (!theApp->m_pCallList.find(sCallId,pEbCallInfo)) {
        return;
    }
    pEbCallInfo->m_tLastTime = time(0);
    DialogChatBase::pointer chatBase = getDialogChatBase(pEbCallInfo);
    chatBase->onSendingFile(pCrFileInfo);
}

void DialogMainFrame::onSentFile(QEvent *e)
{
    const CCrFileInfo * pCrFileInfo = (const CCrFileInfo*)e;
    const EB_STATE_CODE nState = (EB_STATE_CODE)pCrFileInfo->GetStateCode();
    if (EB_STATE_APPID_KEY_ERROR==nState || EB_STATE_APP_ONLINE_KEY_TIMEOUT==nState) {
        return;
    }

    const eb::bigint sCallId = pCrFileInfo->GetCallId();
    const eb::bigint sResId = pCrFileInfo->m_sResId;
    if (sResId>0 && sCallId==0) {
//        if (m_pDlgFileManager!=NULL && (pCrFileInfo->m_sReceiveAccount==0 || pCrFileInfo->m_sReceiveAccount==theEBAppClient.EB_GetLogonUserId()))
//        {
//            m_pDlgFileManager->DeleteDlgTranFile(pCrFileInfo->m_nMsgId);
//            if (m_pDlgFileManager->IsEmpty())
//            {
//                m_pDlgFileManager->ShowWindow(SW_HIDE);
//            }
//        }
    }
    //EbcCallInfo::pointer pEbCallInfo;
    //if (!theApp.m_pCallList.find(lpszCallId,pEbCallInfo))
    //{
    //	return 1;
    //}
    //pEbCallInfo->m_tLastTime = time(0);
    if (pCrFileInfo->m_sReceiveAccount>0) {
        DialogChatBase::pointer chatBase = getDialogChatBase(sCallId);
        if (chatBase.get()!=0) {
            if (chatBase->onSentFile(pCrFileInfo, nState) && !chatBase->isVisible()) {
                QSound::play( ":/wav/msg.wav" );
            }
        }
        else {
            if ( pCrFileInfo->m_sReceiveAccount!=theApp->logonUserId() ) {
                /// * 对方接收消息回执
                const eb::bigint nFromUserId = pCrFileInfo->m_sReceiveAccount;
                const eb::bigint nMsgId = pCrFileInfo->m_nMsgId;
                theApp->updateMsgReceiptData(nMsgId, nFromUserId, 0);
            }
        }
    }

}

void DialogMainFrame::onCancelFile(QEvent *e)
{
    const CCrFileInfo * pCrFileInfo = (const CCrFileInfo*)e;
    const int lParam = (int)pCrFileInfo->GetEventParameter();
    const bool bChangeP2PSending = lParam==1?true:false;
    const eb::bigint sCallId = pCrFileInfo->GetCallId();
    const eb::bigint sResId = pCrFileInfo->m_sResId;
    const eb::bigint nReceiveAccount = pCrFileInfo->m_sReceiveAccount;
    //const eb::bigint nSendToUid = pCrFileInfo->m_sSendTo;
    if (sResId>0 && sCallId==0) {
//        if (m_pDlgFileManager!=NULL && nReceiveAccount==0)
//            m_pDlgFileManager->DeleteDlgTranFile(pCrFileInfo->m_nMsgId);

//        if (sResId==pCrFileInfo->m_nMsgId)
//        {
//            delete pCrFileInfo;
//            return 0;
//        }
    }
    //EbcCallInfo::pointer pEbCallInfo;
    //if (!theApp.m_pCallList.find(lpszCallId,pEbCallInfo))
    //{
    //	return 1;
    //}
    //pEbCallInfo->m_tLastTime = time(0);
    DialogChatBase::pointer chatBase = getDialogChatBase(sCallId);
    if (chatBase.get()!=0) {
        chatBase->onCancelFile(pCrFileInfo,bChangeP2PSending);
        if (!chatBase->isVisible()) {
            if (lParam!=10) {
                QSound::play( ":/wav/msg.wav" );
            }
        }
    }
    const tstring sFileName = libEbc::GetFileName(pCrFileInfo->m_sFileName);

    EbcCallInfo::pointer pEbCallInfo;
    if (nReceiveAccount!=0 && theApp->m_pCallList.find(sCallId,pEbCallInfo))	/// sReceiveAccount==0是主动取消
    {
//        if (chatBase.get()==0 || !chatBase->IsWindowVisible())
//        {
//            CString sFirstMsg;
//            if (pEbCallInfo->m_pCallInfo.m_sGroupCode>0)
//            {
//                sFirstMsg.Format(_T("用户取消发送文件：%s"),sFileName.c_str());
//                const CString sTemp(libEbc::ACP2UTF8(sFirstMsg).c_str());
//                sFirstMsg.Format(_T("<font color=\"#6c6c6c\">%s</font><br/><a href=\"ebim-call-group://%lld\">%s</a>"),sTemp,pEbCallInfo->m_pCallInfo.m_sGroupCode,libEbc::ACP2UTF8("查看聊天内容").c_str());
//            }else
//            {
//                sFirstMsg.Format(_T("用户[%s]取消发送文件：%s"),pEbCallInfo->m_pCallInfo.GetFromAccount().c_str(),sFileName.c_str());
//                const CString sTemp(libEbc::ACP2UTF8(sFirstMsg).c_str());
//                sFirstMsg.Format(_T("<font color=\"#6c6c6c\">%s</font><br/><a href=\"ebim-call-account://%lld\">%s</a>"),sTemp,pEbCallInfo->m_pCallInfo.GetFromUserId(),libEbc::ACP2UTF8("查看聊天内容").c_str());
//            }

//            /// 显示消息
//            CWnd * pParent = CWnd::FromHandle(::GetDesktopWindow());
//            CDlgFuncWindow * pFuncWindow = new CDlgFuncWindow(pParent,true);
//            pFuncWindow->m_pEbCallInfo = pEbCallInfo;
//            pFuncWindow->m_bDisableContextMenu = true;
//            pFuncWindow->m_bBroadcastMsg = true;
//            pFuncWindow->m_bOpenNewClose = true;
//            pFuncWindow->m_sTitle = _T("文件消息提醒");
//            pFuncWindow->m_sFuncUrl = (LPCTSTR)sFirstMsg;
//            pFuncWindow->m_nWidth = 250;
//            pFuncWindow->m_nHeight = 180;
//            CEBCMsgInfo::pointer pEbcMsgInfo = CEBCMsgInfo::create(CEBCMsgInfo::MSG_TYPE_CREATE_FUNC_WINDOW);
//            pEbcMsgInfo->m_pFuncWindow = pFuncWindow;
//            pEbcMsgInfo->m_pParent = pParent;
//            m_pEbcMsgList.add(pEbcMsgInfo);
//        }
    }
}

bool DialogMainFrame::onReceivingFile(QEvent *e)
{
    const CCrFileInfo * fileInfo = (const CCrFileInfo*)e;
    const eb::bigint sResId = fileInfo->m_sResId;
    const int lParam = (int)fileInfo->GetEventParameter();
    if (sResId>0 && fileInfo->GetCallId()==0) {
    //if (sResId>0 && (!pCrFileInfo->m_bOffFile || sResId==pCrFileInfo->m_nMsgId))
//        ShowDlgFileManager();
//        m_pDlgFileManager->OnReceivingFile(pCrFileInfo);
//        m_pDlgFileManager->OnBnClickedButtonTraning();
        if (sResId==fileInfo->m_nMsgId) {
//            delete pCrFileInfo;
            return true;
        }
    }
    const eb::bigint sCallId = fileInfo->GetCallId();
    EbcCallInfo::pointer pEbCallInfo;
    if (!theApp->m_pCallList.find(sCallId,pEbCallInfo))
    {
        return false;// -1;
    }
    pEbCallInfo->m_tLastTime = time(0);
    DialogChatBase::pointer chatBase = getDialogChatBase(pEbCallInfo,false);
    QString sFirstMsg;
    chatBase->onReceivingFile(fileInfo,&sFirstMsg);

    if (!chatBase->isVisible()) {
        if (lParam!=10) {
            QSound::play( ":/wav/msg.wav" );
        }

        bool bRet = false;
        const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
        if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT && m_pDlgFrameList!=0) {
            bRet = m_pDlgFrameList->addUnreadMsg(sCallId,0);
        }
        else if (!theApp->isHideMainFrame()) {
//            const bool bRet = CFrameWndInfoProxy::AddUnreadMsg(sCallId,0);
//            if (bRet && this->IsWindowVisible())
//            {
//                ::FlashWindow(this->GetSafeHwnd(), TRUE);
//                return 0;
//            }
        }
        else if (m_pDlgFrameList!=0) {
            bRet = m_pDlgFrameList->addUnreadMsg(sCallId,0);
        }
        if ( bRet && m_pDlgFrameList!=0 && m_pDlgFrameList->isVisible() ) {
//            ::FlashWindow(m_pDlgFrameList->GetSafeHwnd(), TRUE);
            return true;
        }

        /// 显示消息
//        CWnd * pParent = CWnd::FromHandle(::GetDesktopWindow());
//        CDlgFuncWindow * pFuncWindow = new CDlgFuncWindow(pParent,true);
//        pFuncWindow->m_pEbCallInfo = pEbCallInfo;
//        pFuncWindow->m_bDisableContextMenu = true;
//        pFuncWindow->m_bBroadcastMsg = true;
//        pFuncWindow->m_bOpenNewClose = true;
//        pFuncWindow->m_sTitle = _T("文件消息提醒");
//        pFuncWindow->m_sFuncUrl = (LPCTSTR)sFirstMsg;
//        pFuncWindow->m_nWidth = 250;
//        pFuncWindow->m_nHeight = 180;
//        CEBCMsgInfo::pointer pEbcMsgInfo = CEBCMsgInfo::create(CEBCMsgInfo::MSG_TYPE_CREATE_FUNC_WINDOW);
//        pEbcMsgInfo->m_pFuncWindow = pFuncWindow;
//        pEbcMsgInfo->m_pParent = pParent;
//        m_pEbcMsgList.add(pEbcMsgInfo);
    }
    return true;
}

void DialogMainFrame::onReceivedFile(QEvent *e)
{
    const CCrFileInfo * fileInfo = (const CCrFileInfo*)e;
    const int lParam = (int)fileInfo->GetEventParameter();
    const eb::bigint sCallId = fileInfo->GetCallId();
    const eb::bigint sResId = fileInfo->m_sResId;
    if (sResId>0 && fileInfo->GetCallId()==0) {
    //if (sResId>0)
        const bool bIsHttpDownloadFile = sResId==fileInfo->m_nMsgId?true:false;
//        if (m_pDlgFileManager!=NULL)
//        {
//            m_pDlgFileManager->OnReceivedFile(pCrFileInfo);
//            if (!bIsHttpDownloadFile && m_pDlgFileManager->IsEmpty())
//            {
//                m_pDlgFileManager->ShowWindow(SW_HIDE);
//            }
//        }
//        if (bIsHttpDownloadFile)
//        {
//            tstring sInFileName(pCrFileInfo->m_sFileName);
//            theApp.m_pBoUsers->escape_string_in(sInFileName);
//            CString sSql;
//            sSql.Format(_T("INSERT INTO msg_record_t(msg_id,from_uid,from_name,to_uid,msg_type,msg_text) ")\
//                _T("VALUES(%lld,%lld,'',%lld,%d,'%s')"),
//                pCrFileInfo->m_nMsgId,pCrFileInfo->m_sSendFrom,
//                pCrFileInfo->m_sSendTo,MRT_FILE,libEbc::ACP2UTF8(sInFileName.c_str()).c_str());
//            theApp.m_pBoUsers->execute(sSql);
//            delete pCrFileInfo;
//            if (m_pDlgFileManager->IsWindowVisible() && m_pDlgFileManager->IsEmpty())
//            {
//                m_pDlgFileManager->OnBnClickedButtonRefresh();
//                m_pDlgFileManager->OnBnClickedButtonTraned();
//            }
//            m_pDlgFileManager->FlashWindow(TRUE);
//            return 0;
//        }
    }
    //CEBCCallInfo::pointer pEbCallInfo;
    //if (!theApp.m_pCallList.find(lpszCallId,pEbCallInfo))
    //{
    //	return 1;
    //}
    //pEbCallInfo->m_tLastTime = time(0);
    DialogChatBase::pointer chatBase = getDialogChatBase(sCallId);
    if (chatBase.get()!=0) {
        chatBase->onReceivedFile(fileInfo);
        if (!chatBase->isVisible()) {
            if (lParam!=10) {
                QSound::play( ":/wav/msg.wav" );
            }
        }
    }
}

void DialogMainFrame::onFilePercent(QEvent *e)
{
    const CChatRoomFilePercent * pChatRoomFilePercent = (const CChatRoomFilePercent*)e;
    const eb::bigint sCallId = pChatRoomFilePercent->GetCallId();
    const eb::bigint sResId = pChatRoomFilePercent->m_sResId;
    if (sResId>0 && sCallId==0) {
    //if (sResId>0)
//        if (m_pDlgFileManager!=NULL)
//            m_pDlgFileManager->SetFilePercent(pChatRoomFilePercent);
        if (sResId==pChatRoomFilePercent->m_nMsgId) {
//            delete pChatRoomFilePercent;
            return;
        }
    }
    DialogChatBase::pointer chatBase = getDialogChatBase(sCallId);
    if (chatBase.get()!=0) {
        chatBase->onFilePercent(pChatRoomFilePercent);
    }
}

void DialogMainFrame::CreateFrameList(bool bShow)
{
    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
        /// **会跑下面
    }
    else if (!theApp->isHideMainFrame()) {
        if (bShow) {
            if ( !this->isVisible() ) {
                this->setVisible(true);
//                if (m_bShrinkageWin && m_pDlgShrinkageBar->IsWindowVisible())
//                {
//                    SetTimer(TIMERID_CHECK_SHRINKAGEBAR,200,NULL);
//                }
            }
        }
        return;
    }
    if (m_pDlgFrameList==0) {
        m_pDlgFrameList = new DialogFrameList;
        m_pDlgFrameList->setVisible(false);
        m_pDlgFrameList->setModal(false);
        m_pDlgFrameList->setWindowModality(Qt::WindowModal);
//        m_pDlgFrameList->SetWorkFrameCallback(this);
    }
    if (bShow) {
        m_pDlgFrameList->showFrameList();
    }
}

DialogChatBase::pointer DialogMainFrame::getDialogChatBase(const EbcCallInfo::pointer &pEbCallInfo, bool bShow, bool bOwnerCall)
{
    const bool bAutoCall = (pEbCallInfo->m_pCallInfo.m_sGroupCode>0)?
                theApp->m_pAutoCallGroupIdList.exist(pEbCallInfo->m_pCallInfo.m_sGroupCode):
                theApp->m_pAutoCallFromUserIdList.exist(pEbCallInfo->m_pFromAccountInfo.GetUserId());
    CreateFrameList(false);
//    CreateFrameList(bShow);
    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    const eb::bigint sCallId = pEbCallInfo->m_pCallInfo.GetCallId();
    DialogChatBase::pointer pDlgDialog;
    if (pEbCallInfo->m_pCallInfo.m_sOldCallId>0) {
        if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
            pDlgDialog = m_pDlgFrameList->getDialogChatBase(pEbCallInfo->m_pCallInfo.m_sOldCallId,false,bAutoCall);
        }
        else if ( !theApp->isHideMainFrame() ) {
//            pDlgDialog = CFrameWndInfoProxy::getDialogChatBase(pEbCallInfo->m_pCallInfo.m_sOldCallId,false,bAutoCall);
        }
        else {
            pDlgDialog = m_pDlgFrameList->getDialogChatBase(pEbCallInfo->m_pCallInfo.m_sOldCallId,false,bAutoCall);
        }
        if (pDlgDialog.get()!=0) {
            // 找到old call id
            pDlgDialog->setCallInfo(pEbCallInfo);
        }
    }
    if (pDlgDialog.get()==0) {
        if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
            pDlgDialog = m_pDlgFrameList->getDialogChatBase(sCallId,false,bAutoCall);
        }
        else if (!theApp->isHideMainFrame()) {
//            pDlgDialog = CFrameWndInfoProxy::getDialogChatBase(sCallId,false,bAutoCall);
        }
        else {
            pDlgDialog = m_pDlgFrameList->getDialogChatBase(sCallId,false,bAutoCall);
        }

        if (pDlgDialog.get()==0) {
            //if (!this->IsIconic())
            {
                if (nDefaultUIStyleType!=EB_UI_STYLE_TYPE_CHAT) {
//                    const int nLimitWidth = const_border_left+DEFAULT_P2P_WIDTH;
//                    CRect rect;
//                    this->GetWindowRect(&rect);
//                    if (rect.Width()< nLimitWidth) {
//                        if (m_rectAdjustOld.Width()==0)
//                            m_rectAdjustOld = rect;
//                        rect.right = rect.left + nLimitWidth;
//                        this->MoveWindow(&rect);
//                    }
                }
            }
            QWidget * pParent = (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT||theApp->isHideMainFrame())?m_pDlgFrameList:(QWidget*)this;
            pDlgDialog = DialogChatBase::create(pEbCallInfo,pParent);
            pDlgDialog->setVisible(false);
            pDlgDialog->setModal(false);
            pDlgDialog->setOwnerCall(bOwnerCall);
            EbFrameItem::pointer frameItem = EbFrameItem::create(pDlgDialog);
            if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
                m_pDlgFrameList->addFrameItem( frameItem,bShow,bAutoCall );
            }
            else if (!theApp->isHideMainFrame()) {
//                CFrameWndInfoProxy::AddWnd(pFrameWndInfo, bShow,bAutoCall);
            }
            else {
                m_pDlgFrameList->addFrameItem( frameItem,bShow, bAutoCall );
            }
        }
        else if (pEbCallInfo->m_pCallInfo.m_sOldCallId!=pDlgDialog->callInfo()->m_pCallInfo.m_sOldCallId) {
            // 更新old call id
            pDlgDialog->setCallInfo(pEbCallInfo);
        }
    }

    if (bShow && !bAutoCall)
    {
        if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
            m_pDlgFrameList->showFrameItem(pEbCallInfo->m_pCallInfo.GetCallId());
        }
        else if ( !theApp->isHideMainFrame() ) {
//            CFrameWndInfoProxy::ShowWnd(pEbCallInfo->m_pCallInfo.GetCallId());
//            if (bOwnerCall)
//            {
//                if (this->IsIconic())
//                    this->ShowWindow(SW_RESTORE);
//                else if (!this->IsWindowVisible())
//                    this->ShowWindow(SW_SHOW);
//                this->SetForegroundWindow();
//            }
        }
        else {
            m_pDlgFrameList->showFrameItem(pEbCallInfo->m_pCallInfo.GetCallId());
        }

//        // **不能添加，否则会发生异常；
//        //CDlgFuncWindow * pOldFuncWindow = NULL;
//        //if (pEbCallInfo->m_pCallInfo.m_sGroupCode>0)
//        //{
//        //	theApp.m_pTempGroupMsg.find(pEbCallInfo->m_pCallInfo.m_sGroupCode,pOldFuncWindow,true);
//        //}else
//        //{
//        //	theApp.m_pTempFromUserMsg.find(pEbCallInfo->m_pFromAccountInfo.GetUserId(),pOldFuncWindow,true);
//        //}
//        //// **必须放在前面
//        //if (pOldFuncWindow!=NULL)
//        //{
//        //	pOldFuncWindow->ShowWindow(SW_HIDE);	// *必须隐藏不让用户点击；
//        //	pOldFuncWindow->m_pEbCallInfo.reset();	// *必须设为空，否则会清掉后面窗口
//        //	theApp.m_pCloseWnd.add(pOldFuncWindow);
//        //	//pOldFuncWindow->DestroyWindow();
//        //}
    }

    if (bShow) {
        CreateFrameList(true);
    }
    return pDlgDialog;
}

DialogChatBase::pointer DialogMainFrame::getDialogChatBase(eb::bigint nCallId, bool bRemove)
{
//    if (theWantExitApp)
//    {
//        return NullDlgDialog;	// 预防死锁
//    }
    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();
    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
        /// **跑下面
    }
    else if (!theApp->isHideMainFrame()) {
//        return CFrameWndInfoProxy::getDialogChatBase(nCallId, bRemove, false);
    }
    return (m_pDlgFrameList!=0)?m_pDlgFrameList->getDialogChatBase(nCallId, bRemove, false):DialogChatBaseNull;
}

void DialogMainFrame::onCallConnected(QEvent *e)
{
//    sndPlaySound(NULL,SND_NODEFAULT);
    const EB_CallInfo* pConnectInfo = (const EB_CallInfo*)e;
    const int nConnectFlag = (int)pConnectInfo->GetEventParameter();
    const eb::bigint sCallId = pConnectInfo->GetCallId();
    EB_AccountInfo pFromAccountInfo;
    if ( !theApp->m_ebum.EB_GetCallAccountInfo(sCallId,pConnectInfo->GetFromUserId(),&pFromAccountInfo) ) {
        return; // 1;
    }
    const EB_UI_STYLE_TYPE nDefaultUIStyleType = theApp->defaultUIStyleType();

    if (pConnectInfo->m_sGroupCode==0 || pConnectInfo->m_sGroupCode==sCallId) {// ?? || !pCallInfo->m_sChatId.empty())
        const bool bOffLineUser = (nConnectFlag&EB_CONNECTED_OFFLINE_USER)==EB_CONNECTED_OFFLINE_USER;
        const bool bOwnerCall = (nConnectFlag&EB_CONNECTED_OWNER_CALL)==EB_CONNECTED_OWNER_CALL;
        const bool bConnectMsg = (nConnectFlag&EB_CONNECTED_MSG)==EB_CONNECTED_MSG;
        bool bNewCall = false;
        EbcCallInfo::pointer pEbCallInfo;
        if ( !theApp->m_pCallList.find(sCallId,pEbCallInfo) ) {
            bNewCall = true;
            pEbCallInfo = EbcCallInfo::create();
            pEbCallInfo->m_bOffLineUser = bOffLineUser;
            pEbCallInfo->m_pCallInfo = pConnectInfo;
            pEbCallInfo->m_pFromAccountInfo = pFromAccountInfo;
            theApp->m_pCallList.insert(sCallId,pEbCallInfo);
        }
        else {
            pEbCallInfo->m_bOffLineUser = bOffLineUser;
            pEbCallInfo->m_tLastTime = time(0);
        }
        const int nSettingValue = theApp->m_ebum.EB_GetMyAccountSetting();
        /// 对方离线转在线，本端自动呼叫，不需要处理界面
        if ( bOwnerCall || bConnectMsg ||
                ( (bOffLineUser&&pConnectInfo->m_sGroupCode==0) &&
                 (nConnectFlag&EB_CONNECTED_AUTO_ACK)!=EB_CONNECTED_AUTO_ACK ) ) {
            /// 本方发起会话，离线会话
            /// 自动打开聊天界面
            const bool bAutoCall = (pConnectInfo->m_sGroupCode>0)?
                        theApp->m_pAutoCallGroupIdList.exist(pConnectInfo->m_sGroupCode):
                        theApp->m_pAutoCallFromUserIdList.exist(pConnectInfo->GetFromUserId());
            const bool bShow = bOwnerCall;
            DialogChatBase::pointer pDlgDialog = getDialogChatBase(pEbCallInfo,bShow,bOwnerCall);
            if (bOwnerCall && !bAutoCall) {
//                /// **关闭聊天提示窗口2
//                CDlgFuncWindow * pOldFuncWindow = NULL;
//                if (pEbCallInfo->m_pCallInfo.m_sGroupCode>0)
//                    theApp.m_pTempGroupMsg.find(pEbCallInfo->m_pCallInfo.m_sGroupCode,pOldFuncWindow,true);
//                else
//                    theApp.m_pTempFromUserMsg.find(pEbCallInfo->m_pFromAccountInfo.GetUserId(),pOldFuncWindow,true);
//                /// **必须放在前面
//                if (pOldFuncWindow!=NULL)
//                {
//                    pOldFuncWindow->ShowWindow(SW_HIDE);	// *必须隐藏不让用户点击；
//                    pOldFuncWindow->m_pEbCallInfo.reset();	// *必须设为空，否则会清掉后面窗口
//                    theApp.m_pCloseWnd.add(pOldFuncWindow);
//                }

//                if (m_pDlgFrameList!=0) {
//                    if (m_pDlgFrameList->showMinimized(); IsIconic())
//                        m_pDlgFrameList->ShowWindow(SW_RESTORE);
//                    m_pDlgFrameList->SetForegroundWindow();
//                }
            }

            if (bAutoCall) {
                char sSql[256];
                if (pConnectInfo->m_sGroupCode>0)
                    sprintf(sSql,"SELECT count(msg_id) FROM msg_record_t WHERE dep_code=%lld AND (read_flag&1)=0",pConnectInfo->m_sGroupCode);
                else
                    sprintf(sSql,"SELECT count(msg_id) FROM msg_record_t WHERE from_uid=%lld AND dep_code=0 AND (read_flag&1)=0",pConnectInfo->GetFromUserId());
                int nCookie = 0;
                theApp->m_sqliteUser->select(sSql,nCookie);
                cgcValueInfo::pointer pRecord = theApp->m_sqliteUser->first(nCookie);
                if (pRecord.get()!=NULL) {
                    const int nUnReadMsgCount = pRecord->getVector()[0]->getIntValue();
                    theApp->m_sqliteUser->reset(nCookie);

                    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
                        if (m_pDlgFrameList!=0) {
                            m_pDlgFrameList->setUnreadMsg(sCallId,nUnReadMsgCount);
                        }
                    }
                    else {
//                        this->SetUnreadMsg(sCallId,nUnReadMsgCount);
                    }
                }
            }
        }

        /// 保存会话记录
        if (bNewCall && !theApp->m_ebum.EB_IsLogonVisitor()) {
//            SaveCallRecord(sCallId,pConnectInfo->m_sGroupCode,pEbCallInfo->m_pFromAccountInfo);
        }
    }
    if (pConnectInfo->m_sOldCallId>0) {
        theApp->m_pCallList.remove(pConnectInfo->m_sOldCallId);
    }
//    DeleteDlgIncomingCall(pConnectInfo->GetFromUserId());
//    DeleteDlgAlertingCall(pConnectInfo->GetFromUserId());


    if (nDefaultUIStyleType==EB_UI_STYLE_TYPE_CHAT) {
        /// *跑下面
    }
    else if (!theApp->isHideMainFrame()) {
//        if (!CFrameWndInfoProxy::ExistCallIdWnd(sCallId)) {
//            // 没有打开聊天，设置为退出标识；
//            theApp->m_ebum.EB_CallExit(sCallId);
//        }
        return;// 0;
    }
    if (m_pDlgFrameList==NULL || !m_pDlgFrameList->existFrameItem(sCallId)) {
        /// 没有打开聊天，设置为退出标识；
        theApp->m_ebum.EB_CallExit(sCallId);
    }
}

void DialogMainFrame::onCallError(QEvent *e)
{
    const EB_CallInfo * pCallInfo = (EB_CallInfo*)e;
//    sndPlaySound(NULL,SND_NODEFAULT);
    const EB_STATE_CODE stateCode = (EB_STATE_CODE)pCallInfo->GetEventParameter();
    if (EB_STATE_APPID_KEY_ERROR==stateCode ||
        EB_STATE_APP_ONLINE_KEY_TIMEOUT==stateCode) {
        return; // 0;
    }

    const eb::bigint sCallId = pCallInfo->GetCallId();
    EbMessageBox::IMAGE_TYPE imageType = EbMessageBox::IMAGE_WARNING;
    QString sErrorText;
//    DeleteDlgAlertingCall(pCallInfo->GetFromUserId());
    switch (stateCode)
    {
    case EB_STATE_ACCOUNT_NOT_EXIST:
        /// 用户：\r\n%s 不存在！
        sErrorText = theLocales.getLocalText("on-call-response.account-not-exist.text","Account Not Exist");
        break;
    case EB_STATE_USER_OFFLINE:
        /// 用户：\r\n%s 离线状态！
        sErrorText = theLocales.getLocalText("on-call-response.user-offline.text","User Offline");
        break;
    case EB_STATE_UNAUTH_ERROR:
        /// 帐号：\r\n%s 未通过验证！
        sErrorText = theLocales.getLocalText("on-call-response.unauth-error.text","Unauth Error");
        break;
    case EB_STATE_ACCOUNT_FREEZE:
        /// 帐号：\r\n%s 已被临时冻结！
        sErrorText = theLocales.getLocalText("on-call-response.account-freeze.text","Account Freeze");
        break;
    case EB_STATE_MAX_RETRY_ERROR:
        /// 错误次数太频繁：\r\n帐号已被临时锁住，请稍候再试！
        sErrorText = theLocales.getLocalText("on-call-response.max-retry-error.text","Max Retry Error");
        break;
    case EB_STATE_USER_BUSY:
        /// 用户：\r\n%s 忙！
//        DeleteDlgIncomingCall(pCallInfo->GetFromUserId());
        sErrorText = theLocales.getLocalText("on-call-response.user-busy.text","User Busy");
        break;
    case EB_STATE_TIMEOUT_ERROR:
        /// 用户：\r\n%s 会话超时！
        /// 本地太久未响应，也会有该事件
//        DeleteDlgIncomingCall(pCallInfo->GetFromUserId());
        sErrorText = theLocales.getLocalText("on-call-response.timeout-error.text","Timeout Error");
        break;
    case EB_STATE_USER_HANGUP:
        /// 用户：\r\n%s 取消会话！
//        DeleteDlgIncomingCall(pCallInfo->GetFromUserId());
        sErrorText = theLocales.getLocalText("on-call-response.user-hangup.text","User Hangup");
        break;
    case EB_STATE_ACCOUNT_DISABLE_OFFCALL:
        /// 用户：\r\n%s 离线，对方屏蔽接收离线消息功能！
        sErrorText = theLocales.getLocalText("on-call-response.account-disable-offcall.text","Account Disable Offcall");
        break;
    case EB_STATE_ACCOUNT_DISABLE_EXTCALL:
        /// 用户：\r\n%s 屏蔽外部聊天功能！
        sErrorText = theLocales.getLocalText("on-call-response.account-disable-extcall.text","Account Disable Extcall");
        break;
    case EB_STATE_OAUTH_FORWARD:
        /// 操作成功，等待对方：\r\n%s 通过验证！
        sErrorText = theLocales.getLocalText("on-call-response.oauth-forward.text","Oauth Forward");
        imageType = EbMessageBox::IMAGE_INFORMATION;
        break;
    default:
        /// 请求会话：\r\n%s 失败，错误代码：%d
        sErrorText = theLocales.getLocalText("on-call-response.other-error.text","Call Error");
        break;
    }

//    EB_AccountInfo pFromAccountInfo;
//    if (theEBAppClient.EB_GetCallAccountInfo(sCallId,pCallInfo->GetFromUserId(),&pFromAccountInfo))
//    {
//        SaveCallRecord(sCallId,pCallInfo->m_sGroupCode,pFromAccountInfo);
//        //m_btnMySession.SetWindowText(_T("！"));
//    }

    sErrorText.replace( "[USER_ACCOUNT]", pCallInfo->GetFromAccount().c_str() );
    sErrorText.replace( "[USER_ID]", QString::number((qlonglong)pCallInfo->GetFromUserId()) );
    sErrorText.replace( "[STATE_CODE]", QString::number((int)stateCode) );
    EbMessageBox::doShow( NULL, "", QChar::Null, sErrorText, imageType,default_warning_auto_close );
}

void DialogMainFrame::onCallHangup(QEvent *e)
{
//    sndPlaySound(NULL,SND_NODEFAULT);
    const EB_CallInfo * pCallInfo = (EB_CallInfo*)e;
    const bool bOwner = (bool)(pCallInfo->GetEventParameter()==1);

    bool bRemoveCall = bOwner;
    DialogChatBase::pointer pDlgDialog = getDialogChatBase(pCallInfo->GetCallId(), bRemoveCall);
    if (pDlgDialog.get()!=0) {
        if (!bRemoveCall) {
            pDlgDialog->onUserExitRoom(pCallInfo->GetFromUserId(),false);
        }
    }
    if (bOwner ||
        pDlgDialog.get()==0) {		// 本地已经关闭聊天界面
        theApp->m_pCallList.remove(pCallInfo->GetCallId());
    }
//    DeleteDlgAlertingCall(pCallInfo->GetFromUserId());
//    DeleteDlgIncomingCall(pCallInfo->GetFromUserId());
}

void DialogMainFrame::onCallAlerting(QEvent *e)
{
    const EB_CallInfo * pCallInfo = (EB_CallInfo*)e;
    const int const_dlg_width = 380;
    const int const_dlg_height = 262;
    const QRect& screenRect = theApp->screenRect();

    const eb::bigint sCallId = pCallInfo->GetCallId();
    const eb::bigint nFromUserId = pCallInfo->GetFromUserId();
    const tstring sFromAccount = pCallInfo->GetFromAccount();

    EB_AccountInfo pFromAccountInfo;
    if (!theApp->m_ebum.EB_GetCallAccountInfo(sCallId,nFromUserId,&pFromAccountInfo)) return;   // 1;

    DialogChatBase::pointer pDlgDialog = getDialogChatBase(sCallId);
    if (pDlgDialog.get()!=0) {
        pDlgDialog->onAlertingCall();
    }
//    else if (!m_pAlertingCallList.exist(pCallInfo->GetFromUserId())) {
//        CDlgAlertingCall::pointer pDlgAlertingCall = CDlgAlertingCall::create(this);
//        pDlgAlertingCall->m_pFromAccountInfo = pFromAccountInfo;
//        pDlgAlertingCall->m_sCallId = sCallId;
//        pDlgAlertingCall->Create(CDlgAlertingCall::IDD, this);
//        CRect rectDlgIncomingCall;
//        pDlgAlertingCall->GetWindowRect(&rectDlgIncomingCall);
//        pDlgAlertingCall->MoveWindow(0, nScreenHeight-const_dlg_height, const_dlg_width, const_dlg_height);
//        pDlgAlertingCall->ShowWindow(SW_SHOW);
//        //pDlgAlertingCall->SetActiveWindow();
//        pDlgAlertingCall->SetForegroundWindow();
//        m_pAlertingCallList.insert(pCallInfo->GetFromUserId(), pDlgAlertingCall);
//    }
}

void DialogMainFrame::onCallIncoming(QEvent *e)
{
    const EB_CallInfo * pCallInfo = (EB_CallInfo*)e;
//    QSound::play( ":/wav/incomingcall.wav" );
//    CString sSoundFile;
//    sSoundFile.Format(_T("%s/wav/incomingcall.wav"), theApp.GetAppDataPath());
//    sndPlaySound(sSoundFile, SND_ASYNC|SND_NOWAIT|SND_LOOP);
    const int const_dlg_width = 380;
    const int const_dlg_height = 262;
    const QRect& screenRect = theApp->screenRect();

    const EB_AccountInfo* pFromAccount = (const EB_AccountInfo*)pCallInfo->GetEventData();

    const eb::bigint sCallId = pCallInfo->GetCallId();
    //EbcCallInfo::pointer pCallInfo = theEBAppClient.GetCallInfo(sCallId);;
    //if (pCallInfo.get() == NULL) return 1;

//    CDlgIncomingCall::pointer pDlgIncomingCall = CDlgIncomingCall::create(this);
//    pDlgIncomingCall->m_pFromAccountInfo.operator =(pFromAccount);
//    pDlgIncomingCall->m_pCallInfo = pCallInfo;
//    pDlgIncomingCall->Create(CDlgIncomingCall::IDD, this);
//    pDlgIncomingCall->MoveWindow(nScreenWidth-const_dlg_width, nScreenHeight-const_dlg_height, const_dlg_width, const_dlg_height);
//    pDlgIncomingCall->SetCircle();
//    pDlgIncomingCall->ShowWindow(SW_SHOW);
//    //pDlgIncomingCall->SetActiveWindow();
//    pDlgIncomingCall->SetForegroundWindow();
//    m_pIncomingCallList.insert(pCallInfo->GetFromUserId(), pDlgIncomingCall);
}

