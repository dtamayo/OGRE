#include <math.h>
#include "DoubleSlider.h"

DoubleSlider::DoubleSlider(QWidget *parent, int ndecimals) : QSlider(parent) {
    convFactor = pow(10.,double(ndecimals));
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(notifyValueChanged(int)));
}

void DoubleSlider::notifyValueChanged(int value)
{
    double doubleValue = value/convFactor;
    emit doubleValueChanged(doubleValue);
}

void DoubleSlider::setDoubleValue(double value)
{
    this->setValue(int(value*convFactor));
}
