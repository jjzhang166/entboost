#ifndef EBWIDGETMYGROUP_H
#define EBWIDGETMYGROUP_H

#include "ebclientapp.h"
#include <ebwidgettreeselectbase.h>
#include <QTreeWidget>
#include <QPushButton>
#include <QMenu>
#include <ebdefines.h>
#include <ebcallback.h>
#include <ebwidgetiteminfo.h>
#include <ebcontextmenu.h>

class EbWidgetMyGroup : public EbWidgetTreeSelectBase
{
    Q_OBJECT
public:
    explicit EbWidgetMyGroup(EB_VIEW_MODE viewMode,QWidget *parent = 0);
    virtual ~EbWidgetMyGroup(void);

    void SetMemberInfo(const EbWidgetItemInfo::pointer& pGroupItemInfo, const EB_MemberInfo* pMemberInfo, EB_SORT_ITEMS_FLAG nSortItems=EB_AUTO_SORT);
    void onGroupInfo(const EB_GroupInfo * pGroupInfo);
    void onMemberInfo(const EB_MemberInfo * pMemberInfo, bool bChangeLineState);
    void onRemoveGroup(const EB_GroupInfo* pGroupInfo);
    void deleteMemberInfo(const EB_GroupInfo* pGroupInfo, eb::bigint nMemberCode, bool fromDeleteGroup);
//    void deleteGroupInfo(const EB_GroupInfo* pGroupInfo);

signals:

public slots:
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void onItemClicked(QTreeWidgetItem *item, int column);
    void onItemEntered(QTreeWidgetItem *item, int column);
//    void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void onClickedPushButtonSelect(void);
    void onClickedPushButtonCall(void);
    void onClickedPushButtonEdit(void);

protected:
    virtual void resizeEvent(QResizeEvent *);
    void createMenuData(void);
    virtual void contextMenuEvent(QContextMenuEvent * e);

private:
    CLockMap<eb::bigint,EbWidgetItemInfo::pointer> m_pDepItemInfo;	// dep_code->
    CLockMap<eb::bigint,EbWidgetItemInfo::pointer> m_pEmpItemInfo;	// emp_code->
    EbContextMenu * m_contextMenu;
};

#endif // EBWIDGETMYGROUP_H
