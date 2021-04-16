#include "tabeditor.h"
#include "ui_tabeditor.h"

#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "atomeditor.h"
#include "compeditor.h"

TabEditor::TabEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabEditor)
{
    ui->setupUi(this);

    connect(ui->tabs, &QTabWidget::tabCloseRequested,
            this, qOverload<int>(&TabEditor::closeTab));
}

TabEditor::~TabEditor()
{
    delete ui;
}

void TabEditor::editFile(QString path){
    //Check if it is not open already
    QFileInfo fi{path};
    path = fi.canonicalFilePath();
    for (int i = 0; i < ui->tabs->count(); ++i){
        if (ui->tabs->widget(i)->accessibleDescription() == path){
            //This path is already open, switch to it
            ui->tabs->setCurrentIndex(i);
            return;
        }
    }
    //Path is not open yet, open it
    QWidget* page;
    QIcon icon = QIcon{fi.suffix()};
    QString label= fi.baseName();
    if (fi.suffix() == "atom"){
        auto* ae = new AtomEditor{};
        connect(this, &TabEditor::saveWork,
                ae, &AtomEditor::saveWork);
        page = ae;
    } else if (fi.suffix() == "comp") {
        auto* ce = new CompEditor{};
        connect(this, &TabEditor::saveWork,
                ce, &CompEditor::saveWork);
        page = ce;
    } else {
        return;
    }
    page->setAccessibleDescription(path);
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
    for (int i = 0; i < ui->tabs->count(); ++i){
        auto* widget = ui->tabs->widget(i);
        if (widget->accessibleDescription() == oldFilePath){
            widget->setAccessibleDescription(newFilePath);
            ui->tabs->setTabText(i, QFileInfo{newFilePath}.baseName());
            return true;
        }
    }
    return true;
}

bool TabEditor::deleteFile(QString path){
    //Delete file
    if (!QFile::remove(path)){
        return false;
    }
    //Close tab
    for (int i = 0; i < ui->tabs->count(); ++i){
        auto* widget = ui->tabs->widget(i);
        if (widget->accessibleDescription() == path){
            closeTab(widget);
            break;
        }
    }
    return true;
}

bool TabEditor::renameFolder(QString path, QString oldName, QString newName){
    //Rename folder
    QDir dir{path};
    if (!dir.rename(oldName, newName)){
        return false;
    }
    //Redirect tabs
    for (int i = 0; i < ui->tabs->count(); ++i){
        auto* widget = ui->tabs->widget(i);
        auto tabPath = widget->accessibleDescription();
        int prevIndex = tabPath.lastIndexOf('/', -1);
        int index;
        do {
            index = tabPath.lastIndexOf('/', prevIndex);
            if (tabPath.midRef(index + 1, prevIndex - index) == oldName){
                //Folder with same name (as old name) found
                if (tabPath.leftRef(index) == path){
                    //And it is in correct path - replace name
                    widget->setAccessibleDescription(
                        path + "/" + newName + tabPath.midRef(prevIndex + 1, -1));
                    break;
                }
            }
            prevIndex = index - 1;
        } while (index != -1);
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
    for (int i = 0; i < ui->tabs->count(); ++i){
        auto* widget = ui->tabs->widget(i);
        QFileInfo fi{widget->accessibleDescription()};
        if (!fi.exists()){
            closeTab(widget);
        }
    }
    return true;
}

void TabEditor::checkTabValidity(){

}

void TabEditor::closeTab(QWidget* page){
    if (page && page != ui->no_block){
        delete page;
        if (ui->tabs->count() == 0){
            ui->tabs->addTab(ui->no_block, tr("<no_block>"));
        }
    }
}

void TabEditor::closeTab(int index){
    closeTab(ui->tabs->widget(index));
}
