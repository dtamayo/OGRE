#include <QSlider>

class DoubleSlider : public QSlider {
    Q_OBJECT

public:
    DoubleSlider(QWidget *parent = 0, int ndecimals = 2);
    void setDoubleValue(double value);

signals:
    void doubleValueChanged(double value);

public slots:
    void notifyValueChanged(int value);

private:
    double convFactor; // factor to multiply and divide by to get ndecimals (10^ndecimals)
};
