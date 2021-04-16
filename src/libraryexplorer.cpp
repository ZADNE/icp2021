/***
 * \author Tomáš Dubský (xdubsk08)
 * */
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

    //View connections
    connect(ui->view, &QTreeWidget::customContextMenuRequested,
            this, &LibraryExplorer::contextMenu);
    connect(ui->view, &QTreeWidget::itemDoubleClicked,
            this, &LibraryExplorer::viewItemDoubleclicked);
    //Context menu actions connections
    connect(ui->actionAdd_new, &QAction::triggered,
            this, &LibraryExplorer::actionAdd_new);
    connect(ui->actionEdit_file, &QAction::triggered,
            this, &LibraryExplorer::actionEdit_file);
    connect(ui->actionRename, &QAction::triggered,
            this, &LibraryExplorer::actionRename);
    connect(ui->actionDelete, &QAction::triggered,
            this, &LibraryExplorer::actionDelete);
    //Context menus construction
    menuFolder.addAction(ui->actionAdd_new);
    menuFolder.addAction(ui->actionRename);
    menuFolder.addAction(ui->actionDelete);
    menuFile.addAction(ui->actionEdit_file);
    menuFile.addAction(ui->actionRename);
    menuFile.addAction(ui->actionDelete);
}

LibraryExplorer::~LibraryExplorer(){
    delete ui;
}

void LibraryExplorer::loadLibrary(QString libraryPath){
    ui->view->clear();

    QFileInfo lib{libraryPath};
    libPath = lib.absoluteFilePath();

    auto* root = addItem(nullptr, lib);
    ui->view->addTopLevelItem(root);
    addDir(root, libraryPath);

    ui->view->expandAll();
}

void LibraryExplorer::reloadLibrary(){
    loadLibrary(libPath);
}

void LibraryExplorer::contextMenu(const QPoint &point){
    QModelIndex index = ui->view->indexAt(point);
    if (index.isValid()){ //If clicked on an item
        QMenu contextMenu{this};
        QFileInfo fi  = QFileInfo{index.data(Qt::UserRole).toString()};
        if (fi.isDir()){
            menuFolder.popup(mapToGlobal(point));
        } else {
            menuFile.popup(mapToGlobal(point));
        }
    }
}

void LibraryExplorer::actionEdit_file(){
    emit editFile(currentPath());
}

void LibraryExplorer::actionAdd_new(){
    emit addNew(currentPath());
}

void LibraryExplorer::actionRename(){
    emit renameThis(currentPath());
}

void LibraryExplorer::actionDelete(){
    emit deleteThis(currentPath());
}

void LibraryExplorer::addDir(QTreeWidgetItem* parent, QString path){
    QFileInfoList list = QDir{path}.entryInfoList(
                            {"*.atom", "*.comp"},
                            QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot,
                            QDir::DirsFirst | QDir::Type);
    for (auto& file: list){
        auto* newItem = addItem(parent, file);
        if (file.isDir()){
            //Go to subdirectory
            addDir(newItem, file.filePath());
        }
    }
}

QTreeWidgetItem* LibraryExplorer::addItem(QTreeWidgetItem* parent, QFileInfo fi){
    QTreeWidgetItem* newItem;
    if (fi.isFile()){
        newItem = new QTreeWidgetItem{parent, QStringList() << fi.baseName()};
        //Icon based on suffix
        newItem->setIcon(0, QIcon{fi.suffix()});
    } else {
        newItem = new QTreeWidgetItem{parent, QStringList() << fi.fileName()};
        //Default folder icon
        newItem->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
    }
    newItem->setData(0, Qt::UserRole, {fi.canonicalFilePath()});
    return newItem;
}

QString LibraryExplorer::currentPath(){
    return path(ui->view->currentItem(), ui->view->currentColumn());
}

QString LibraryExplorer::path(QTreeWidgetItem *item, int column){
    return item->data(column, Qt::UserRole).toString();
}

void LibraryExplorer::viewItemDoubleclicked(QTreeWidgetItem *item, int column){
    QFileInfo fi = QFileInfo(path(item, column));
    if (fi.isFile()){
        emit editFile(fi.canonicalFilePath());
    }
}
