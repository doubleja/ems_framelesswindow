#ifndef PCS_H
#define PCS_H

#include <QWidget>

namespace Ui {
class PCS;
}

class PCS : public QWidget
{
    Q_OBJECT

public:
    explicit PCS(QWidget *parent = nullptr);
    ~PCS();

private:
    Ui::PCS *ui;
};

#endif // PCS_H
