#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include "src/IniFile.h"
#include <QModelIndex>
#include <QInputDialog>
#include <QMessageBox>
#include <map>
#include <QValidator>

enum type {
    _int,
    _double,
    _bool,
    _string,
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    //      void getFolderList(QString folderPath, QFileInfoList *dirList);
    ~MainWindow();

private slots:
    void on_open_file_triggered();

    void on_open_directory_triggered();

    void on_save_file_triggered();

    //void on_comboBox_type1_currentIndexChanged(int index);

    void on_listWidget_file_clicked(const QModelIndex &index);

    void on_pushButton_add_section_clicked();

    void on_pushButton_delete_section_clicked();

    void on_comboBox_section_currentTextChanged(const QString &arg1);

    void on_pushButton_add_key_clicked();

    void on_pushButton_delete_key_clicked();

    void on_treeWidget_file_itemSelectionChanged();

    void on_action_triggered();

    void on_comboBox_type1_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    std::map<std::string, IniFile> mapa;
    QIntValidator* intValid;
    QDoubleValidator* doubleValid;
    QRegExpValidator* boolValidator;
};
#endif // MAINWINDOW_H
