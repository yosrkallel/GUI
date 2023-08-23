#ifndef CHECKDEMO_H
#define CHECKDEMO_H

#include <QDialog>
#include <QWidget>
#include <QCheckBox>
#include <QHBoxLayout>

namespace Ui {
class CheckDemo;
}

class CheckDemo : public QDialog
{
    Q_OBJECT

public:
    explicit CheckDemo(QWidget *parent = nullptr);
    void setFilterText(const QString &text);

    ~CheckDemo();

signals:
    void ecuApidFiltersChangedSignal(const QJsonArray& filteredData);

private slots:

    void stateSlot(int state);
    void on_checkBox_stateChanged(int arg1);
    void on_checkBox_2_stateChanged(int arg1);
    void on_buttonBox_accepted();

private:

    Ui::CheckDemo *ui;
    QCheckBox *showEcuCheckbox;
    QCheckBox *showApidCheckbox;
};

#endif // CHECKDEMO_H
