/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef TABEDITOR_H
#define TABEDITOR_H

#include <QWidget>
#include <QLabel>
#include <unordered_map>

class BlockEditor;

namespace Ui {
class TabEditor;
}

///
/// \brief Manager of block editors inside tabs
///
class TabEditor : public QWidget{
    Q_OBJECT

public:
    explicit TabEditor(QWidget *parent = nullptr);
    ~TabEditor();

    QLabel& getNoTabLabel();

    void setLibPath(QString libPath);

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
protected slots:
    void closeTab(int index);
    void havingUnsavedChanges(BlockEditor* editor);
    void withoutUnsavedChanges(BlockEditor* editor);

private:
    Ui::TabEditor *ui;

    void addNoTab();

    std::unordered_map<QString, BlockEditor*> m_tabs;

    QString m_libPath;
};

#endif // TABEDITOR_H
