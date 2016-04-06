#ifndef CHECKBOXGROUP_H
#define CHECKBOXGROUP_H

#include <QString>
#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QCheckBox>

class CheckBoxGroup : public QGroupBox
{
    Q_OBJECT
    QGridLayout* layout;
    QLabel* label;
    QCheckBox* checkBox;
public:
    void setLabel(QString str);
    CheckBoxGroup();
    void setState(bool value);
private slots:
    void checkBoxChanged(bool state);
signals:
    void stateChanged(bool newState);

};

#endif // CHECKBOXGROUP_H
