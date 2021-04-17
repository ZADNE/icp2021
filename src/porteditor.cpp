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
    //Context menus construction
    menuSlot.addAction(ui->actionRemove_port);
    menuSpace.addAction(ui->actionAdd_new_port);
}

PortEditor::~PortEditor(){
    delete ui;
}

void PortEditor::contextMenu(const QPoint &point){
    QModelIndex index = ui->ports->indexAt(point);
    if (index.isValid()){ //If clicked on a port
        menuSlot.exec(mapToGlobal(point));
    } else { //Did not click on a port
        menuSpace.popup(mapToGlobal(point));
    }
}

void PortEditor::addNewPort(){
    auto* port = new QTreeWidgetItem{static_cast<QTreeWidget*>(nullptr), QStringList()
        << "int" << QStringLiteral("port%1").arg(ui->ports->topLevelItemCount())};
    ui->ports->addTopLevelItem(port);
    port->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
}

void PortEditor::removeCurrentPort(){
    delete ui->ports->currentItem();
}
