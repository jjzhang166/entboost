#ifndef EBWIDGETMYCONTACT_H
#define EBWIDGETMYCONTACT_H

#include <ebclientapp.h>
#include <QWidget>
#include <QTreeWidget>
#include <QPushButton>
#include <ebcontextmenu.h>

class EbWidgetMyContact : public QWidget
{
    Q_OBJECT
public:
    explicit EbWidgetMyContact(QWidget *parent = nullptr);
    virtual ~EbWidgetMyContact(void);

    void timerCheckState(void);
    const EbWidgetItemInfo::pointer onUGInfo(const EB_UGInfo* ugInfo);
    void onUGDelete(const EB_UGInfo* ugInfo);
    void onContactInfo(const EB_ContactInfo* contactInfo);
    void onContactDelete(const EB_ContactInfo* contactInfo);
    void onContactHeadChange(const EB_ContactInfo* contactInfo);

signals:

public slots:
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void onItemClicked(QTreeWidgetItem *item, int column);
    void onItemEntered(QTreeWidgetItem *item, int column);
    void onItemChanged(QTreeWidgetItem *item, int column);
    void onClickedPushButtonCall(void);
    void onClickedPushButtonEdit(void);
    void onAddContactGroup(void);

protected:
    virtual void resizeEvent(QResizeEvent *);
    void createMenuData(void);
    virtual void contextMenuEvent(QContextMenuEvent * e);

    void deleteGroupItem(QTreeWidgetItem *item);
private:
    QTreeWidget * m_treeWidgetMyContact;
    QPushButton * m_pushButtonCall;
    QPushButton * m_pushButtonEdit;
    CLockMap<eb::bigint,EbWidgetItemInfo::pointer> m_pGroupItemInfo;	// ug_id->
    CLockMap<eb::bigint,EbWidgetItemInfo::pointer> m_pContactItemInfo;	// contact_id->
    EbContextMenu * m_contextMenu;
    time_t m_timeNewGroup;
    bool m_inGroupEdit;
};

#endif // EBWIDGETMYCONTACT_H
