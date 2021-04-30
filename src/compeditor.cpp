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
    connect(ui->includesEditor, &QTextEdit::textChanged,
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
    qDebug() << "save " << filePath();
}

void CompEditor::build(){
    qDebug() << "build " << filePath();
}
