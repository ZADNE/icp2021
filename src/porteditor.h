/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#ifndef PORTEDITOR_H
#define PORTEDITOR_H

#include <QWidget>
#include <QMenu>

namespace Ui {
class PortEditor;
}

class PortEditor : public QWidget{
    Q_OBJECT

public:
    explicit PortEditor(QWidget *parent = nullptr);
    ~PortEditor();

public slots:
    void contextMenu(const QPoint &point);

    void addNewPort();
    void removeCurrentPort();

private:
    Ui::PortEditor* ui;

    QMenu menuSlot;
    QMenu menuSpace;
};

#endif // PORTEDITOR_H
