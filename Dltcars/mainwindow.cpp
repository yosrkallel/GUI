#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include <QCoreApplication>
#include <QFile>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QJsonParseError>
#include <QIcon>
#include "dialog.h"
#include "checkdemo.h"
#include <QSortFilterProxyModel>
#include <QString>
#include <QMessageBox>
#include <QLineEdit>
#include <QHeaderView>
#include <QApplication>
#include <ranges>
#include <vector>
#include <algorithm>
#include <QList>
#include <QListView>
#include <iostream>
#include <QMainWindow>
#include <QTextEdit>
#include <QTreeView>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLineEdit>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DLTCars)
{
    ui->setupUi(this);
    QString spath = "C:/Users/bji/Downloads/output_file.json";
    dmodel =new QFileSystemModel(this);
    dmodel ->setFilter (QDir::NoDotAndDotDot|QDir::AllDirs);
    dmodel ->setRootPath(spath);
    ui->treeView->setModel(dmodel);
    fmodel = new QFileSystemModel(this);
    fmodel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    // Set the name filter to show only JSON files
    fmodel->setNameFilters(QStringList() << "*.json");
    fmodel->setNameFilterDisables(false); // Show only files that match the name filter
    ui->listView->setModel(fmodel);
   /* fmodel = new QFileSystemModel(this);
    dmodel ->setFilter (QDir::NoDotAndDotDot|QDir::Files);
    ui->listView->setModel(fmodel);
*/
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *fileMenu = menuBar->addMenu("File");
    QMenu *filterMenu = menuBar->addMenu("Filter");

    QAction *addAction = filterMenu->addAction("Add");
    connect(addAction, &QAction::triggered, this, &MainWindow::on_actionAdd_triggered);
    addAction->setIcon(QIcon(":/images/add1"));

    /*QAction *addAction = filterMenu->addAction("Add");
    connect(addAction, &QAction::triggered, this, &MainWindow::actionAdd);*/

    QAction *openAction = fileMenu->addAction("Open");
    connect(openAction, &QAction::triggered, this, &MainWindow::actionOpen);
    openAction->setIcon(QIcon(":/images/open.png"));

    QAction *saveAction = fileMenu->addAction("Save");
    connect(saveAction, &QAction::triggered, this, &MainWindow::actionSave);
    saveAction->setIcon(QIcon(":/images/save.png"));
    //connect(ui->treeView, &QTreeView::doubleClicked, this, &MainWindow::actionOpen);

    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::treeViewClicked);

    connect(ui->listView, &QListView::clicked, this, &MainWindow::listViewclicked);
  //  connect(ui->filterButton, &QPushButton::clicked, this, &MainWindow::on_filterButton_clicked);
    ui->toolBar->addAction(openAction);
    ui->toolBar->addAction(saveAction);
    ui->toolBar->addAction(addAction);

    CheckDemo *checkDemo = new CheckDemo(this);

    // Connecter le signal de CheckDemo au slot de MainWindow
    connect(checkDemo, &CheckDemo::ecuApidFiltersChangedSignal, this, &MainWindow::applyFilter);

    // Create an instance of the CheckDemo dialog
    checkDemoDialog = new CheckDemo(this);

    // Connect the ecuApidFiltersChangedSignal signal from the dialog to the applyFilter slot in the main window
    connect(checkDemoDialog, &CheckDemo::ecuApidFiltersChangedSignal, this, &MainWindow::applyFilter);

  // debugTextEdit = findChild<QTextEdit*>("debugTextEdit");

    // ... Le reste du code ...
    layout()->setMenuBar(menuBar);
}
void MainWindow::listViewclicked(const QModelIndex &index)
{
    // Get the file path from the model index
    QString filePath = fmodel->fileInfo(index).absoluteFilePath();

    // Read and display the JSON file
    ReadJson(filePath);
}
void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    QString spath = dmodel ->fileInfo(index).absoluteFilePath();
    ui->treeView->setRootIndex(fmodel->setRootPath(spath));
}
// Dans MainWindow
void MainWindow::on_actionAdd_triggered()
{
    // Créer une instance de la fenêtre CheckDemo
    CheckDemo *checkDemo = new CheckDemo(this);

    // Connecter le signal de CheckDemo au slot de MainWindow pour le filtrage
    connect(checkDemo, &CheckDemo::ecuApidFiltersChangedSignal, this, &MainWindow::applyFilter);

    // Ouvrir la fenêtre CheckDemo en tant que boîte de dialogue modale
    checkDemo->exec();
}

