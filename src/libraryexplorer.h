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

class LibraryExplorer : public QWidget{
    Q_OBJECT

public:
    explicit LibraryExplorer(QWidget* parent = nullptr);
    ~LibraryExplorer();


public slots:
    void loadLibrary(QString libraryPath);
    void reloadLibrary();

signals:
    void addNew(QString path);
    void editFile(QString path);
    void renameThis(QString path);
    void deleteThis(QString path);


private:
    Ui::LibraryExplorer* ui;

    QString m_libPath;

    void addDir(QTreeWidgetItem* parent, QString path);
    QTreeWidgetItem* addItem(QTreeWidgetItem* parent, QFileInfo fi);

    //Item to path
    QString currentPath();
    QString path(QTreeWidgetItem* item, int column);

    QMenu m_menuFolder;
    QMenu m_menuFile;
private slots:
    void viewItemDoubleclicked(QTreeWidgetItem *item, int column);
    void contextMenu(const QPoint &point);

    void actionEdit_file();
    void actionAdd_new();
    void actionRename();
    void actionDelete();
};

#endif // LIBRARYEXPLORER_H
