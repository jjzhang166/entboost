#ifndef DIALOGGROUPINFO_H
#define DIALOGGROUPINFO_H

#include "ebclientapp.h"
#include <EbDialogBase.h>
#include <eblabel.h>

namespace Ui {
class DialogGroupInfo;
}

class DialogGroupInfo : public EbDialogBase
{
    Q_OBJECT

public:
    explicit DialogGroupInfo(QWidget *parent = 0);
    ~DialogGroupInfo();

    EB_GroupInfo m_groupInfo;

    void updateLocaleInfo(void);
public slots:
    virtual int exec();
    virtual void accept();
    void onClickedLabelManagerUserName(void);

private:
    Ui::DialogGroupInfo *ui;
    EbLabel * m_labelManagerUserName;
};

#endif // DIALOGGROUPINFO_H
