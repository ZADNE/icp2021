/***
 * \author Tomas Dubsky (xdubsk08)
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

    //Signals from top menu
    connect(ui->actionOpenLib, &QAction::triggered,
            this, &MainMenu::openLibraryDialog);
    connect(ui->actionExit, &QAction::triggered,
            this, &MainMenu::exitProgram);
    //Signals from library explorer
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
    //Signals to tab editor
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
        m_libPath = fi.canonicalFilePath() + "/";
        ui->tabEditor->closeAllTabs();
        updateNoTabLabel();
        ui->libraryExplorer->loadLibrary(m_libPath);
        BlockCompiler::get().openLibrary(m_libPath.toStdString());
        ui->tabEditor->setLibPath(m_libPath);
    }
}

void MainMenu::openLibraryDialog(){
    QString folderName = QFileDialog::getExistingDirectory(this,
        tr("Open a library"), QDir::currentPath());

    if (!folderName.isEmpty()){//If a folder was selected
        openLibrary(folderName);
    }
}

void MainMenu::exitProgram(){
    emit saveWork();
    QApplication::exit(0);
}

void MainMenu::addNew(QString path){
    NewFileDialog f{this, m_libPath, path};
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
    if (path == "."){//Renaming library
        QFileInfo fi{m_libPath.leftRef(m_libPath.size() - 1).toString()};
        QString oldName = fi.fileName();
        QString newName = renameDialog(tr("library"), oldName);
        if (!newName.isEmpty()){
            ui->tabEditor->closeAllTabs();
            QDir dir{fi.path()};
            if (dir.rename(oldName, newName)){
                openLibrary(dir.path() + "/" + newName);
            }
        }
        return;
    }
    //Renaming files or categories
    QFileInfo fi{m_libPath + path};
    if (fi.isDir()){
        //Renaming category
        QString newName = renameDialog(tr("folder"), fi.fileName());
        if (!newName.isEmpty()){
            QFileInfo helper{path};
            if (ui->tabEditor->renameFolder(helper.path(), fi.fileName(), newName)){
                //Successfully renamed
                ui->libraryExplorer->reloadLibrary();
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
    QFileInfo fi{m_libPath + path};
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
    QString text = tr("<font  size=\"6\">No block is open for editing.</font><br>");
    QFileInfo fi{m_libPath};
    if (fi.exists()){
        text += tr("<font  size=\"4\">&gt; Open a block from library on the left.</font><br>");
    } else {
        text += tr("<font  size=\"4\">&gt; Open a library from top menu bar.</font><br>");
    }
    ui->tabEditor->getNoTabLabel().setText(text);
}
