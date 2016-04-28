#ifndef COMBOBOXGROUP_H
#define COMBOBOXGROUP_H

#include <QObject>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include <QList>

class ComboBoxGroup : public  QGroupBox
{
    Q_OBJECT
    QLabel* label;
    QGridLayout* layout;
    QComboBox* comboBox;
public:
    explicit ComboBoxGroup(QObject *parent = 0);

    void setList(QStringList &list);
    void setLabel(QString text);

signals:
    void pickedItemChanged(QString newItem);
public slots:
    void comboBoxActivated(const QString & text);
};

#endif // COMBOBOXGROUP_H
