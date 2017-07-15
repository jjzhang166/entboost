#ifndef DIALOGMYCENTER_H
#define DIALOGMYCENTER_H

#include "ebclientapp.h"
#include <EbDialogBase.h>


namespace Ui {
class DialogMyCenter;
}
class EbWidgetAccountInfo;
class EbWidgetMySetting;
class EbWidgetModifyPassword;

class DialogMyCenter : public EbDialogBase
{
    Q_OBJECT

public:
    explicit DialogMyCenter(QWidget *parent = 0);
    ~DialogMyCenter();

    void updateLocaleInfo(void);
    void onAreaInfo(const EB_AreaInfo* pAreaInfo,int nParameter);

public slots:
    void onClickedPushButtonAccountInfo(void);
    void onClickedPushButtonMySetting(void);
    void onClickedPushButtonModifyPassword(void);
    void udpateClickedPushButton(const QObject *sender);

protected:
    virtual void accept(void);

private:
    Ui::DialogMyCenter *ui;
    QLabel * m_labelAccountInfoIcon;
    QLabel * m_labelModifyPasswordIcon;
    EbWidgetAccountInfo *m_widgetAccountInfo;
    EbWidgetMySetting *m_widgetMySetting;
    EbWidgetModifyPassword *m_widgetModifyPassword;
};

#endif // DIALOGMYCENTER_H
