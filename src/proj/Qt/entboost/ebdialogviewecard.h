#ifndef EBDIALOGVIEWECARD_H
#define EBDIALOGVIEWECARD_H

#include <EbDialogBase.h>

namespace Ui {
class EbDialogViewECard;
}

class EbDialogViewECard : public EbDialogBase
{
    Q_OBJECT

public:
    explicit EbDialogViewECard(QWidget *parent = 0);
    ~EbDialogViewECard();

private:
    Ui::EbDialogViewECard *ui;
};

#endif // EBDIALOGVIEWECARD_H
