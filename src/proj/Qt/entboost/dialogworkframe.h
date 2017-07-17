#ifndef DIALOGWORKFRAME_H
#define DIALOGWORKFRAME_H

#include "ebclientapp.h"
#include <EbDialogBase.h>
#include <ebworklist.h>

namespace Ui {
class DialogWorkFrame;
}

class EbWidgetSearchBar;
class EbWidgetAppBar;

/// 工作台界面
/// 上面集成一个搜索地址栏，下面集成浏览器
class DialogWorkFrame : public EbDialogBase
        , public EbWorkList
{
    Q_OBJECT

public:
    explicit DialogWorkFrame(bool showSearchBae, bool showAppBar, QWidget *parent = 0);
    ~DialogWorkFrame();

    void updateLocaleInfo(void);
    void setSubUnreadMsg( mycp::bigint subId, size_t unreadMsgCount );
    void addSubUnreadMsg( mycp::bigint subId );
//    bool addSubscribeFuncInfo( const EB_SubscribeFuncInfo & pSubscribeFuncInfo );
//    bool IsSubscribeFuncInfoEmpty(void) const {return m_pMainFuncButtonList.empty();}
    void addUrl(bool bSaveBrowseTitle, const QString& sAppUrl, const QString& sPostData="", int nInsertOffset=-1);
    void addUrl(bool bSaveBrowseTitle, const QString& sAppUrl, const QString& sPostData, const EB_SubscribeFuncInfo& pFuncInfo, bool nOpenNewClose=false, int nInsertOffset = -1);
    void onMoveEvent(void);
signals:
    void itemSizeChange(bool fromAboutBlank,int size);

public slots:
    void onClickedButtonTop(void);
    void onUrlChanged(const EbWidgetWorkView * view, const QUrl&);
    void onLoadStateChange(const EbWidgetWorkView * view, bool loadFinished, bool canGoBack, bool canGoForward);
    void onOpenLinkInNewTab(const EbWidgetWorkView * view, const QUrl &linkUrl);
    void onWindowCloseRequested(const EbWidgetWorkView * view);

    void onClickedBack(void);
    void onClickedForward(void);
    void onClickedRefresh(void);
    void onClickedStop(void);
    void onSearchKeyPressEnter(const QString &text);
    void onSaveUrlHistory(void);
    void onClickedSearchResultUrl(const QString &url);

    void onClickedPushButtonShowHideLeft(void);
    void onClickedButtonAdd(void);
    void onClickedSubApp(const EB_SubscribeFuncInfo &);

    void onCloseViewList(void);
protected:
    void updateShowHideIcon(void);
    void moveSize(void);
    virtual void resizeEvent(QResizeEvent *);
    virtual bool onBeforeClickedPushButtonSysMin(void);
    virtual bool onBeforeClickedPushButtonSysMax(void);
    virtual void reject(void);
    virtual void timerEvent(QTimerEvent *event);
//    virtual void closeEvent(QCloseEvent *event);

    virtual void onItemSizeChange(const EbWorkItem::pointer & fromWorkItem,int size, int right);

private:
    Ui::DialogWorkFrame *ui;
    EbWidgetSearchBar * m_widgetSearchBar;
    bool m_showSearchBar;
    EbWidgetAppBar * m_widgetAppBar;
    bool m_showAppBar;
    int m_timerIdCheckState;
    bool m_showedLeft;
    CLockList<const EbWidgetWorkView*> m_listCloseView;

};

#endif // DIALOGWORKFRAME_H
