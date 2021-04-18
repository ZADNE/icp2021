/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#include "mainmenu.h"
#include "ui_mainmenu.h"

#include <string>

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>

#include "newfiledialog.h"
#include "blockcompiler.h"

MainMenu::MainMenu(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainMenu)  {
    ui->setupUi(this);

    //Connect signals from library explorer
    connect(ui->libraryExplorer, &LibraryExplorer::addNew,
            this, &MainMenu::addNew);
    connect(ui->libraryExplorer, &LibraryExplorer::editFile,
            ui->tabEditor, &TabEditor::editFile);
    connect(ui->libraryExplorer, &LibraryExplorer::renameThis,
            this, &MainMenu::saveWork);//Save work before renaming
    connect(ui->libraryExplorer, &LibraryExplorer::renameThis,
            this, &MainMenu::renameThis);
    connect(ui->libraryExplorer, &LibraryExplorer::deleteThis,
            this, &MainMenu::deleteThis);
    //Connect signals to tab editor
    connect(this, &MainMenu::saveWork,
            ui->tabEditor, &TabEditor::saveWork);

    updateNoTabLabel();

    //Convenience
    #ifdef QT_DEBUG
        openLibrary("exampleLib");
    #endif
}

MainMenu::~MainMenu(){
    delete ui;
}

void MainMenu::openLibrary(QString libpath){
    QFileInfo fi = QFileInfo{libpath};
    if (fi.isDir()){
        m_lib = fi;
        ui->tabEditor->closeAllTabs();
        updateNoTabLabel();
        ui->libraryExplorer->loadLibrary(m_lib.canonicalFilePath());
        BlockCompiler::bc().openLibrary(libpath.toUtf8().constData());
    }
}

void MainMenu::on_actionOpenLib_triggered(){
    QString folderName = QFileDialog::getExistingDirectory(this,
        tr("Open a library"), QDir::currentPath());

    if (!folderName.isEmpty()){//If a folder was selected
        openLibrary(folderName);
    }
}

void MainMenu::on_actionExit_triggered(){
    emit saveWork();
    QApplication::exit(0);
}

void MainMenu::addNew(QString path){
    NewFileDialog f{this, path};
    if (f.exec()){
        NewFileRequest r = f.getResult();
        bool success = false;
        if (r.type == FileType::dir){
            success = ui->tabEditor->createFolder(r.path);
        } else {
            success = ui->tabEditor->editFile(r.path);
        }
        if (success){
            ui->libraryExplorer->reloadLibrary();
        }
    }
}

void MainMenu::renameThis(QString path){
    QFileInfo fi{path};
    if (fi.isDir()){
        //Renaming directory
        bool renamingLib = fi == m_lib;
        QString newName = renameDialog(renamingLib ? tr("library"): tr("folder"), fi.fileName());
        if (!newName.isEmpty()){
            if (ui->tabEditor->renameFolder(fi.path(), fi.fileName(),
                    newName)){
                //Successfully renamed
                if (renamingLib){
                    openLibrary(fi.path() + "/" + newName);
                } else {
                    ui->libraryExplorer->reloadLibrary();
                }
            }
        }
    } else {
        //Renaming file
        QString newName = renameDialog(tr("block"), fi.completeBaseName());
        if (!newName.isEmpty()){
            if (ui->tabEditor->renameFile(
                        path, fi.path() + "/" + newName + "." + fi.suffix())){
                //Successfully renamed
                ui->libraryExplorer->reloadLibrary();
            }
        }
    }
}

void MainMenu::deleteThis(QString path){
    QFileInfo fi{path};
    if (fi.isDir()){
        //Removing directory
        int clicked = deleteQuestion(
            tr("Delete folder?"),
            tr("Are you sure you want to delete the following folder and all its contents?"),
            path);
        if (clicked == QMessageBox::Yes){
            //User really wants to delete this folder
            if (ui->tabEditor->deleteFolder(path)){
                ui->libraryExplorer->reloadLibrary();
            }
        }
    } else {
        //Removing file
        int clicked = deleteQuestion(
            tr("Delete file?"),
            tr("Are you sure you want to delete the following file?"),
            path);
        if (clicked == QMessageBox::Yes){
            //User really wants to delete this file
            if (ui->tabEditor->deleteFile(path)){
                ui->libraryExplorer->reloadLibrary();
            }
        }
    }
}

QString MainMenu::renameDialog(QString what, QString oldName){
    QString newName = QInputDialog::getText(
                this, tr("Rename ") + what + "?",
                tr("Enter new name for the ") + what + ":", QLineEdit::Normal,
                oldName);
    return newName;
}

int MainMenu::deleteQuestion(QString title, QString text, QString path){
    return QMessageBox::question(
                this, title,
                text + "\n\n" + path,
                QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);
}

void MainMenu::updateNoTabLabel(){
    QString text = "<font  size=\"6\">No block is open for editing.</font><br>";
    if (m_lib.exists()){
        text += "<font  size=\"4\">&gt; Open a block from library on the left.</font><br>";;
    } else {
        text += "<font  size=\"4\">&gt; Open a library from top menu bar.</font><br>";
    }
    ui->tabEditor->getNoTabLabel().setText(text);
}
