#include "ebdialogviewecard.h"
#include "ui_ebdialogviewecard.h"

EbDialogViewECard::EbDialogViewECard(QWidget *parent) :
    EbDialogBase(parent),
    ui(new Ui::EbDialogViewECard)
{
    ui->setupUi(this);
}

EbDialogViewECard::~EbDialogViewECard()
{
    delete ui;
}
