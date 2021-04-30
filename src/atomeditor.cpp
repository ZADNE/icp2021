/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "atomeditor.h"
#include "ui_atomeditor.h"

#include <QDebug>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>

#include "blockcompiler.h"

//Helper functions
void loadPorts(VariableEditor* ve, const SlotList& sl);
void savePorts(const VariableEditor* ve, SlotList& sl);

AtomEditor::AtomEditor(QWidget *parent):
    BlockEditor(parent),
    ui(new Ui::AtomEditor)
{
    ui->setupUi(this);

    connect(ui->inputEditor, &VariableEditor::edited,
            this, &AtomEditor::editedWork);
    connect(ui->outputEditor, &VariableEditor::edited,
            this, &AtomEditor::editedWork);
    connect(ui->codeEditor, &QTextEdit::textChanged,
            this, &AtomEditor::editedWork);
    connect(ui->stateVarEditor, &QTextEdit::textChanged,
            this, &AtomEditor::editedWork);
    connect(ui->nameEditor, &QLineEdit::textChanged,
            this, &AtomEditor::editedWork);
}

AtomEditor::~AtomEditor(){
    delete ui;
}

void AtomEditor::load(){
    //Read specs
    auto spec = AtomSpec{};
    BlockCompiler::get().readAtom(filePath().toStdString(), spec);
    //Insert specs to widgets
    ui->nameEditor->setText(spec.name.c_str());
    loadPorts(ui->inputEditor, spec.inputs);
    loadPorts(ui->outputEditor, spec.outputs);
    ui->codeEditor->setPlainText(spec.body.c_str());
    ui->stateVarEditor->setPlainText(spec.stateVars.c_str());
}

void AtomEditor::save(){
    //Compose specs
    auto spec = AtomSpec{};
    spec.name = ui->nameEditor->text().toStdString();
    savePorts(ui->inputEditor, spec.inputs);
    savePorts(ui->outputEditor, spec.outputs);
    spec.body = ui->codeEditor->toPlainText().toStdString();
    spec.stateVars = ui->stateVarEditor->toPlainText().toStdString();
    //Write specs
    BlockCompiler::get().writeAtom(filePath().toStdString(), spec);
}

void AtomEditor::build(){
    BlockCompiler::get().buildAtom(filePath().toStdString());
}


void loadPorts(VariableEditor* ve, const SlotList& sl){
    for(auto& slot: sl){
        ve->addVariable(slot.templ, slot.type.c_str(), slot.name.c_str());
    }
}

void savePorts(const VariableEditor* ve, SlotList& sl){
    auto it = ve->iterator();
    while (*it){
        sl.emplace_back(
            (*it)->checkState(0) == Qt::Checked ? true : false,
            (*it)->text(1).toStdString(),
            (*it)->text(2).toStdString());
        ++it;
    }
}