void MainWindow::actionOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open JSON File", "", "JSON Files (*.json)");

    if (fileName.isEmpty())
        return;

    ReadJson(fileName);
    emit jsonFileOpened(fileName);
}

void MainWindow::treeViewClicked(const QModelIndex &index)
{
    QString filePath = dmodel->fileInfo(index).absoluteFilePath();
    QFileInfo fileInfo(filePath);

    // Check if the selected item is a directory or a JSON file
    if (fileInfo.isDir()) {
        // If it's a directory, set the root path for the QListView's model
        fmodel->setRootPath(filePath);
        ui->listView->setRootIndex(fmodel->index(filePath));
    } else if (fileInfo.suffix() == "json") {
        // If it's a JSON file, read and display its content
        ReadJson(filePath);
    }
}
void MainWindow::applyFilter(const QJsonArray& filteredData)
{
    // Clear the existing table contents
    ui->filteredTableWidget->clearContents();
    ui->filteredTableWidget->setRowCount(0);

    // Determine the number of columns and update the table widget
    if (!filteredData.isEmpty())
    {
        QJsonObject logObject = filteredData.at(0).toObject();
        int numColumns = logObject.keys().size();
        ui->filteredTableWidget->setColumnCount(numColumns);

        for (int i = 0; i < filteredData.size(); ++i)
        {
            QJsonObject logObject = filteredData.at(i).toObject();

            // Add log data to the table widget
            QStringList keys = logObject.keys();
            for (int col = 0; col < numColumns; ++col)
            {
                QTableWidgetItem *item = new QTableWidgetItem(logObject.value(keys.at(col)).toString());
                ui->filteredTableWidget->setItem(i, col, item);
            }
        }
    }
}

