#ifndef PCS_H
#define PCS_H

#include <QWidget>
#include "http.h"

namespace Ui {
class PCS;
}

class PCS : public QWidget
{
    Q_OBJECT

public:
    explicit PCS(QWidget *parent = nullptr);
    ~PCS();
    void myslots();

private:
    Ui::PCS *ui;
    Http http;
};

#endif // PCS_H
