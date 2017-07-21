#ifndef EBDIALOGREQUESTADDCONTACT_H
#define EBDIALOGREQUESTADDCONTACT_H

#include "ebclientapp.h"
#include <ebdialogbase.h>


namespace Ui {
class EbDialogRequestAddContact;
}

class EbDialogRequestAddContact : public EbDialogBase
{
    Q_OBJECT

public:
    explicit EbDialogRequestAddContact(QWidget *parent = 0);
    ~EbDialogRequestAddContact();

    QString m_headFilePath;
    QString m_fromName;
    eb::bigint m_fromUserId;
    QString m_fromAccount;
    void updateLocaleInfo(void);
public slots:
    virtual int exec();
    virtual void accept();

private:
    Ui::EbDialogRequestAddContact *ui;
};

#endif // EBDIALOGREQUESTADDCONTACT_H
