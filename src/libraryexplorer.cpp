/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "libraryexplorer.h"
#include "ui_libraryexplorer.h"

#include <QTreeWidget>
#include <QDirIterator>
#include <QDebug>

LibraryExplorer::LibraryExplorer(QWidget* parent):
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
    m_menuFolder.addAction(ui->actionAdd_new);
    m_menuFolder.addAction(ui->actionRename);
    m_menuFolder.addAction(ui->actionDelete);
    m_menuFile.addAction(ui->actionEdit_file);
    m_menuFile.addAction(ui->actionRename);
    m_menuFile.addAction(ui->actionDelete);
}

LibraryExplorer::~LibraryExplorer(){
    delete ui;
}

void LibraryExplorer::loadLibrary(QString libraryPath){
    ui->view->clear();

    QFileInfo lib{libraryPath};
    m_libPath = lib.canonicalFilePath();

    auto* root = addItem(nullptr, lib.canonicalFilePath(), ".");
    ui->view->addTopLevelItem(root);
    addDir(root, m_libPath, ".");

    ui->view->expandAll();
}

void LibraryExplorer::reloadLibrary(){
    loadLibrary(m_libPath);
}

void LibraryExplorer::contextMenu(const QPoint &point){
    QModelIndex index = ui->view->indexAt(point);
    if (index.isValid()){ //If clicked on an item
        QString filePath =
                m_libPath + index.data(REL_PATH_ROLE).toString();
        QFileInfo fi  = QFileInfo{filePath};
        if (fi.isDir()){
            m_menuFolder.popup(mapToGlobal(point));
        } else if (fi.isFile()) {
            m_menuFile.popup(mapToGlobal(point));
        }
    }
}

void LibraryExplorer::actionEdit_file(){
    emit editFile(currentRelPath());
}

void LibraryExplorer::actionAdd_new(){
    emit addNew(currentRelPath());
}

void LibraryExplorer::actionRename(){
    emit renameThis(currentRelPath());
}

void LibraryExplorer::actionDelete(){
    emit deleteThis(currentRelPath());
}

void LibraryExplorer::addDir(QTreeWidgetItem* parent, QString path, QString relPath){
    QFileInfoList list = QDir{path}.entryInfoList(
                            {"*.atom", "*.comp", "*.appl"},
                            QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot,
                            QDir::DirsFirst | QDir::Type);
    for (auto& file: list){
        QString nextRelPath = relPath + "/" + file.fileName();
        auto* newItem = addItem(parent, file, nextRelPath);
        if (file.isDir()){
            //Go to subdirectory
            addDir(newItem, path + "/" + file.fileName(), nextRelPath);
        }
    }
}

QTreeWidgetItem* LibraryExplorer::addItem(QTreeWidgetItem* parent, QFileInfo fi, QString relPath){
    QTreeWidgetItem* newItem;
    if (fi.isFile()){
        newItem = new QTreeWidgetItem{parent, QStringList() << fi.completeBaseName()};
        //Icon based on suffix
        newItem->setIcon(0, QIcon{fi.suffix()});
    } else {
        newItem = new QTreeWidgetItem{parent, QStringList() << fi.fileName()};
        //Default folder icon
        newItem->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
    }
    newItem->setData(0, REL_PATH_ROLE, {relPath});
    return newItem;
}

QString LibraryExplorer::currentRelPath(){
    return relPath(ui->view->currentItem(), ui->view->currentColumn());
}

QString LibraryExplorer::relPath(QTreeWidgetItem* item, int column){
    return item->data(column, REL_PATH_ROLE).toString();
}

void LibraryExplorer::viewItemDoubleclicked(QTreeWidgetItem* item, int column){
    auto rel = relPath(item, column);
    QFileInfo fi = QFileInfo(m_libPath + rel);
    if (fi.isFile()){
        emit editFile(rel);
    }
}
