#ifndef EBWIDGETUSERLIST_H
#define EBWIDGETUSERLIST_H

#include <ebclientapp.h>
#include <QPushButton>
#include <QWidget>
#include <QListWidget>
#include <ebwidgetiteminfo.h>
#include <ebcontextmenu.h>

class EbWidgetUserList : public QWidget
{
    Q_OBJECT
public:
    explicit EbWidgetUserList(const EbcCallInfo::pointer &callInfo,QWidget *parent = nullptr);
    virtual ~EbWidgetUserList(void);

    void onEnterUser(eb::bigint nUserId, bool bSort);
    void onExitUser(eb::bigint nUserId,bool bExitDep);
    int onMemberInfo(const EB_MemberInfo* pMemberInfo, bool bSort);

signals:
    void memberOnlineSizeChange(int memberSize, int onlineSize);

public slots:
    void onItemDoubleClicked(QListWidgetItem *item);
    void onItemEntered(QListWidgetItem *);
    void onLoadUserList(void);
    void onClickedPushButtonCall(void);
    void onClickedPushButtonEdit(void);

protected:
    virtual void resizeEvent(QResizeEvent *event);
    void createMenuData(void);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void timerEvent(QTimerEvent *event);
private:
    void onCallItem(QListWidgetItem* item);
    void onEditItem(QListWidgetItem* item);
    void updateGroupMemberSize(void);

private:
    EbcCallInfo::pointer m_callInfo;
    CLockMap<eb::bigint, EbWidgetItemInfo::pointer> m_pUserItem;		// uid->
    QListWidget * m_listWidgetUsers;
    QPushButton * m_pushButtonCall;
    QPushButton * m_pushButtonEdit;
    EbContextMenu * m_contextMenu;
    int m_timerIdCheckState;
};

#endif // EBWIDGETUSERLIST_H
