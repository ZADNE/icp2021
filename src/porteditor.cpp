/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "porteditor.h"
#include "ui_variableeditor.h"

#include <QDebug>

PortEditor::PortEditor(QWidget *parent):
    QWidget(parent),
    ui(new Ui::PortEditor)
{
    ui->setupUi(this);

    ui->vars->setColumnWidth(0, 40);

    connect(ui->vars, &QTreeWidget::customContextMenuRequested,
            this, &PortEditor::contextMenu);
    connect(ui->actionAdd_new_port, &QAction::triggered,
            this, &PortEditor::addNewPort);
    connect(ui->actionRemove_port, &QAction::triggered,
            this, &PortEditor::removeCurrentPort);
    connect(ui->vars, &QTreeWidget::itemChanged,
            this, &PortEditor::edited);
    //Context menus construction
    m_menuSlot.addAction(ui->actionRemove_port);
    m_menuSpace.addAction(ui->actionAdd_new_port);
}

PortEditor::~PortEditor(){
    delete ui;
}

int PortEditor::count(){
    return ui->vars->topLevelItemCount();
}

void PortEditor::contextMenu(const QPoint &point){
    QModelIndex index = ui->vars->indexAt(point);
    if (index.isValid()){ //If clicked on a port
        m_menuSlot.exec(mapToGlobal(point));
    } else { //Did not click on a port
        m_menuSpace.popup(mapToGlobal(point));
    }
}

void PortEditor::addPort(bool templ, QString type, QString name){
    auto* port = new QTreeWidgetItem{static_cast<QTreeWidget*>(nullptr), QStringList()
        << "" << type << name};
    ui->vars->addTopLevelItem(port);
    port->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    port->setCheckState(0,  templ ? Qt::Checked : Qt::Unchecked);
}

void PortEditor::addNewPort(){
    addPort(false, "int", QStringLiteral("port%1").arg(ui->vars->topLevelItemCount()));
}

void PortEditor::removeCurrentPort(){
    auto* curr = ui->vars->currentItem();
    if (curr){
        delete curr;
        emit edited();
    }
}

void PortEditor::collectPorts(PortList& portl){
    auto it = QTreeWidgetItemIterator{ui->vars};
    while (*it){
        portl.emplace_back(
            (*it)->checkState(0) == Qt::Checked ? true : false,
            (*it)->text(1).toStdString(),
            (*it)->text(2).toStdString());
        ++it;
    }
}

void PortEditor::setPorts(const PortList& portl){
    for(auto& port: portl){
        addPort(port.templ, port.type.c_str(), port.name.c_str());
    }
}
