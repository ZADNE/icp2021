/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#ifndef TABEDITOR_H
#define TABEDITOR_H

#include <QWidget>

namespace Ui {
class TabEditor;
}

class TabEditor : public QWidget{
    Q_OBJECT

public:
    explicit TabEditor(QWidget *parent = nullptr);
    ~TabEditor();




public slots:
    void editFile(QString path);
    bool renameFile(QString oldFilePath, QString newFilePath);
    bool deleteFile(QString path);
    bool renameFolder(QString path, QString oldName, QString newName);
    bool deleteFolder(QString path);
    void checkTabValidity();

signals:
    void saveWork();

private:
    Ui::TabEditor *ui;

protected slots:
    void closeTab(QWidget* page);
    void closeTab(int index);
};

#endif // TABEDITOR_H
