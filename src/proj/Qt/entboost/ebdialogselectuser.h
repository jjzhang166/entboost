#ifndef EBDIALOGSELECTUSER_H
#define EBDIALOGSELECTUSER_H

#include <ebclientapp.h>
#include <EbDialogBase.h>
#include <QLabel>
#include <ebcallback.h>
#include <eblineedit.h>
#include <ebwidgetiteminfo.h>
#include <ebwidgetsearchresult.h>

namespace Ui {
class EbDialogSelectUser;
}
class EbWidgetMyGroup;
class EbWidgetMyContact;
class EbWidgetMyEnterprise;

class EbDialogSelectUser : public EbDialogBase
        , public CEBSearchCallback
        , public EbSelectedUserCallback
{
    Q_OBJECT

public:
    explicit EbDialogSelectUser(QWidget *parent = 0);
    ~EbDialogSelectUser();

    void updateLocaleInfo(void);
    void timerCheckState(void);
    void clearSelected(void);
    void setSingleSelect(bool b) {m_singleSelect = b;}
    void setSelectedGroupId(eb::bigint v) {m_selectedGroupId = v;}

    void onGroupInfo(const EB_GroupInfo * pGroupInfo);
    void onMemberInfo(const EB_MemberInfo * pMemberInfo, bool bChangeLineState);
    void onRemoveGroup(const EB_GroupInfo* pGroupInfo);
    void deleteMemberInfo(const EB_GroupInfo* pGroupInfo, eb::bigint nMemberCode, bool fromDeleteGroup);

    CLockMap<tstring,bool> m_existedUser;    /// account->
    CLockMap<tstring,EbWidgetItemInfo::pointer> m_selectedList;
public slots:
    void onSelectedItemInfo(const EbWidgetItemInfo::pointer&);
    void onItemEnteredSelected(QListWidgetItem *item);
    void updateClickedButton(const QObject *sender);
    void onClickedButtonMyGroup(void);
    void onClickedButtonMyContact(void);
    void onClickedButtonMyEnterprise(void);
    void updateSelectedCount(void);
    void onClickedButtonDelete(void);
    void onLoadEnterpriseInfo(void);

    void onSearchEditTextChange(const QString &text);
    void onSearchEditKeyPressEsc(void);
    void onSearchEditKeyPressEnter(const QString &text);
    void onSearchEditKeyPressDown(void);

//    void onSearchFirst(const QString &url);
    void onClickedSearchResultUrl(const QString &url);
    void onListResultsKeyPressFirstItemUp(void);
    void onListResultsKeyPressEsc(void);

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void accept(void);

    /// CEBSearchCallback
    virtual void onGroupInfo(const EB_GroupInfo* pGroupInfo,const EB_EnterpriseInfo* pEnterpriseInfo, unsigned long dwParam);
    virtual void onMemberInfo(const EB_GroupInfo* pGroupInfo, const EB_MemberInfo* pMemberInfo, unsigned long dwParam);
    virtual void onContactInfo(const EB_ContactInfo* pContactInfo, unsigned long dwParam);
    /// EbSelectedUserCallback
    virtual bool isSelectedUser(const EbWidgetItemInfo::pointer &itemInfo);

private:
    Ui::EbDialogSelectUser *ui;
    QLabel * m_labelMyGroupIcon;
    QLabel * m_labelMyContactIcon;
    QLabel * m_labelMyEnterpriseIcon;
    EbLineEdit * m_lineEditSearch;
    EbWidgetMyGroup * m_widgetMyGroup;
    EbWidgetMyContact * m_widgetMyContact;
    const EB_EnterpriseInfo * m_enterpriseInfo;
    EbWidgetMyEnterprise * m_widgetMyEnterprise;
    EbWidgetSearchResult * m_widgetSearchResult;
    bool m_singleSelect;
    eb::bigint m_selectedGroupId;
};

#endif // EBDIALOGSELECTUSER_H