/*
void MainWindow::applyFilter(const QStringList &ecuFilters, const QStringList &apidFilters,
                             const QStringList &ctidFilters, const QStringList &payloadFilters)
{
    // Get the filter text from the QLineEdit
    QString filterText = ui->filterLineEdit->text().trimmed(); // Trim leading/trailing spaces

    // Clear the filteredTableWidget and set the same header labels
    ui->filteredTableWidget->clearContents();
    ui->filteredTableWidget->setRowCount(0);
    int columnCount = 8;
    ui->filteredTableWidget->setColumnCount(columnCount);
    int rowCount = ui->tableWidget->rowCount();

    // Set table headers
    QStringList headerLabels;
    headerLabels << "Log Key" << "Type" << "Time" << "Time_stamp" << "Ecu_id" << "Apid" << "Ctid" << "Payload";
    ui->filteredTableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->filteredTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QList<int> filterlist;

    // Loop through the rows of the original table and check if any cell contains the filter text
    for (int row = 0; row < rowCount; row++)
    {
        bool rowMatchesFilter = false;
        for (int col = 0; col < columnCount; col++)
        {
            QTableWidgetItem* item = ui->tableWidget->item(row, col);
            if (item && item->text().contains(filterText, Qt::CaseInsensitive))
            {
                rowMatchesFilter = true;
                break; // No need to continue checking other columns in this row
            }
        }
        if (rowMatchesFilter)
        {
            filterlist.append(row);
        }
    }

    // Filter based on ecuFilters, apidFilters, ctidFilters and payloadFilters
    for (int i = filterlist.size() - 1; i >= 0; --i)
    {
        int rowIndex = filterlist[i];
        QTableWidgetItem* ecuItem = ui->tableWidget->item(rowIndex, 4); // Assuming Ecu_id column is at index 4
        QTableWidgetItem* apidItem = ui->tableWidget->item(rowIndex, 5); // Assuming Apid column is at index 5
        QTableWidgetItem* ctidItem = ui->tableWidget->item(rowIndex, 6); // Assuming Ctid column is at index 6
        QTableWidgetItem* payloadItem = ui->tableWidget->item(rowIndex, 7); // Assuming Payload column is at index 7

        bool ecuMatched = ecuFilters.isEmpty() || ecuFilters.contains(ecuItem->text());
        bool apidMatched = apidFilters.isEmpty() || apidFilters.contains(apidItem->text());
        bool ctidMatched = ctidFilters.isEmpty() || ctidFilters.contains(ctidItem->text());
        bool payloadMatched = payloadFilters.isEmpty() || payloadFilters.contains(payloadItem->text());

        if (!ecuMatched || !apidMatched || !ctidMatched || !payloadMatched)
        {
            filterlist.removeAt(i);
        }
    }

    // Clear and set up the filteredTableWidget
    ui->filteredTableWidget->clearContents();
    ui->filteredTableWidget->setRowCount(0);
    ui->filteredTableWidget->setColumnCount(columnCount);
    ui->filteredTableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->filteredTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Populate the filteredTableWidget with the filtered log objects
    for (int i = 0; i < filterlist.size(); ++i)
    {
        int rowIndex = filterlist[i];
        if (rowIndex < rowCount)
        {
            int newRow = ui->filteredTableWidget->rowCount();
            ui->filteredTableWidget->insertRow(newRow);

            for (int col = 0; col < columnCount; col++)
            {
                QTableWidgetItem* originalItem = ui->tableWidget->item(rowIndex, col);
                if (originalItem)
                {
                    QTableWidgetItem* newItem = new QTableWidgetItem(originalItem->text());
                    ui->filteredTableWidget->setItem(newRow, col, newItem);
                }
            }
        }
    }
}


void MainWindow::onEcuApidFiltersChanged(const QStringList &ecuFilters, const QStringList &apidFilters)
{
    // Implement the filtering logic here based on the ecuFilters and apidFilters
    // For example, update the filteredTableWidget according to the selected filters

    // Clear the existing contents of the filteredTableWidget
    ui->filteredTableWidget->clearContents();
    ui->filteredTableWidget->setRowCount(0);

    // Get the number of rows in the original tableWidget
    int rowCount = ui->tableWidget->rowCount();

    // Loop through the rows of the original tableWidget
    for (int row = 0; row < rowCount; ++row)
    {
        QString ecuId = ui->tableWidget->item(row, 4)->text();
        QString apid = ui->tableWidget->item(row, 5)->text();

        // Check if the ecuId and apid match the selected filters
        if (ecuFilters.contains(ecuId) && apidFilters.contains(apid))
        {
            // If both filters match, add a new row to the filteredTableWidget
            int newRow = ui->filteredTableWidget->rowCount();
            ui->filteredTableWidget->insertRow(newRow);

            // Copy the content of each cell from the original tableWidget to the filteredTableWidget
            for (int col = 0; col < ui->tableWidget->columnCount(); ++col)
            {
                QTableWidgetItem *item = ui->tableWidget->item(row, col);
                QTableWidgetItem *clonedItem = item->clone();
                ui->filteredTableWidget->setItem(newRow, col, clonedItem);
            }
        }
    }
}


// Add the applyFilter function to handle the filtering
void MainWindow::applyFilter(const QString &ecuIdFilter, const QString &appIdFilter)
{
    ui->filteredTableWidget->clearContents();
    ui->filteredTableWidget->setRowCount(0);

    int filteredRowCount = 0;
    for (int row = 0; row < ui->tableWidget->rowCount(); row++)
    {
        QString ecuId = ui->tableWidget->item(row, 4)->text(); // Assuming column 4 contains the Ecu_id
        QString appId = ui->tableWidget->item(row, 5)->text(); // Assuming column 5 contains the Apid

        if ((ecuIdFilter.isEmpty() || ecuId.contains(ecuIdFilter, Qt::CaseInsensitive)) &&
            (appIdFilter.isEmpty() || appId.contains(appIdFilter, Qt::CaseInsensitive)))
        {
            ui->filteredTableWidget->setRowCount(filteredRowCount + 1);
            for (int col = 0; col < ui->tableWidget->columnCount(); col++)
            {
                QTableWidgetItem* item = new QTableWidgetItem(ui->tableWidget->item(row, col)->text());
                ui->filteredTableWidget->setItem(filteredRowCount, col, item);
            }
            filteredRowCount++;
        }
    }

    // Adjust the width of the columns based on the content
    ui->filteredTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
*/
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onLoginAction()
{
    // Create the login dialog
    Dialog dialog;
    int result = dialog.exec(); // Show the login dialog as a modal dialog

    // Check the result of the dialog (accepted or rejected)
    if (result == QDialog::Accepted)
    {
        // If the login was successful, show the main window
        show();
    }
}
void MainWindow::actionAdd()
{
    // Créez une instance de la fenêtre CheckDemo
    CheckDemo checkDemoDialog;

    // Affichez la fenêtre de dialogue modale
    checkDemoDialog.exec();
}
/*void MainWindow::actionAdd()
{
    QDialog *dialog = new QDialog();
    dialog ->setMinimumHeight(240);
    dialog ->setMinimumWidth(350);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(dialog);
    dialog->setLayout(layout);

    dialog ->show();

}*/
void MainWindow::ReadJson(const QString &path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray bytes = file.readAll();
        file.close();

        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);
        if (jsonError.error != QJsonParseError::NoError)
        {
            qDebug() << "fromJson failed: " << jsonError.errorString();
            return;
        }

        if (document.isObject())
        {
            jsonObject = document.object(); // Assign the JSON object to the member variable

        if (jsonObject.contains("logging"))
        {
            QJsonObject loggingObject = jsonObject["logging"].toObject();
            QStringList logKeys = loggingObject.keys();
            //rendre en general le nb col et de lig
            int maxRowCount = 60;
            int rowCount = qMin(maxRowCount, logKeys.size());
            ui->tableWidget->setRowCount(rowCount);
            int columnCount = 8;
            ui->tableWidget->setColumnCount(columnCount);

            // Set table headers
            QStringList headerLabels;
            headerLabels << "Log Key" << "Type" << "Time" << "Time_stamp" << "Ecu_id" << "Apid" << "Ctid" << "Payload";
            ui->tableWidget->setHorizontalHeaderLabels(headerLabels);

            int row = 0;
            for (const auto &logKey : logKeys)
            {
                QJsonObject logObject = loggingObject[logKey].toObject();
                ui->tableWidget->setItem(row, 0, new QTableWidgetItem(logKey));
                ui->tableWidget->setItem(row, 1, new QTableWidgetItem(logObject["Type"].toString()));
                ui->tableWidget->setItem(row, 2, new QTableWidgetItem(logObject["Time"].toString()));
                ui->tableWidget->setItem(row, 3, new QTableWidgetItem(logObject["Time_stamp"].toString()));
                ui->tableWidget->setItem(row, 4, new QTableWidgetItem(logObject["Ecu_id"].toString()));
                ui->tableWidget->setItem(row, 5, new QTableWidgetItem(logObject["Apid"].toString()));
                ui->tableWidget->setItem(row, 6, new QTableWidgetItem(logObject["Ctid"].toString()));
                ui->tableWidget->setItem(row, 7, new QTableWidgetItem(logObject["Payload"].toString()));

                row++;
            }
            // Ajuster la largeur des colonnes en fonction du contenu
            ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        }
        }
    }
}

