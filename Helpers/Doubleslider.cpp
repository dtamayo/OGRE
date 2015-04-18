#include "DoubleSlider.h"

DoubleSlider::DoubleSlider(QWidget *parent) : QSlider(parent) {
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(notifyValueChanged(int)));
}

void DoubleSlider::notifyValueChanged(int value)
{
    double doubleValue = value / 100.0;
    emit doubleValueChanged(doubleValue);
}
