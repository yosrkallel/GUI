#include "checkdemo.h"
#include "ui_checkdemo.h"
#include "mainwindow.h"
#include <QJsonArray>
#include <QStringList>


CheckDemo::CheckDemo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckDemo)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &CheckDemo::on_buttonBox_accepted);

    connect(ui->checkBox, &QCheckBox::stateChanged, this, &CheckDemo::stateSlot);
    connect(ui->checkBox_2, &QCheckBox::stateChanged, this, &CheckDemo::stateSlot);

}

CheckDemo::~CheckDemo()
{
    delete ui;
}
struct FilterData
{
    QList<QString> ecuIdFilter;
    QList<QString> appIdFilter;
    QList<QString> ctidFilters;
    QList<QString> payloadFilters;
};

void CheckDemo::on_buttonBox_accepted()
{
    // Get the checked state of each checkbox
    bool ecuIdChecked = ui->checkBox->isChecked();
    bool apidChecked = ui->checkBox_2->isChecked();
    bool contextIdChecked = ui->checkBox_3->isChecked();
    bool payloadTextChecked = ui->checkBox_4->isChecked();

    // Get the filter values from line edits and date time edit
    QString ecuIdFilter = ui->lineEdit->text().trimmed();
    QString apidFilter = ui->lineEdit_2->text().trimmed();
    QString contextIdFilter = ui->lineEdit_3->text().trimmed();
    QString payloadTextFilter = ui->lineEdit_4->text().trimmed();
    QDateTime dateTimeFilter = ui->dateTimeEdit->dateTime(); // Get selected date and time

    // Filter JSON data based on selected checkboxes and filter values
    QJsonObject jsonData;

    QJsonArray filteredLogs;
    for (const QString& logKey : jsonData.keys())
    {
        QJsonObject logObject = jsonData.value(logKey).toObject();
        QDateTime logDateTime = QDateTime::fromString(logObject.value("Time").toString(), "yyyy/MM/dd  hh:mm:ss.zzz000");

        if ((!ecuIdChecked || logObject.value("Ecu_id").toString() == ecuIdFilter) &&
            (!apidChecked || logObject.value("Apid").toString() == apidFilter) &&
            (!contextIdChecked || logObject.value("Ctid").toString() == contextIdFilter) &&
            (!payloadTextChecked || logObject.value("Payload").toString().contains(payloadTextFilter)) &&
            (logDateTime >= dateTimeFilter)) // Check date and time filter
        {
            filteredLogs.append(logObject);
        }
    }
    FilterData filters;
    // Populate filters with appropriate data
    emit ecuApidFiltersChangedSignal(filteredLogs);
    // Emit a signal with the filtered data to communicate with MainWindow
}






void CheckDemo::stateSlot(int state)
{
    // Get the checked state of each checkbox
    bool showEcuChecked = ui->checkBox->isChecked();
    bool showApidChecked = ui->checkBox_2->isChecked();

    // Initialize QStringList to store the filters for Ecu_id and Apid
    QStringList ecuIdFilter;
    QStringList appIdFilter;
    QStringList ctidFilters;
    QStringList payloadFilters;

    QJsonArray filteredLogs;

    // Check the state of each checkbox and add the corresponding filter texts
    if (showEcuChecked)
        ecuIdFilter << "Filter for Ecu_id"; // Replace with your desired Ecu_id filter text
    if (showApidChecked)
        appIdFilter << "Filter for Apid"; // Replace with your desired Apid filter text



    emit ecuApidFiltersChangedSignal(filteredLogs);
}

void CheckDemo::setFilterText(const QString &text)
{
    // Set the text to your line edits or perform filtering logic
    ui->lineEdit->setText(text);
    ui->lineEdit_2->setText(text);
}

void CheckDemo::on_checkBox_stateChanged(int arg1)
{
    stateSlot(arg1);
}

void CheckDemo::on_checkBox_2_stateChanged(int arg1)
{
    stateSlot(arg1);
}



