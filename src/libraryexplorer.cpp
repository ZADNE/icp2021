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


    connect(ui->view, &QTreeWidget::itemDoubleClicked,
            this, &LibraryExplorer::itemDoubleClicked);
    connect(ui->view, &QTreeWidget::customContextMenuRequested,
            this, &LibraryExplorer::contextMenu);
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
            //Context menu for folder
            contextMenu.addAction(ui->actionAdd_new);
            contextMenu.addAction(ui->actionDelete_folder);
        } else {
            //Context menu for file
            contextMenu.addAction(ui->actionEdit_file);
            contextMenu.addAction(ui->actionDelete_file);
        }
        contextMenu.exec(mapToGlobal(point));
    }
}

void LibraryExplorer::on_actionEdit_file_triggered(){
    emit editFile(currentPath());
}

void LibraryExplorer::on_actionAdd_new_triggered(){
    emit addNew(currentPath());
}

void LibraryExplorer::on_actionDelete_folder_triggered(){
    emit deleteFolder(currentPath());
}

void LibraryExplorer::on_actionDelete_file_triggered(){
    emit deleteFile(currentPath());
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
    QTreeWidgetItem* newItem = new QTreeWidgetItem{parent, QStringList() << fi.fileName()};
    newItem->setData(0, Qt::UserRole, {fi.canonicalFilePath()});
    if (fi.isFile()){
        //Icon based on suffix
        newItem->setIcon(0, QIcon{fi.suffix()});
    } else {
        //Default folder icon
        newItem->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
    }
    return newItem;
}

QString LibraryExplorer::currentPath(){
    return path(ui->view->currentItem(), ui->view->currentColumn());
}

QString LibraryExplorer::path(QTreeWidgetItem *item, int column){
    return item->data(column, Qt::UserRole).toString();
}

void LibraryExplorer::itemDoubleClicked(QTreeWidgetItem *item, int column){
    QFileInfo fi = QFileInfo(path(item, column));
    if (fi.isFile()){
        emit editFile(fi.canonicalFilePath());
    }
}
