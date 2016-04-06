#include "CheckBoxGroup.h"
#include <QDebug>

void CheckBoxGroup::setLabel(QString str){
    label->setText(str);
}

CheckBoxGroup::CheckBoxGroup(){
    layout = new QGridLayout;
    setLayout(layout);

    label = new QLabel();

    checkBox = new QCheckBox;
    connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxChanged(bool)));
    layout->addWidget(label, 0, 0);
    layout->addWidget(checkBox, 0, 1);
    setFixedHeight(80);
}

void CheckBoxGroup::setState(bool value){
    checkBox->setChecked(value);
}

void CheckBoxGroup::checkBoxChanged(bool state){
    qDebug() << state;
    emit stateChanged(state);
}
