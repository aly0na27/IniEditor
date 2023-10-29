#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    ui->groupBox->setEnabled(false);
    ui->groupBox_2->setEnabled(false);
    intValid = new QIntValidator(this);
    doubleValid = new QDoubleValidator(this);
    boolValidator = new QRegExpValidator(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}

int c = 0;

void MainWindow::on_open_file_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Открыть", "", "*.ini");
    QList<QListWidgetItem *> found = ui->listWidget_file->findItems(filename, Qt::MatchWildcard);//возвращает пустой лист
    if (!found.isEmpty() && !filename.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Файл уже открыт!");
        msgBox.exec();
    }
    else if (filename.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Вы не выбрали файл!");
        msgBox.exec();
    }
    else {
        ui->listWidget_file->addItem(filename);
    }
}
void MainWindow::on_open_directory_triggered()
{
    QString dir_list = QFileDialog::getExistingDirectory();
    QDir dir(dir_list);
    QStringList nameFilter;
    nameFilter << "*.ini";
    dir.setNameFilters(nameFilter);
    QFileInfoList list = dir.entryInfoList();

    if (dir_list.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Вы не выбрали директорию!");
        msgBox.exec();
        return;
    }
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QString s = fileInfo.absoluteFilePath();
        QList<QListWidgetItem *> found = ui->listWidget_file->findItems(s, Qt::MatchWildcard);
        if (!found.isEmpty()) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Warning");
            QString s1 = "Файл с именем ";
            QString s2 = " уже открыт!";
            msgBox.setText(s1 + s + s2);
            msgBox.exec();
        }
        else {
            ui->listWidget_file->addItem(s);
        }
    }
}


void MainWindow::on_save_file_triggered()
{

    QList<QListWidgetItem*> l = ui->listWidget_file->selectedItems();
    if (l.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Вы не выбрали файл!");
        msgBox.exec();
        return;
    }
    QString filename = l.last()->text();
    auto i = mapa.find(filename.toStdString());
    delete ui->listWidget_file->takeItem(ui->listWidget_file->row(l.last()));
    delete l.last();
    i->second.save();
    ui->treeWidget_file->clear();
    ui->groupBox->setEnabled(false);
    ui->groupBox_2->setEnabled(false);
    c--;
    QString s1 = "Open files: ";
    QString s2 = QString::number(ui->listWidget_file->count());
    ui->statusbar->showMessage(s1+s2);
}

void MainWindow::on_action_triggered()
{
    bool ok;
    QString name_file = QInputDialog::getText(this, tr("Название файла"), tr("Введите название файла:"),  QLineEdit::Normal, QDir::home().dirName(), &ok);
    if (ok && !name_file.isEmpty()) {
        QFile file(name_file);
        ui->listWidget_file->addItem(name_file);
    }
    else {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Вы не ввели название файла!");
        msgBox.exec();
    }
}

void MainWindow::on_listWidget_file_clicked(const QModelIndex &index)
{
    if (ui->groupBox_2->isEnabled() == false && ui->groupBox->signalsBlocked() == false) {
        ui->groupBox_2->setEnabled(true);
        ui->groupBox->setEnabled(true);
    }
    ui->treeWidget_file->clear();
    ui->comboBox_section->clear();
    ui->comboBox_type1->clear();

    QString str = (ui->listWidget_file->selectedItems().last())->text();
    std::map<std::string, IniFile>::iterator i;
    SectionsMap sections;
    if (mapa.find(str.toStdString()) == mapa.end()) {
        IniFile file(str.toStdString());
        std::pair<std::string, IniFile> para = std::make_pair(str.toStdString(), file);
        mapa.insert(para);
        c++;
        sections = file.getSections();
    }
    else {
        i = mapa.find(str.toStdString());

        sections =  i->second.getSections();
    }
    for (auto it = sections.begin(); it != sections.end(); it++) {
        ui->comboBox_section->addItem(QString::fromStdString(it->first));
        QTreeWidgetItem* itm = new QTreeWidgetItem(ui->treeWidget_file);
        itm->setText(0, QString::fromStdString(it->first));
        QList<QTreeWidgetItem*> child_list;
        for (auto jt = it->second.begin(); jt != it->second.end(); jt++) {
            QTreeWidgetItem* child = new QTreeWidgetItem(itm);
            child->setText(0, QString::fromStdString((jt->first)));
            child->setText(1, QString::fromStdString((jt->second)));
            child_list.append(child);
        }

        itm->addChildren(child_list);
        ui->treeWidget_file->addTopLevelItem(itm);
        ui->treeWidget_file->expandAll();
    }

    for (int i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                ui->comboBox_type1->addItem("int");
                break;
            case 1:
                ui->comboBox_type1->addItem("double");
                break;
            case 2:
                ui->comboBox_type1->addItem("bool");
                break;
            case 3:
                ui->comboBox_type1->addItem("string");
                break;
        }
    }
    QString s = "Open files: ";
    QString s1 = QString::number(c);
    ui->statusbar->showMessage(s+s1);
}

