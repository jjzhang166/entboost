#ifndef EBDIALOGVIEWECARD_H
#define EBDIALOGVIEWECARD_H

#include <ebclientapp.h>
#include <ebdialogbase.h>
#include <ebwidgetiteminfo.h>

namespace Ui {
class EbDialogViewECard;
}

class EbDialogViewECard : public EbDialogBase
{
    Q_OBJECT

public:
    enum VIEW_TYPE {
        VIEW_UNKNOWE
        , VIEW_ENTERPRISE
        , VIEW_GROUP
        , VIEW_MEMBER
        , VIEW_CONTACT
    };

    explicit EbDialogViewECard(QWidget *parent = 0);
    ~EbDialogViewECard();

    void updateLocaleInfo(void);
    bool setItemInfo(const EbWidgetItemInfo::pointer &itemInfo);
    void setEnterpriseInfo(const EB_EnterpriseInfo* pEnterpriseInfo);
    void setGroupInfo(const EB_GroupInfo* pGroupInfo,const EB_EnterpriseInfo* pEnterpriseInfo);
    void setGroupInfo(eb::bigint groupId);
    void setMemberInfo(const EB_MemberInfo* pMemberInfo,const EB_GroupInfo* pGroupInfo);
    void setMemberInfo(eb::bigint groupId, eb::bigint memberUserId);
    void setContactInfo(const EB_ContactInfo* pContactInfo);
    void setContactInfo1(eb::bigint contactId);
    void setContactInfo2(eb::bigint userId);

    void setMouseEnter(const QRect & rectValid, bool showImmediate=false);
    void hideReset(void);

public slots:
    void onClieckedButtonAddContact(void);
    void onClieckedButtonSendMsg(void);
protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void timerEvent(QTimerEvent *event);
    void updateEnterpriseInfo(void);
    void updateGroupInfo(void);
    void updateMemberInfo(void);
    void updateContactInfo(void);
private:
    Ui::EbDialogViewECard *ui;
    QRect m_rectValid;
    int m_timerIdCheck2Show;
    int m_timerIdCheck2Hide;
    VIEW_TYPE m_viewType;
    EB_EnterpriseInfo m_enterpriseInfo;
    EB_GroupInfo m_groupInfo;
    EB_MemberInfo m_memberInfo;
    EB_ContactInfo m_contactInfo;
};

#endif // EBDIALOGVIEWECARD_H
