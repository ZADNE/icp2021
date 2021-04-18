/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#ifndef PORTEDITOR_H
#define PORTEDITOR_H

#include <QWidget>
#include <QMenu>
#include <QTreeWidgetItemIterator>

namespace Ui {
class PortEditor;
}

class PortEditor : public QWidget{
    Q_OBJECT

public:
    explicit PortEditor(QWidget *parent = nullptr);
    ~PortEditor();

    QTreeWidgetItemIterator iterator();

signals:
    void edited();

public slots:
    void contextMenu(const QPoint &point);

    void addPort(QString type, QString name);
    void addNewPort();
    void removeCurrentPort();

private:
    Ui::PortEditor* ui;

    QMenu m_menuSlot;
    QMenu m_menuSpace;
};

#endif // PORTEDITOR_H
