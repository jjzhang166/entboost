#ifndef DIALOGREGISTER_H
#define DIALOGREGISTER_H

#include "ebclientapp.h"
#include <EbDialogBase.h>


namespace Ui {
class DialogRegister;
}

class DialogRegister : public EbDialogBase
{
    Q_OBJECT

public:
    explicit DialogRegister(QWidget *parent = 0);
    ~DialogRegister();

    const QString& account(void) const {return m_sAccount;}
//    const QString& password(void) const {return m_sPassword;}
    const QString& userName(void) const {return m_sUserName;}
    const QString& enterpriseName(void) const {return m_sEnterpriseName;}
    mycp::bigint registerUserId(void) const {return m_nRegisterUserId;}

protected:
    virtual void accept(void);
    virtual void customEvent(QEvent *e);

    void onRegisterResponse(QEvent *e);

private:
    Ui::DialogRegister *ui;
    CEBAppClient m_pEBAppClient;
    QString m_sAccount;
    QString m_sPassword;
    QString m_sConfirmPwd;
    QString m_sUserName;
    QString m_sEnterpriseName;
    mycp::bigint m_nRegisterUserId;
};

#endif // DIALOGREGISTER_H
