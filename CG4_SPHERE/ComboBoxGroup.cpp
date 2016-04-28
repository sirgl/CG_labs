#include "ComboBoxGroup.h"
#include "QComboBox"

ComboBoxGroup::ComboBoxGroup(QObject *parent) : QGroupBox(){
    layout = new QGridLayout();

    label = new QLabel();
    comboBox = new QComboBox;
    label->setText("Hello");


    layout->addWidget(label, 0, 0);
    layout->addWidget(comboBox, 0, 1);
    setLayout(layout);

    connect(comboBox, SIGNAL(activated(QString)), this, SLOT(comboBoxActivated(QString)));

    setFixedHeight(80);
}

void ComboBoxGroup::setList(QStringList& list){
    comboBox->insertItems(0, list);
}

void ComboBoxGroup::setLabel(QString text){
    label->setText(text);
}

void ComboBoxGroup::comboBoxActivated(const QString &text){
    emit pickedItemChanged(text);
}
