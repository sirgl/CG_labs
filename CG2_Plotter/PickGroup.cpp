#include "PickGroup.h"
#include <QDebug>


PickGroup::PickGroup(QWidget* parent) : QGroupBox(parent) {
	layout = new QGridLayout();

	label = new QLabel();
	slider = new QSlider(Qt::Horizontal);
	spinBox = new QSpinBox();

	layout->addWidget(label, 0, 0);
	layout->addWidget(spinBox, 0, 1);
	layout->addWidget(slider, 1, 0, 1, 2);

	setLayout(layout);

	connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxChanged(int)));
	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
	setFixedHeight(80);
}

int PickGroup::getCurrentValue() {
	return spinBox->value();
}

void PickGroup::spinBoxChanged(int value) {
	emit valueChanged(value);
	slider->setSliderPosition(value);
}

void PickGroup::sliderChanged(int value) {
	emit valueChanged(value);
	spinBox->setValue(value);
}


void PickGroup::setLabel(QString labelStr) {
	label->setText(labelStr);
}

void PickGroup::setValue(int position){
	if (position < minValue || position > maxValue){
		return;
	}
	spinBox->setValue(position);
	slider->setValue(position);
}

void PickGroup::setMin(int min) {
	spinBox->setMinimum(min);
	slider->setMinimum(min);
	minValue = min;
}

void PickGroup::setMax(int max) {
	spinBox->setMaximum(max);
	slider->setMaximum(max);
	maxValue = max;
}
