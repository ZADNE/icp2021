/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#include "mainmenu.h"
#include "ui_mainmenu.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu){
    ui->setupUi(this);

    //Connect signals from library explorer
    connect(ui->libraryExplorer, &LibraryExplorer::addNew,
            this, &MainMenu::addNew);
    connect(ui->libraryExplorer, &LibraryExplorer::editFile,
            this, &MainMenu::editFile);
    connect(ui->libraryExplorer, &LibraryExplorer::deleteFolder,
            this, &MainMenu::deleteFolder);
    connect(ui->libraryExplorer, &LibraryExplorer::deleteFile,
            this, &MainMenu::deleteFile);
}

MainMenu::~MainMenu(){
    delete ui;
}

void MainMenu::on_actionOpenLib_triggered(){
    QString folder = QFileDialog::getExistingDirectory(this,
        tr("Open a library"), QDir::currentPath());

    if (!folder.isEmpty()){//If a folder was selected
        emit saveWork();
        ui->libraryExplorer->loadLibrary(folder);
    }
}

void MainMenu::on_actionExit_triggered(){
    emit saveWork();
    QApplication::exit(0);
}

void MainMenu::addNew(QString folderPath){
    qDebug() << "Add new: " << folderPath;
}

void MainMenu::editFile(QString filePath){
    qDebug() << "Edit file: " << filePath;
}

void MainMenu::deleteFolder(QString folderPath){
    int clicked = deleteQuestion(
                tr("Delete folder?"),
                tr("Are you sure you want to delete the following folder and all its contents?"),
                folderPath);

    if (clicked == QMessageBox::Yes){
        //User really wants to delete this folder
        QDir dir{folderPath};
        dir.removeRecursively();
        ui->libraryExplorer->reloadLibrary();
    }
}

void MainMenu::deleteFile(QString filePath){
    int clicked = deleteQuestion(
                tr("Delete file?"),
                tr("Are you sure you want to delete the following file?"),
                filePath);

    if (clicked == QMessageBox::Yes){
        //User really wants to delete this file
        QFile::remove(filePath);
        QFile file{filePath};
        ui->libraryExplorer->reloadLibrary();
    }
}

int MainMenu::deleteQuestion(QString title, QString text, QString path){
    return QMessageBox::question(
                this, title,
                text + "\n\n" + path,
                QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);
}
