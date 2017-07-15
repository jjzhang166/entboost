#ifndef DIALOGCONNECTSETTING_H
#define DIALOGCONNECTSETTING_H

#include <QDialog>
#include <EbDialogBase.h>

namespace Ui {
class DialogConnectSetting;
}

class DialogConnectSetting : public EbDialogBase
{
    Q_OBJECT

public:
    explicit DialogConnectSetting(QWidget *parent = 0);
    ~DialogConnectSetting();

    const QString& oldServer(void) const {return m_oldServer;}
    const QString& newServer(void) const {return m_newServer;}
    bool isServerModified(void) const {return m_oldServer==m_newServer?false:true;}

protected:
    virtual void accept(void);

private:
    Ui::DialogConnectSetting *ui;
    QString m_oldServer;
    QString m_newServer;
};

#endif // DIALOGCONNECTSETTING_H
