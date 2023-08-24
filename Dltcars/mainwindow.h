#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qjsonobject.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QSortFilterProxyModel>
#include <QTextEdit>
#include <QFileSystemModel>
#include <QTreeView>
#include "checkdemo.h"

#include <QStringList>



QT_BEGIN_NAMESPACE
namespace Ui { class DLTCars; }
QT_END_NAMESPACE
#include <QTableWidget>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void on_actionAdd_triggered();
    void ReadJson(const QString &path);


private slots:
    void actionOpen();
    void actionSave();
    void actionAdd();
    void on_filterButton_clicked();
    void treeViewClicked(const QModelIndex &index);
    void listViewclicked(const QModelIndex &index);
    void on_treeView_doubleClicked(const QModelIndex &index);

public slots:
    void onLoginAction();

    void applyFilter(const QJsonArray &filteredData);

signals:
    void jsonFileOpened(const QString &path);
    void ecuApidFiltersChangedSignal(const QStringList &ecuFilters, const QStringList &apidFilters);


private:
    Ui::DLTCars *ui;
    QTextEdit* debugTextEdit;
    QFileSystemModel * dmodel;
    QFileSystemModel * fmodel;
    QJsonObject jsonObject;
    QLineEdit *filterLineEdit;
    QSortFilterProxyModel *filterProxyModel;
    QString ecuIdFilter;
    QString appIdFilter;

    CheckDemo* checkDemoDialog;


};



#endif // MAINWINDOW_H
