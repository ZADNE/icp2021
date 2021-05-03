/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "atomeditor.h"
#include "ui_atomeditor.h"

#include <QDebug>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>

#include "blockbuilder.h"
#include "speccache.h"


AtomEditor::AtomEditor(QWidget *parent):
    BlockEditor(parent),
    ui(new Ui::AtomEditor)
{
    ui->setupUi(this);

    connect(ui->inputEditor, &PortEditor::edited,
            this, &AtomEditor::editedWork);
    connect(ui->outputEditor, &PortEditor::edited,
            this, &AtomEditor::editedWork);
    connect(ui->codeEditor, &QTextEdit::textChanged,
            this, &AtomEditor::editedWork);
    connect(ui->stateVarEditor, &QTextEdit::textChanged,
            this, &AtomEditor::editedWork);
    connect(ui->nameEditor, &QLineEdit::textChanged,
            this, &AtomEditor::editedWork);

    ui->inputEditor->setName("in");
    ui->outputEditor->setName("out");
}

AtomEditor::~AtomEditor(){
    delete ui;
}

void AtomEditor::load(){
    //Read specs
    auto spec = AtomSpec{};
    SpecCache::fetch(filePath().toStdString(), spec);
    //Insert specs to widgets
    ui->nameEditor->setText(spec.name.c_str());
    ui->inputEditor->setPorts(spec.inputs);
    ui->outputEditor->setPorts(spec.outputs);
    ui->codeEditor->setPlainText(spec.body.c_str());
    ui->stateVarEditor->setPlainText(spec.stateVars.c_str());
}

void AtomEditor::save(){
    //Compose specs
    auto spec = AtomSpec{};
    spec.name = ui->nameEditor->text().toStdString();
    ui->inputEditor->collectPorts(spec.inputs);
    ui->outputEditor->collectPorts(spec.outputs);
    spec.body = ui->codeEditor->toPlainText().toStdString();
    spec.stateVars = ui->stateVarEditor->toPlainText().toStdString();
    //Write specs
    SpecCache::save(filePath().toStdString(), spec);
}

void AtomEditor::build(){
    BlockBuilder::get().buildAtom(filePath().toStdString());
}
