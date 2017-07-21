#ifndef EBDIALOGGROUPINFO_H
#define EBDIALOGGROUPINFO_H

#include "ebclientapp.h"
#include <ebdialogbase.h>
#include <eblabel.h>

namespace Ui {
class EbDialogGroupInfo;
}

class EbDialogGroupInfo : public EbDialogBase
{
    Q_OBJECT

public:
    explicit EbDialogGroupInfo(QWidget *parent = 0);
    ~EbDialogGroupInfo();

    EB_GroupInfo m_groupInfo;

    void updateLocaleInfo(void);
public slots:
    virtual int exec();
    virtual void accept();
    void onClickedLabelManagerUserName(void);

private:
    Ui::EbDialogGroupInfo *ui;
    EbLabel * m_labelManagerUserName;
};

#endif // EBDIALOGGROUPINFO_H
