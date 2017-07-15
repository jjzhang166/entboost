#ifndef EBLINEEDIT_H
#define EBLINEEDIT_H

#include <QLineEdit>
#include <QFocusEvent>

class EbLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit EbLineEdit(QWidget *parent=0);

    void setMouseFocusInSelectAll(bool enable) {m_mouseFocusInSelectAll=enable;}
    bool isMouseFocusInSelectAll(void) const {return m_mouseFocusInSelectAll;}

signals:
    void keyPressEsc(void);
    void keyPressEnter(const QString &text);
    void keyPressDown(void);

public slots:
    void mouseFocusInSelectAll(void);

protected:
    virtual void focusInEvent(QFocusEvent *);
    virtual void keyPressEvent(QKeyEvent *);

private:
    bool m_mouseFocusInSelectAll;

};

#endif // EBLINEEDIT_H