/*void MainWindow::actionOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open JSON File", "", "JSON Files (*.json)");

    if (fileName.isEmpty())
        return;

    ReadJson(fileName);
    emit jsonFileOpened(fileName);
}*/
/*void MainWindow::actionOpen()
{
    // Get the selected index from the tree view
    QModelIndex index = ui->treeView->currentIndex();

    // Check if the index is valid and represents a file
    if (index.isValid() && !dmodel->isDir(index))
    {
        // Get the directory path of the double-clicked file
        QString directoryPath = dmodel->fileInfo(index).absolutePath();

        // Open the file dialog starting from the directory of the double-clicked file
        QString fileName = QFileDialog::getOpenFileName(this, "Open JSON File", directoryPath, "JSON Files (*.json)");

        if (fileName.isEmpty())
        return;

        ReadJson(fileName);
        emit jsonFileOpened(fileName);
    }
}*/
void MainWindow::actionSave()
{
    int rowCount = ui->tableWidget->rowCount();
    int columnCount = ui->tableWidget->columnCount();

    for (int row = 0; row < rowCount; row++)
    {
        for (int col = 0; col < columnCount; col++)
        {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            if (item)
            {
                qDebug() << "Row:" << row << "Column:" << col << "Value:" << item->text();
            }
        }
    }
}


