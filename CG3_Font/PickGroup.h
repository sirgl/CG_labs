#pragma once
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>

class PickGroup : public  QGroupBox {
	Q_OBJECT
	QSlider* slider;
	QLabel* label;
	QGridLayout* layout;
	QSpinBox* spinBox;
	int minValue;
	int maxValue;
public:

	void setMax(int max);
	void setMin(int min);
	void setLabel(QString labelStr);
	void setValue(int position);

	explicit PickGroup(QWidget* parent = 0);

	int getCurrentValue();

private slots:
	void spinBoxChanged(int value);
	void sliderChanged(int value);

signals:
	void valueChanged(int value);
};
