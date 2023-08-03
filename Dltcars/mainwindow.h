#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qjsonobject.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QSortFilterProxyModel>
#include <QTextEdit>
#include <QFileSystemModel>
#include <QTreeView>



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

    void ReadJson(const QString &path);
    //void ReadJson(const QString &path, const QString &filterKey = "", const QString &filterValue = "");
   // bool logObjectContainsFilterText(const QJsonObject &logObject, const QString &filterText) const;
    //void filtertable ();


private slots:
    void actionOpen();
    void actionSave();

    void on_treeView_doubleClicked(const QModelIndex &index);

public slots:
    void on_filterButton_clicked();

    void onLoginAction();


signals:
    void jsonFileOpened(const QString &path);

private:
    Ui::DLTCars *ui;
    QTextEdit* debugTextEdit;
    QFileSystemModel * dmodel;
    QFileSystemModel * fmodel;
    QJsonObject jsonObject;
    QLineEdit *filterLineEdit;
    QSortFilterProxyModel *filterProxyModel;



};



#endif // MAINWINDOW_H
