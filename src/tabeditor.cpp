/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "tabeditor.h"
#include "ui_tabeditor.h"

#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QShortcut>

#include "blockeditor.h"
#include "atomeditor.h"
#include "compeditor.h"
#include "appleditor.h"

TabEditor::TabEditor(QWidget *parent):
    QWidget(parent),
    ui(new Ui::TabEditor)
{
    ui->setupUi(this);

    connect(ui->tabs, &QTabWidget::tabCloseRequested,
            this, &TabEditor::closeTab);


    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SIGNAL(saveWork()));
}

TabEditor::~TabEditor(){
    emit saveWork();
    delete ui;
}

QLabel& TabEditor::getNoTabLabel(){
    return *ui->no_block_label;
}

void TabEditor::setLibPath(QString libPath){
    m_libPath = libPath;
    BlockEditor::setLibPath(libPath);
}

bool TabEditor::createFolder(QString path){
    QFileInfo fi{m_libPath + path};
    return QDir{fi.path()}.mkdir(fi.fileName());
}

bool TabEditor::editFile(QString path){
    QFileInfo fi{path};
    //Check if it is not open already
    auto it = m_tabs.find(path);
    if (it != m_tabs.end()){
        //This path is already open, switch to it
        ui->tabs->setCurrentWidget(it->second);
        return true;
    }
    //Path is not open yet, open it
    BlockEditor* page;
    QIcon icon = QIcon{fi.suffix()};
    QString label = fi.completeBaseName();
    if (fi.suffix() == "atom"){
        page = new AtomEditor{};
    } else if (fi.suffix() == "comp") {
        page = new CompEditor{};
    } else if (fi.suffix() == "appl") {
        page = new ApplEditor{};
    } else {
        return false;
    }
    connect(this, &TabEditor::saveWork,
            page, &BlockEditor::saveWork);
    connect(page, &BlockEditor::havingUnsavedChanges,
            this, &TabEditor::havingUnsavedChanges);
    connect(page, &BlockEditor::withoutUnsavedChanges,
            this, &TabEditor::withoutUnsavedChanges);
    connect(page, &BlockEditor::editBlock,
            this, &TabEditor::editFile);
    page->setFilePath(path, true);
    m_tabs.insert({path, page});
    //Remove no-block tab (if it is there)
    int index = ui->tabs->indexOf(ui->no_block);
    if (index != -1){
        ui->tabs->removeTab(index);
    }
    //Add new tab and set it current
    ui->tabs->addTab(page, icon, label);
    ui->tabs->setCurrentWidget(page);
    return true;
}

bool TabEditor::renameFile(QString oldFilePath, QString newFilePath){
    //Rename file
    QFile file{m_libPath + oldFilePath};
    if (!file.rename(m_libPath + newFilePath)){
        return false;
    }
    emit BlockEditor::sig.blockRenamed(oldFilePath, newFilePath);
    //Rename tab
    auto it = m_tabs.find(oldFilePath);
    if (it != m_tabs.end()){
        //Reinsert page with new path
        auto* page = it->second;
        m_tabs.erase(it);
        m_tabs.insert({newFilePath, page});
        //Change tab text
        ui->tabs->setTabText(ui->tabs->indexOf(page),
                QFileInfo{newFilePath}.completeBaseName());
        //And inform editor of new path
        page->setFilePath(newFilePath, false);
    }
    return true;
}

bool TabEditor::deleteFile(QString path){
    //Delete file
    if (!QFile::remove(m_libPath + path)){
        return false;
    }
    emit BlockEditor::sig.blockEdited(path);
    //Close tab
    auto it = m_tabs.find(path);
    if (it != m_tabs.end()){
        ui->tabs->removeTab(ui->tabs->indexOf(it->second));
        delete it->second;
        m_tabs.erase(it);
    }
    addNoTab();
    return true;
}

bool TabEditor::renameFolder(QString path, QString oldName, QString newName){
    //Rename folder
    QDir dir{m_libPath + path};
    if (!dir.rename(oldName, newName)){
        return false;
    }
    //Redirect tabs
    for (auto it = m_tabs.begin(); it != m_tabs.end(); ){
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
                it = m_tabs.erase(it);
                QString newPath = path + "/" + newName + tabPath.midRef(prevIndex + 1, -1);
                m_tabs.insert({newPath, page});
                //Inform page of new path
                page->setFilePath(newPath, false);
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
    emit BlockEditor::sig.blockEdited("");
    return true;
}

bool TabEditor::deleteFolder(QString path){
    //Delete folder
    QDir dir{m_libPath + path};
    if (!dir.removeRecursively()){
        return false;
    }
    emit BlockEditor::sig.blockEdited("");
    //Close tabs
    for (auto it = m_tabs.begin(); it != m_tabs.end(); ){
        QFileInfo fi{it->first};
        if (!fi.exists()){
            ui->tabs->removeTab(ui->tabs->indexOf(it->second));
            delete it->second;
            it = m_tabs.erase(it);
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
    auto it = m_tabs.begin();
    while (it != m_tabs.end()){
        it->second->saveWork();
        delete it->second;
        it = m_tabs.erase(it);
    }
    //Add no-tab
    addNoTab();
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
    auto it = m_tabs.begin();
    while (it != m_tabs.end()){
        if (it->second == page){
            it->second->saveWork();
            m_tabs.erase(it);
            break;
        }
        ++it;
    }
    //Delete the page
    delete page;
    addNoTab();
}

void TabEditor::havingUnsavedChanges(BlockEditor* editor){
    int i = ui->tabs->indexOf(editor);
    if (i == -1) return;
    auto tabText = ui->tabs->tabText(i);
    if (!tabText.endsWith("*")){
        ui->tabs->setTabText(i, tabText + "*");
    }
}

void TabEditor::withoutUnsavedChanges(BlockEditor* editor){
    int i = ui->tabs->indexOf(editor);
    if (i == -1) return;
    auto tabText = ui->tabs->tabText(i);
    if (tabText.endsWith("*")){
        ui->tabs->setTabText(i, tabText.left(tabText.size() - 1));
    }
}
