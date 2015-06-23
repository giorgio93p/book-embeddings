#include "myfirstdialog.h"


MyFirstDialog::MyFirstDialog()
{
    qsb = new QSpinBox();
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(qsb, 0, 0);
    connect(this,SIGNAL(accept()), this, SLOT(done(qsb->value())));

}


