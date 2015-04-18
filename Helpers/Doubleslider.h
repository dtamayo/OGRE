#include <QSlider>

class DoubleSlider : public QSlider {
    Q_OBJECT

public:
    DoubleSlider(QWidget *parent = 0);

signals:
    void doubleValueChanged(double value);

public slots:
    void notifyValueChanged(int value);
};
