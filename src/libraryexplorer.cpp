#include "libraryexplorer.h"
#include "ui_libraryexplorer.h"

#include <QTreeWidget>
#include <QDirIterator>
#include <QDebug>

LibraryExplorer::LibraryExplorer(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::LibraryExplorer)
{
    ui->setupUi(this);
    ui->view->setHeaderLabel(tr("Library explorer"));
}

LibraryExplorer::~LibraryExplorer(){
    delete ui;
}

void LibraryExplorer::loadLibrary(QString libraryPath){
    ui->view->clear();

    QFileInfo lib{libraryPath};
    libPath = lib.absoluteFilePath();

    QTreeWidgetItem* dir = new QTreeWidgetItem{ui->view, QStringList() << lib.baseName()};
    addDir(dir, libraryPath);
    ui->view->expandAll();
}

void LibraryExplorer::reloadLibrary(){
    loadLibrary(libPath);
}

void LibraryExplorer::addDir(QTreeWidgetItem* parent, QString path){
    QFileInfoList list = QDir{path}.entryInfoList(
                            {"*.atom", "*.comp"},
                            QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot,
                            QDir::DirsFirst | QDir::Type);
    for (auto& file: list){
        QTreeWidgetItem* dir = new QTreeWidgetItem{QStringList() << file.baseName()};
        parent->addChild(dir);
        if (!file.isDir()){
            //Set icon for file
            dir->setIcon(0, QIcon{file.suffix()});
        } else {
            //Go to subdirectory
            addDir(dir, path + QDir::separator() + file.baseName());
        }
    }
}
