/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef PORTEDITOR_H
#define PORTEDITOR_H

#include <QWidget>
#include <QMenu>
#include <QTreeWidgetItemIterator>

#include "blockspec.h"

namespace Ui {
class PortEditor;
}

///
/// \brief Editor of input/output ports
///
class PortEditor: public QWidget{
    Q_OBJECT

public:
    explicit PortEditor(QWidget *parent = nullptr);
    ~PortEditor();

    int count();

    void setName(QString name);

signals:
    void edited();

public slots:
    void contextMenu(const QPoint& point);

    void addPort(/*bool templ, */QString type, QString name);
    void addNewPort();
    void removeCurrentPort();

    void collectPorts(PortList& portl);
    void setPorts(const PortList& portl);

private:
    Ui::PortEditor* ui;

    QMenu m_menuSlot;
    QMenu m_menuSpace;
    QString m_name;
};

#endif // PORTEDITOR_H