void MainWindow::on_pushButton_add_section_clicked()
{
    bool ok;
    if (c == 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Вы не выбрали файл!");
        msgBox.exec();
        return;
    }
    QString name_section = QInputDialog::getText(this, tr("Добавление секции"), tr("Введите название секции:"),  QLineEdit::Normal, QDir::home().dirName(), &ok);
    if (ok && !name_section.isEmpty()) {
        QString namefile = (ui->listWidget_file->selectedItems().last())->text();
        auto i = mapa.find(namefile.toStdString());
        i->second.writeSection(name_section.toStdString());

        ui->treeWidget_file->clear();
        ui->comboBox_section->clear();
        ui->comboBox_type2->clear();

        SectionsMap sections = i->second.getSections();

        for (auto it = sections.begin(); it != sections.end(); it++) {
            ui->comboBox_section->addItem(QString::fromStdString(it->first));
            QTreeWidgetItem* itm = new QTreeWidgetItem(ui->treeWidget_file);
            itm->setText(0, QString::fromStdString(it->first));
            QList<QTreeWidgetItem*> child_list;
            for (auto jt = it->second.begin(); jt != it->second.end(); jt++) {
                QTreeWidgetItem* child = new QTreeWidgetItem(itm);
                child->setText(0, QString::fromStdString((jt->first)));
                child->setText(1, QString::fromStdString((jt->second)));
                child_list.append(child);
            }
            itm->addChildren(child_list);
            ui->treeWidget_file->addTopLevelItem(itm);
            ui->treeWidget_file->expandAll();
        }
    }
}

void MainWindow::on_pushButton_delete_section_clicked()
{
    if (c == 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Вы не выбрали файл!");
        msgBox.exec();
        return;
    }
    QString namefile = (ui->listWidget_file->selectedItems().last())->text();
    QString name_section = ui->comboBox_section->currentText();
    auto i = mapa.find(namefile.toStdString());
    i->second.deleteSection(name_section.toStdString());

    ui->treeWidget_file->clear();
    ui->comboBox_section->clear();
    ui->comboBox_type2->clear();

    SectionsMap sections = i->second.getSections();

    for (auto it = sections.begin(); it != sections.end(); it++) {
        ui->comboBox_section->addItem(QString::fromStdString(it->first));
        QTreeWidgetItem* itm = new QTreeWidgetItem();
        itm->setText(0, QString::fromStdString(it->first));
        QList<QTreeWidgetItem*> child_list;
        for (auto jt = it->second.begin(); jt != it->second.end(); jt++) {
            QTreeWidgetItem* child = new QTreeWidgetItem();
            child->setText(0, QString::fromStdString((jt->first)));
            child->setText(1, QString::fromStdString((jt->second)));
            child_list.append(child);
        }
        itm->addChildren(child_list);
        ui->treeWidget_file->addTopLevelItem(itm);
        ui->treeWidget_file->expandAll();
    }

}
void MainWindow::on_comboBox_section_currentTextChanged(const QString &arg1)
{
    ui->comboBox_type2->clear();
    QString name_section = ui->comboBox_section->currentText();
    QString namefile = (ui->listWidget_file->selectedItems().last())->text();
    auto i = mapa.find(namefile.toStdString());
    SectionsMap sections = i->second.getSections();

    for (auto it = sections.begin(); it != sections.end(); it++) {
        if (it->first == arg1.toStdString()) {
            for (auto jt = it->second.begin(); jt != it->second.end(); jt++) {
                ui->comboBox_type2->addItem(QString::fromStdString((jt->first)));
            }
        }
    }
}


