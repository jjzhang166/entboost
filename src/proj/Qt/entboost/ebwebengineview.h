#ifndef EBWEBENGINEVIEW_H
#define EBWEBENGINEVIEW_H

#include <QWebEngineView>
#include <QMenu>
#include <stl/lockmap.h>
#include <ebwebenginedownloaditem.h>
//#include <QWebEngineDownloadItem>

class EbWebEnginePage;

class EbWebEngineView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit EbWebEngineView(QWidget* parent=0);
    virtual ~EbWebEngineView(void);

    EbWebEnginePage* page() const;

signals:
    void openLink(const EbWebEngineView * view, QWebEnginePage::WebWindowType type, const QUrl &linkUrl);
    void findAction(const QString &selectedText);

protected:
    virtual QWebEngineView *createWindow(QWebEnginePage::WebWindowType type);
    virtual void contextMenuEvent(QContextMenuEvent*) Q_DECL_OVERRIDE;

public slots:
    void onTriggeredActionOpenLinkInNewTab(void);
    void onTriggeredActionOpenLinkInNewWindow(void);
    void onTriggeredActionFind(void);
    void onTriggeredActionPrint(void);
    void onTriggeredActionInspectElement(void);
    void onTriggeredActionPageTriggerAction(void);
    void onDownloadRequested(QWebEngineDownloadItem* );
    void onDownloadFinished(const EbWebEngineDownloadItem*);
//    void onDownLoadStateChanged(QWebEngineDownloadItem::DownloadState state);
//    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    void onLinkHovered(const QString &url);

protected:
//    virtual void keyPressEvent(QKeyEvent *event);
//    virtual bool event(QEvent*);

private:
    void createMenuData(void);
    void hideAllAction(void);
    void setActionVisible( QWebEnginePage::WebAction webAction, bool visible, bool enable=true );

private:
    QMenu * m_menu;
    QString m_linkHoveredUrl;
    CLockMap<QWebEnginePage::WebAction,QAction*> m_actions;
    CLockMap<quint32,EbWebEngineDownloadItem::pointer> m_downloads;
};

#endif // EBWEBENGINEVIEW_H
