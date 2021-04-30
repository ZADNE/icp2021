/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "compeditor.h"
#include "ui_compeditor.h"

#include <QDebug>

CompEditor::CompEditor(QWidget *parent):
    BlockEditor(parent),
    ui(new Ui::CompEditor)
{
    ui->setupUi(this);

    connect(ui->inputEditor, &VariableEditor::edited,
            this, &CompEditor::editedWork);
    connect(ui->outputEditor, &VariableEditor::edited,
            this, &CompEditor::editedWork);
    connect(ui->designer, &ConnectionDesigner::changed,
            this, &CompEditor::editedWork);
    connect(ui->nameEditor, &QLineEdit::textChanged,
            this, &CompEditor::editedWork);
}

CompEditor::~CompEditor(){
    delete ui;
}

void CompEditor::load(){
    qDebug() << "load " << filePath();
    //ui->designer->setTarget(filePath(), m_spec);
}

void CompEditor::save(){
    //Compose specs
    auto spec = CompSpec{};
    spec.name = "Com";
    spec.inputs.emplace_back(false, "int", "in0");
    spec.outputs.emplace_back(false, "int", "out0");
    spec.instances.emplace_back("acc", "acum.atom", 0, 0);
    spec.connections.emplace_back("", "in0", "acc", "in");
    spec.connections.emplace_back("acc", "out", "", "out0");
    //Write specs
    BlockCompiler::get().writeComp(filePath().toStdString(), spec);
}

void CompEditor::build(){
    BlockCompiler::get().buildComp(filePath().toStdString());
}
