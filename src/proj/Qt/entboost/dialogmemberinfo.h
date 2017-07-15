#ifndef DIALOGMEMBERINFO_H
#define DIALOGMEMBERINFO_H

#include "ebclientapp.h"
#include <EbDialogBase.h>
#include <eblabel.h>

namespace Ui {
class DialogMemberInfo;
}

class DialogChangeHead;

class DialogMemberInfo : public EbDialogBase
{
    Q_OBJECT

public:
    explicit DialogMemberInfo(QWidget *parent = 0);
    ~DialogMemberInfo();

    QString m_groupName;
    EB_MemberInfo m_memberInfo;

    void updateLocaleInfo(void);
public slots:
//    virtual void open();
    virtual int exec();
    virtual void accept();

    void onClickedLabelUserHead(void);
    void onSelectedHeadImage(qint64 resourceId,const QString & resourceFile);

protected:
    void updateUserHeadImage(void);

private:
    Ui::DialogMemberInfo *ui;
    EbLabel * m_labelUserHead;
    tstring m_oldFileMd5;
    EB_GROUP_TYPE m_groupType;
    bool m_newEemployee;
    DialogChangeHead * m_dialogChangeHead;
};

#endif // DIALOGMEMBERINFO_H
