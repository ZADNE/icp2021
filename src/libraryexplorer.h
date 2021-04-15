/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#ifndef LIBRARYEXPLORER_H
#define LIBRARYEXPLORER_H

#include <QWidget>
#include <QTreeWidget>
#include <QFileInfo>
#include <QMenu>

namespace Ui {
class LibraryExplorer;
}

class LibraryExplorer : public QWidget
{
    Q_OBJECT

public:
    explicit LibraryExplorer(QWidget* parent = nullptr);
    ~LibraryExplorer();


public slots:
    void loadLibrary(QString libraryPath);
    void reloadLibrary();

signals:
    void addNew(QString folderPath);
    void editFile(QString filePath);
    void deleteFolder(QString folderPath);
    void deleteFile(QString filePath);


private:
    Ui::LibraryExplorer* ui;

    QString libPath;

    void addDir(QTreeWidgetItem* parent, QString path);
    QTreeWidgetItem* addItem(QTreeWidgetItem* parent, QFileInfo fi);

    //Item to path
    QString currentPath();
    QString path(QTreeWidgetItem* item, int column);

    QMenu menuFolder;
    QMenu menuFile;


private slots:
    void itemDoubleClicked(QTreeWidgetItem *item, int column);
    void contextMenu(const QPoint &point);

    void on_actionEdit_file_triggered();
    void on_actionAdd_new_triggered();
    void on_actionDelete_folder_triggered();
    void on_actionDelete_file_triggered();
};

#endif // LIBRARYEXPLORER_H
