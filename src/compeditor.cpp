/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "compeditor.h"
#include "ui_compeditor.h"

#include <QDebug>

#include "blockcompiler.h"
#include "speccache.h"

CompEditor::CompEditor(QWidget *parent):
    BlockEditor(parent),
    ui(new Ui::CompEditor)
{
    ui->setupUi(this);

    connect(ui->inputEditor, &PortEditor::edited,
            this, &CompEditor::editedWork);
    connect(ui->outputEditor, &PortEditor::edited,
            this, &CompEditor::editedWork);
    connect(ui->designer, &ConnectionDesigner::changed,
            this, &CompEditor::editedWork);
    connect(ui->nameEditor, &QLineEdit::textChanged,
            this, &CompEditor::editedWork);

    connect(ui->designer, &ConnectionDesigner::editBlock,
            this, &BlockEditor::editBlock);
    connect(ui->designer, &ConnectionDesigner::changed,
            this, &BlockEditor::editedWork);
}

CompEditor::~CompEditor(){
    delete ui;
}

void CompEditor::load(){
    ui->designer->setMyPath(filePath());
    //Read specs
    auto spec = CompSpec{};
    SpecCache::fetch(filePath().toStdString(), spec);
    //Insert specs to widgets
    ui->nameEditor->setText(spec.name.c_str());
    ui->inputEditor->setPorts(spec.inputs);
    ui->outputEditor->setPorts(spec.outputs);
    ui->designer->insertSpecs(spec.instances, spec.connections, spec.constants);
}

void CompEditor::save(){
    //Compose specs
    auto spec = CompSpec{};
    spec.name = ui->nameEditor->text().toStdString();
    ui->inputEditor->collectPorts(spec.inputs);
    ui->outputEditor->collectPorts(spec.outputs);
    ui->designer->collectSpecs(spec.instances, spec.connections, spec.constants);
    //Write specs
    SpecCache::save(filePath().toStdString(), spec);
}

void CompEditor::build(){
    BlockCompiler::get().buildComp(filePath().toStdString());
}
