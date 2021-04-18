/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#ifndef TABEDITOR_H
#define TABEDITOR_H

#include <QWidget>
#include <QLabel>
#include <unordered_map>

#include "blockeditor.h"

namespace Ui {
class TabEditor;
}

class TabEditor : public QWidget{
    Q_OBJECT

public:
    explicit TabEditor(QWidget *parent = nullptr);
    ~TabEditor();

    QLabel& getNoTabLabel();

signals:
    void saveWork();

public slots:
    bool createFolder(QString path);
    bool editFile(QString path);//Creates it if it does not exist
    bool renameFile(QString oldFilePath, QString newFilePath);
    bool deleteFile(QString path);
    bool renameFolder(QString path, QString oldName, QString newName);
    bool deleteFolder(QString path);

    void closeAllTabs();
    void checkTabValidity();

protected slots:
    void closeTab(int index);

private:
    Ui::TabEditor *ui;

    void addNoTab();

    std::unordered_map<QString, BlockEditor*> m_tabs;
};

#endif // TABEDITOR_H