void MainWindow::on_filterButton_clicked()
{
    // Get the filter text from the QLineEdit
    QString filterText = ui->filterLineEdit->text().trimmed(); // Trim leading/trailing spaces
    qDebug() << "Filter Text:" << filterText;

    // Ensure that the filter key exists in the JSON object
    QString filterKey = "headerLabels"; // Assuming the filter key is "Type"
    if (!jsonObject.contains("logging"))
    {
        QMessageBox::warning(this, "Error", "Invalid JSON format: 'logging' key not found.");
        return;
    }

    // Get the array of log objects from the JSON object
    QJsonObject loggingObject = jsonObject["logging"].toObject();
    QStringList logKeys = loggingObject.keys();

    // Clear the filteredTableWidget and set the same header labels
    ui->filteredTableWidget->clearContents();
    ui->filteredTableWidget->setRowCount(0);
    int columnCount = 8;
    ui->filteredTableWidget->setColumnCount(columnCount);
    int rowCount = ui->tableWidget->rowCount();
    //int columnCount = ui->tableWidget->columnCount();
    // Set table headers
    QStringList headerLabels;
    headerLabels << "Log Key" << "Type" << "Time" << "Time_stamp" << "Ecu_id" << "Apid" << "Ctid" << "Payload";
    ui->filteredTableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->filteredTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //creation de filteredLogs:

    QList<int> filterlist;

    // Check if the filterText is empty, which means no filtering is required
    if (filterText.isEmpty())
    {
        for (int row = 0; row < rowCount; row++)
        {
            filterlist.append(row);
        }
    }
    else
    {
        // Loop through the rows of the original table and check if any cell contains the filter text
        for (int row = 0; row < rowCount; row++)
        {
            for (int col = 0; col < columnCount; col++)
            {
                QTableWidgetItem* item = ui->tableWidget->item(row, col);
                if (item && item->text().contains(filterText, Qt::CaseInsensitive))
                {
                filterlist.append(row);
                break; // No need to continue checking other columns in this row
                }
            }
        }
    }

    // Clear the filteredTableWidget and set the same header labels
    ui->filteredTableWidget->clearContents();
    ui->filteredTableWidget->setRowCount(0);
    ui->filteredTableWidget->setColumnCount(columnCount);
    ui->filteredTableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->filteredTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    int row=0;
    // Populate the filteredTableWidget with the filtered log objects
    for (int i = 0; i < filterlist.size(); ++i)
    {
        int rowIndex = filterlist[i]; // Get the row index from filterlist
        if (rowIndex < rowCount)
        {
            // Add a new row to the filteredTableWidget at the end
            int newRow = ui->filteredTableWidget->rowCount();
            ui->filteredTableWidget->insertRow(newRow);

            for (int col = 0; col < columnCount; col++)
            {
                QTableWidgetItem* originalItem = ui->tableWidget->item(rowIndex, col);
                if (originalItem)
                {
                // Get the text from the original table and create a new item for the filteredTableWidget
                QTableWidgetItem* newItem = new QTableWidgetItem(originalItem->text());
                ui->filteredTableWidget->setItem(newRow, col, newItem);
                }
            }
        }
    }

}




