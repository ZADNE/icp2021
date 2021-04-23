/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef VARIABLEEDITOR_H
#define VARIABLEEDITOR_H

#include <QWidget>
#include <QMenu>
#include <QTreeWidgetItemIterator>

namespace Ui {
class PortEditor;
}

class VariableEditor : public QWidget{
    Q_OBJECT

public:
    explicit VariableEditor(QWidget *parent = nullptr);
    ~VariableEditor();

    int count();

    QTreeWidgetItemIterator iterator();

signals:
    void edited();

public slots:
    void contextMenu(const QPoint &point);

    void addVariable(QString type, QString name);
    void addNewVariable();
    void removeCurrentVariable();

private:
    Ui::PortEditor* ui;

    QMenu m_menuSlot;
    QMenu m_menuSpace;
};

#endif // VARIABLEEDITOR_H
