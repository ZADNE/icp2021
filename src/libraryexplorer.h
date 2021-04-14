#ifndef LIBRARYEXPLORER_H
#define LIBRARYEXPLORER_H

#include <QWidget>
#include <QTreeWidget>

namespace Ui {
class LibraryExplorer;
}

class LibraryExplorer : public QWidget
{
    Q_OBJECT

public:
    explicit LibraryExplorer(QWidget *parent = nullptr);
    ~LibraryExplorer();


public slots:
    void loadLibrary(QString libraryPath);
    void reloadLibrary();

private:
    Ui::LibraryExplorer *ui;

    QString libPath;
    void addDir(QTreeWidgetItem* parent, QString path);
};

#endif // LIBRARYEXPLORER_H