void MainWindow::on_pushButton_add_key_clicked()
{
    QString namefile = (ui->listWidget_file->selectedItems().last())->text();
    auto i = mapa.find(namefile.toStdString());
    QString name_key = ui->lineEdit_key->text();
    QString value = ui->lineEdit_value->text();
    KeysMap k;
    if (!name_key.isEmpty() && !value.isEmpty() && !ui->comboBox_section->currentText().isEmpty()) {
        switch (ui->comboBox_type1->currentIndex()) {
        case 0:
            i->second.writeInt((ui->comboBox_section->currentText()).toStdString(), name_key.toStdString(), value.toInt());
            break;
        case 1:
            i->second.writeDouble((ui->comboBox_section->currentText()).toStdString(), name_key.toStdString(), value.toDouble());
            break;
        case 2:
            if (value.toStdString() == trueValues[0] || value.toStdString() == trueValues[1] || value.toStdString() == trueValues[2] || value.toStdString() == trueValues[3]) {
                i->second.writeBool((ui->comboBox_section->currentText()).toStdString(), name_key.toStdString(), 1);
            }
            else {
                i->second.writeBool((ui->comboBox_section->currentText()).toStdString(), name_key.toStdString(), 0);
            }
            break;
        case 3:
            i->second.writeString((ui->comboBox_section->currentText()).toStdString(), name_key.toStdString(), value.toStdString());
            break;
        }
        ui->treeWidget_file->clear();
        ui->comboBox_section->clear();
        ui->comboBox_type2->clear();

        SectionsMap sections = i->second.getSections();

        for (auto it = sections.begin(); it != sections.end(); it++) {
            ui->comboBox_section->addItem(QString::fromStdString(it->first));
            QTreeWidgetItem* itm = new QTreeWidgetItem(ui->treeWidget_file);
            itm->setText(0, QString::fromStdString(it->first));
            QList<QTreeWidgetItem*> child_list;
            for (auto jt = it->second.begin(); jt != it->second.end(); jt++) {
                QTreeWidgetItem* child = new QTreeWidgetItem(itm);
                child->setText(0, QString::fromStdString((jt->first)));
                child->setText(1, QString::fromStdString((jt->second)));
                child_list.append(child);
            }
            itm->addChildren(child_list);
            ui->treeWidget_file->addTopLevelItem(itm);
            ui->treeWidget_file->expandAll();
        }

    }
    else if (ui->comboBox_section->currentText().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Вы не выбрали секцию!");
        msgBox.exec();
    }
    else if (name_key.isEmpty() && !value.isEmpty()){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Вы не ввели название ключа!");
        msgBox.exec();
    }
    else if (!name_key.isEmpty() && value.isEmpty()){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Вы не ввели значение!");
        msgBox.exec();
    }
    else {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Вы не ввели название ключа и значение!");
        msgBox.exec();
    }
    ui->lineEdit_key->clear();
    ui->lineEdit_value->clear();
    ui->lineEdit_value->setValidator(0);
}


void MainWindow::on_pushButton_delete_key_clicked()
{
    if (ui->comboBox_section->currentText().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Вы не выбрали секцию!");
        msgBox.exec();
    }
    QString namefile = (ui->listWidget_file->selectedItems().last())->text();
    auto i = mapa.find(namefile.toStdString());
    QString name_key = ui->comboBox_type2->currentText();
    i->second.deleteKey((ui->comboBox_section->currentText()).toStdString(), name_key.toStdString());

    ui->treeWidget_file->clear();
    ui->comboBox_section->clear();
    ui->comboBox_type2->clear();

    SectionsMap sections = i->second.getSections();

    for (auto it = sections.begin(); it != sections.end(); it++) {
        ui->comboBox_section->addItem(QString::fromStdString(it->first));
        QTreeWidgetItem* itm = new QTreeWidgetItem(ui->treeWidget_file);
        itm->setText(0, QString::fromStdString(it->first));
        QList<QTreeWidgetItem*> child_list;
        for (auto jt = it->second.begin(); jt != it->second.end(); jt++) {
            QTreeWidgetItem* child = new QTreeWidgetItem(itm);
            child->setText(0, QString::fromStdString((jt->first)));
            child->setText(1, QString::fromStdString((jt->second)));
            child_list.append(child);
        }
        itm->addChildren(child_list);
        ui->treeWidget_file->addTopLevelItem(itm);
        ui->treeWidget_file->expandAll();
    }
}


void MainWindow::on_treeWidget_file_itemSelectionChanged()
{
    QString namefile = (ui->listWidget_file->selectedItems().last())->text();
    auto i = mapa.find(namefile.toStdString());
    QTreeWidgetItem* l = ui->treeWidget_file->currentItem();
    if (l->text(1).isEmpty()) {
        ui->comboBox_section->setCurrentText(l->text(0));
    }
    else {
        ui->comboBox_section->setCurrentText(l->parent()->text(0));
        ui->comboBox_type2->setCurrentText(l->text(0));
    }
}

void MainWindow::on_comboBox_type1_currentTextChanged(const QString &arg1)
{
    ui->lineEdit_key->clear();
    ui->lineEdit_value->clear();

    if (arg1 == "int") {
        ui->lineEdit_value->setValidator(intValid);
    }
    else if (arg1 == "double") {
        ui->lineEdit_value->setValidator(doubleValid);
    }
    else if (arg1 == "bool") {
        ui->lineEdit_value->setValidator(boolValidator);
    }
    else {
        ui->lineEdit_value->setValidator(0);
    }
}

