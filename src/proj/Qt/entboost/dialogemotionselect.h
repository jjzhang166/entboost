#ifndef DIALOGEMOTIONSELECT_H
#define DIALOGEMOTIONSELECT_H

#include "ebclientapp.h"
#include <EbDialogBase.h>
#include <QListWidgetItem>

namespace Ui {
class DialogEmotionSelect;
}

class DialogEmotionSelect : public EbDialogBase
{
    Q_OBJECT

public:
    explicit DialogEmotionSelect(QWidget *parent = 0);
    ~DialogEmotionSelect();

    void updateLocaleInfo(void);
signals:
    void selectedResource(const QIcon& icon,mycp::bigint resourceId,const QString& resourceCmInfo,const QString & resourceFilePath);

public slots:
    void onItemClicked(QListWidgetItem* item);
    void onItemEntered(QListWidgetItem* item);

protected:
    virtual void resizeEvent(QResizeEvent *);
    void checkFocusOut(void);
    virtual void focusOutEvent(QFocusEvent *event);
    virtual bool eventFilter(QObject *obj, QEvent *ev);
    void loadEmotion(void);
    virtual void timerEvent(QTimerEvent * event);

private:
    Ui::DialogEmotionSelect *ui;
    int m_timerIdLoadEmotion;
    bool m_hasEmptyFile;
    CLockMap<mycp::bigint,bool> m_loadedResourceIdList;
};

#endif // DIALOGEMOTIONSELECT_H
