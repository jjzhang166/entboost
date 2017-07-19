#ifndef EBWIDGETMYENTERPRISE_H
#define EBWIDGETMYENTERPRISE_H

#include "ebclientapp.h"
#include <QWidget>
#include <QTreeWidget>
#include <QPushButton>
#include <QMenu>
#include <ebwidgetiteminfo.h>
#include <ebcontextmenu.h>

class EbWidgetMyEnterprise : public QWidget
{
    Q_OBJECT
public:
    explicit EbWidgetMyEnterprise(QWidget *parent = nullptr);
    virtual ~EbWidgetMyEnterprise(void);
    enum SORT_ITEMS_FLAG {
        DISABLE_SORT    /// 禁排序
        , AUTO_SORT     /// 自动排序
        , FORCE_SORT   /// 强制排序
    };
    void timerCheckState(void);
    void onEnterpriseInfo(const EB_EnterpriseInfo* pEnterpriseInfo);
    void SetMemberInfo(const EbWidgetItemInfo::pointer& pGroupItemInfo, const EB_MemberInfo* pMemberInfo, SORT_ITEMS_FLAG nSortItems=AUTO_SORT);
    void onGroupInfo(const EB_GroupInfo * pGroupInfo);
    void onMemberInfo(const EB_MemberInfo * pMemberInfo, bool bChangeLineState);
    void onRemoveGroup(const EB_GroupInfo* pGroupInfo);
    void deleteMemberInfo(const EB_GroupInfo* pGroupInfo, eb::bigint nMemberCode, bool fromDeleteGroup);

public slots:
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void onItemClicked(QTreeWidgetItem *item, int column);
    void onItemEntered(QTreeWidgetItem *item, int column);
//    void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void onClickedPushButtonCall(void);
    void onClickedPushButtonEdit(void);

protected:
    virtual void resizeEvent(QResizeEvent *);
    void createMenuData(void);
    virtual void contextMenuEvent(QContextMenuEvent * e);
    virtual void timerEvent(QTimerEvent *e);

    void setGroupCount(const EbWidgetItemInfo::pointer& pGroupItem, eb::bigint sGroupCode, bool bFromMemberInfo);
    void setGroupCount(const EbWidgetItemInfo::pointer& pGroupItem, const EB_GroupInfo* pGroupInfo, bool bFromMemberInfo);

private:
    QTreeWidget * m_treeWidget;
    QPushButton * m_pushButtonCall;
    QPushButton * m_pushButtonEdit;
    CLockMap<eb::bigint,EbWidgetItemInfo::pointer> m_pEntItemInfo;	// ent_code->
    CLockMap<eb::bigint,EbWidgetItemInfo::pointer> m_pDepItemInfo;	// dep_code->
    CLockMap<eb::bigint,EbWidgetItemInfo::pointer> m_pEmpItemInfo;	// emp_code->
    EbContextMenu * m_contextMenu;
    int m_timerIdUpdateEnterpriseCount;
    EbWidgetItemInfo::pointer m_pUpdateEntItemInfo;
    int m_timerIdUpdateGroupCount;
    CLockMap<eb::bigint,EbWidgetItemInfo::pointer> m_pUpdateGroupCountList;	// group_id->
};

#endif // EBWIDGETMYENTERPRISE_H
