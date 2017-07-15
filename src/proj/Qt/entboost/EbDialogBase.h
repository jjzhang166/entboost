// EbDialogBase.h file here
#ifndef EBDIALOGBASE_H
#define EBDIALOGBASE_H

#include <QDialog>
//#include <QPushButton>
#include <QMouseEvent>
#include <QLabel>

namespace Ui {
class EbDialogBase;
}

const QSize const_sys_button_size(24,22);
const int const_sys_button_right_interval = 0;      /// 关闭按钮右键间隔，默认 0 像素
const QSize const_okcancel_button_size(81,32);

class EbDialogBase : public QDialog
{
    Q_OBJECT

public:
    EbDialogBase(QWidget *parent = 0);
    virtual ~EbDialogBase(void);
    typedef enum EB_DIALOG_FLAG {
        EB_DIALOG_FLAG_NONE = 0x0
        , EB_DIALOG_FLAG_FILTER_ESC_KEY     = 0x1
        , EB_DIALOG_FLAG_FILTER_ENTER_KEY   = 0x2
        , EB_DIALOG_FLAG_DRAW_BORDER        = 0x100
        , EB_DIALOG_FLAG_EASY_MOVE          = 0x200

    }EB_DIALOG_FLAG;

    void showTitleLogoText(const QString& textString, int textSize=12, QChar logoChar = QChar::Null, int logoSize=12, const QString& titleTextObjectName="DialogTitleText");
    void showTitleBackground(int height,const QString& objectName="TitleBackgroundWidget");
    int titleBackgroundHeight(void) const;
//    void setBackgroundColor(QRgb rgb);
    void showPushButtonSysMin(const QString& sTooltip, const QString& objectName="SysTransButton");
    void showPushButtonSysMax(const QString& sTooltip, const QString& objectName="SysTransButton");
    void showPushButtonSysClose(const QString& sTooltip, const QString& objectName="SysCloseButton");
    int getSysButtonWidth(void) const;
    void showPushButtonStdOkCancel(const QString& sOkText, const QString& okObjectName="OkButton", const QString& sCancelText="", const QString& cancelObjectName="CancelButton");

    bool isMouseEasyMove(void) const {return (m_dialogFlags&EB_DIALOG_FLAG_EASY_MOVE)==0?false:true;}
    void setMouseEasyMove(bool bEnable);
    bool isFilterEscapeKey(void) const {return (m_dialogFlags&EB_DIALOG_FLAG_FILTER_ESC_KEY)==0?false:true;}
    void setFilterEscapeKey(bool b);
    bool isFilterEnterKey(void) const {return (m_dialogFlags&EB_DIALOG_FLAG_FILTER_ENTER_KEY)==0?false:true;}
    void setFilterEnterKey(bool b);
    bool isDrawBorder(void) const {return (m_dialogFlags&EB_DIALOG_FLAG_DRAW_BORDER)==0?false:true;}
    void setDrawBorder(bool b);

    void setButtonOkEnabled(bool bEnable);
    void setButtonOkVisible(bool visible);
    void setButtonCancelVisible(bool visible);
    void setDoubleClickMaxEnable(bool bEnable) {m_enableDoubleClickMax = bEnable;}
    bool isDoubleClickMaxEnable(void) const {return m_enableDoubleClickMax;}

public slots:
    virtual bool onBeforeClickedPushButtonSysMin(void) {return true;}
    void onClickedPushButtonSysMin(void);
    virtual bool onBeforeClickedPushButtonSysMax(void) {return true;}
    bool isMaximizedEb(void) const;
    void setMaxRestoreIcon(bool max);
    void onClickedPushButtonSysMax(void);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);

    virtual void keyPressEvent(QKeyEvent * e);
    virtual void resizeEvent(QResizeEvent *);

private:
    QPoint move_point; //移动的距离
    bool m_bMousePress; //按下鼠标左键
    int m_dialogFlags;

    QLabel* m_labelTitleLogo;
    QLabel* m_labelTitleText;
    QWidget* m_widgetTitleBackground;
    QPushButton* m_pushButtonSysMin;
    QPushButton* m_pushButtonSysMax;
    bool m_enableDoubleClickMax;
    QRect m_windowRect; // for maximize
    QPushButton* m_pushButtonSysClose;

    QPushButton* m_pushButtonOk;
    QPushButton* m_pushButtonCancel;
};

#endif // EBDIALOGBASE_H
