/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#include "porteditor.h"
#include "ui_porteditor.h"

#include <QDebug>

PortEditor::PortEditor(QWidget *parent):
    QWidget(parent),
    ui(new Ui::PortEditor)
{
    ui->setupUi(this);

    connect(ui->ports, &QTreeWidget::customContextMenuRequested,
            this, &PortEditor::contextMenu);
    connect(ui->actionAdd_new_port, &QAction::triggered,
            this, &PortEditor::addNewPort);
    connect(ui->actionRemove_port, &QAction::triggered,
            this, &PortEditor::removeCurrentPort);
    connect(ui->ports, &QTreeWidget::itemChanged,
            this, &PortEditor::edited);
    //Context menus construction
    m_menuSlot.addAction(ui->actionRemove_port);
    m_menuSpace.addAction(ui->actionAdd_new_port);
}

PortEditor::~PortEditor(){
    delete ui;
}

int PortEditor::count(){
    return ui->ports->topLevelItemCount();
}

QTreeWidgetItemIterator PortEditor::iterator(){
    return QTreeWidgetItemIterator{ui->ports};
}

void PortEditor::contextMenu(const QPoint &point){
    QModelIndex index = ui->ports->indexAt(point);
    if (index.isValid()){ //If clicked on a port
        m_menuSlot.exec(mapToGlobal(point));
    } else { //Did not click on a port
        m_menuSpace.popup(mapToGlobal(point));
    }
}

void PortEditor::addPort(QString type, QString name){
    auto* port = new QTreeWidgetItem{static_cast<QTreeWidget*>(nullptr), QStringList()
        << type << name};
    ui->ports->addTopLevelItem(port);
    port->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
}

void PortEditor::addNewPort(){
    addPort("int", QStringLiteral("port%1").arg(ui->ports->topLevelItemCount()));
}

void PortEditor::removeCurrentPort(){
    delete ui->ports->currentItem();
}
