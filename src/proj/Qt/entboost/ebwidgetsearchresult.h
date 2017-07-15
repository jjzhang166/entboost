#ifndef EBWIDGETSEARCHRESULT_H
#define EBWIDGETSEARCHRESULT_H

#include <ebclientapp.h>
#include <QWidget>
#include <QResizeEvent>
#include <QMenu>
#include <QContextMenuEvent>
#include <eblistwidget.h>

class EbWidgetSearchResult : public QWidget
{
    Q_OBJECT
public:
    explicit EbWidgetSearchResult(QWidget *parent = nullptr);

    void search(const char* key);
    void setFocusSelectFirst(void);
    const EbListWidget * listResults(void) const {return m_listWidgetResults;}
    void updateLocaleInfo(void);
signals:
    void clickedSearchResultUrl(const QString &url);
    void searchFirst(const QString &url);

public slots:
//    void onItemClicked(QListWidgetItem *item);
    void onKeyPressEnterItem(const QListWidgetItem *item);
    void onClickedItemLabel(void);
//    void onKeyPressEnterItemLabel(void);
    void onClickedContextMenu(void);

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void contextMenuEvent(QContextMenuEvent *);

private:
    inline void checkShowHide(void);
private:
    EbListWidget * m_listWidgetResults;
    QMenu * m_menu;
    QAction * m_actionCopy;
    QAction * m_actionDelete;
};

#endif // EBWIDGETSEARCHRESULT_H
