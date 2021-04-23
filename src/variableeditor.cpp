/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "variableeditor.h"
#include "ui_variableeditor.h"

#include <QDebug>

VariableEditor::VariableEditor(QWidget *parent):
    QWidget(parent),
    ui(new Ui::PortEditor)
{
    ui->setupUi(this);

    ui->vars->setColumnWidth(0, 40);

    connect(ui->vars, &QTreeWidget::customContextMenuRequested,
            this, &VariableEditor::contextMenu);
    connect(ui->actionAdd_new_port, &QAction::triggered,
            this, &VariableEditor::addNewVariable);
    connect(ui->actionRemove_port, &QAction::triggered,
            this, &VariableEditor::removeCurrentVariable);
    connect(ui->vars, &QTreeWidget::itemChanged,
            this, &VariableEditor::edited);
    //Context menus construction
    m_menuSlot.addAction(ui->actionRemove_port);
    m_menuSpace.addAction(ui->actionAdd_new_port);
}

VariableEditor::~VariableEditor(){
    delete ui;
}

int VariableEditor::count(){
    return ui->vars->topLevelItemCount();
}

QTreeWidgetItemIterator VariableEditor::iterator(){
    return QTreeWidgetItemIterator{ui->vars};
}

void VariableEditor::contextMenu(const QPoint &point){
    QModelIndex index = ui->vars->indexAt(point);
    if (index.isValid()){ //If clicked on a port
        m_menuSlot.exec(mapToGlobal(point));
    } else { //Did not click on a port
        m_menuSpace.popup(mapToGlobal(point));
    }
}

void VariableEditor::addVariable(bool templ, QString type, QString name){
    auto* port = new QTreeWidgetItem{static_cast<QTreeWidget*>(nullptr), QStringList()
        << "" << type << name};
    ui->vars->addTopLevelItem(port);
    port->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    port->setCheckState(0,  templ ? Qt::Checked : Qt::Unchecked);
}

void VariableEditor::addNewVariable(){
    addVariable(false, "int", QStringLiteral("port%1").arg(ui->vars->topLevelItemCount()));
}

void VariableEditor::removeCurrentVariable(){
    delete ui->vars->currentItem();
}
