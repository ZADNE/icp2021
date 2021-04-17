/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#include "tabeditor.h"
#include "ui_tabeditor.h"

#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "atomeditor.h"
#include "compeditor.h"

TabEditor::TabEditor(QWidget *parent):
    QWidget(parent),
    ui(new Ui::TabEditor)
{
    ui->setupUi(this);

    connect(ui->tabs, &QTabWidget::tabCloseRequested,
            this, &TabEditor::closeTab);
}

TabEditor::~TabEditor(){
    delete ui;
}

QLabel& TabEditor::getNoTabLabel(){
    return *ui->no_block_label;
}

void TabEditor::editFile(QString path){
    //Check if it is not open already
    QFileInfo fi{path};
    path = fi.canonicalFilePath();
    auto it = tabs.find(path);
    if (it != tabs.end()){
        //This path is already open, switch to it
        ui->tabs->setCurrentWidget(it->second);
        return;
    }
    //Path is not open yet, open it
    BlockEditor* page;
    QIcon icon = QIcon{fi.suffix()};
    QString label = fi.completeBaseName();
    if (fi.suffix() == "atom"){
        page = new AtomEditor{};
    } else if (fi.suffix() == "comp") {
        page = new CompEditor{};
    } else {
        return;
    }
    page->setFilePath(path);
    tabs.insert({path, page});
    //Remove no-block tab (if it is there)
    int index = ui->tabs->indexOf(ui->no_block);
    if (index != -1){
        ui->tabs->removeTab(index);
    }
    //Add new tab and set it current
    ui->tabs->addTab(page, icon, label);
    ui->tabs->setCurrentWidget(page);
}

bool TabEditor::renameFile(QString oldFilePath, QString newFilePath){
    //Rename file
    QFile file{oldFilePath};
    if (!file.rename(newFilePath)){
        return false;
    }
    //Rename tab
    auto it = tabs.find(oldFilePath);
    if (it != tabs.end()){
        //Reinsert page with new path
        auto* page = it->second;
        tabs.erase(it);
        tabs.insert({newFilePath, page});
        //Change tab text
        ui->tabs->setTabText(ui->tabs->indexOf(page),
                QFileInfo{newFilePath}.completeBaseName());
        //And inform editor of new path
        page->setFilePath(newFilePath);
    }
    return true;
}

bool TabEditor::deleteFile(QString path){
    //Delete file
    if (!QFile::remove(path)){
        return false;
    }
    //Close tab
    auto it = tabs.find(path);
    if (it != tabs.end()){
        ui->tabs->removeTab(ui->tabs->indexOf(it->second));
        delete it->second;
        tabs.erase(it);
    }
    addNoTab();
    return true;
}

bool TabEditor::renameFolder(QString path, QString oldName, QString newName){
    //Rename folder
    QDir dir{path};
    if (!dir.rename(oldName, newName)){
        return false;
    }
    //Redirect tabs
    for (auto it = tabs.begin(); it != tabs.end(); ){
        auto tabPath = it->first;
        int prevIndex = tabPath.lastIndexOf('/', -1);
        int index;
        bool matchFound = false;
        do {
            index = tabPath.lastIndexOf('/', prevIndex);
            if (tabPath.midRef(index + 1, prevIndex - index) == oldName //Folder with same old name found
                && tabPath.leftRef(index) == path){//And it is in correct path - replace name
                //Reinsert page
                auto* page = it->second;
                it = tabs.erase(it);
                QString newPath = path + "/" + newName + tabPath.midRef(prevIndex + 1, -1);
                tabs.insert({newPath, page});
                //Inform page of new path
                page->setFilePath(newPath);
                matchFound = true;
                break;
            }
            prevIndex = index - 1;
        } while (index != -1);
        if (!matchFound){
            //Match not found, check next
            ++it;
        }
    }
    return true;
}

bool TabEditor::deleteFolder(QString path){
    //Delete folder
    QDir dir{path};
    if (!dir.removeRecursively()){
        return false;
    }
    //Close tabs
    for (auto it = tabs.begin(); it != tabs.end(); ){
        QFileInfo fi{it->first};
        if (!fi.exists()){
            ui->tabs->removeTab(ui->tabs->indexOf(it->second));
            delete it->second;
            it = tabs.erase(it);
        } else {
            ++it;
        }
    }
    addNoTab();
    return true;
}

void TabEditor::closeAllTabs(){
    //Check if only no-tab is open
    if (ui->tabs->indexOf(ui->no_block) != -1){
        return;
    }
    //Close all tabs
    ui->tabs->clear();
    auto it = tabs.begin();
    while (it != tabs.end()){
        delete it->second;
        it = tabs.erase(it);
    }
    //Add no-tab
    addNoTab();
}

void TabEditor::checkTabValidity(){

}

void TabEditor::addNoTab(){
    if (ui->tabs->count() == 0){
        ui->tabs->addTab(ui->no_block, tr("<no_block>"));
    }
}

void TabEditor::closeTab(int index){
    auto* page = ui->tabs->widget(index);
    if (page == ui->no_block){
        return;//Do not delete no-tab
    }
    //Remove it from tabs table
    auto it = tabs.begin();
    while (it != tabs.end()){
        if (it->second == page){
            tabs.erase(it);
            break;
        }
        ++it;
    }
    //Delete the page
    delete page;
    addNoTab();
}
