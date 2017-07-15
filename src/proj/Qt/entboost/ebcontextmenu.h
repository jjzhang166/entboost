#ifndef EBCONTEXTMENU_H
#define EBCONTEXTMENU_H

#include <QMenu>
#include <ebwidgetiteminfo.h>
#include <stl/lockmap.h>

class EbContextMenu : QObject
{
    Q_OBJECT
public:
    enum Type {
        MyGroup,
        UserList
    };
    EbContextMenu(Type type, QWidget * parent=0);
    virtual ~EbContextMenu(void);

    QAction * exec(const QPoint & pos);
    QAction * setMenuActionVisible(int commandId,bool visible);
    bool updateMenuItem(const EbWidgetItemInfo::pointer &itemInfo);

    void onCallItem(const EbWidgetItemInfo::pointer &itemInfo);
    void onEditItem(const EbWidgetItemInfo::pointer &itemInfo);

public slots:
    void onTriggeredActionNewGroup(void);
    void onTriggeredActionEditGroup(void);
    void onTriggeredActionDeleteGroup(void);
    void onTriggeredActionExitGroup(void);
    void onTriggeredActionGroupAddForbidSpeech(void);
    void onTriggeredActionGroupDelForbidSpeech(void);
    void onTriggeredActionEditMember(void);
    void onTriggeredActionDeleteMember(void);
    void onTriggeredActionMemberAddAdminLevel(void);
    void onTriggeredActionMemberDelAdminLevel(void);
    void onTriggeredActionMemberAddForbidSpeech(void);
    void onTriggeredActionMemberDelForbidSpeech(void);
    void onTriggeredActionCallUser(void);
    void onTriggeredActionViewMsgRecord(void);
    void onTriggeredActionViewGroupShare(void);
private:
    void hideAllMenuAction(void);
private:
    Type m_type;
    EbWidgetItemInfo::pointer m_itemInfo;
    CLockMap<int,QAction*> m_actionList;
    QAction * m_adminSeparatorAction;
    QAction * m_memberSeparatorAction;
    QAction * m_memberForbidSpeechAction;
    QMenu * m_menu;

};

#endif // EBCONTEXTMENU_H