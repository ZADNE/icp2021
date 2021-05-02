/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "appleditor.h"
#include "ui_appleditor.h"

#include <QDebug>

#include "blockbuilder.h"
#include "speccache.h"

ApplEditor::ApplEditor(QWidget *parent):
    BlockEditor(parent),
    ui(new Ui::ApplEditor)
{
    ui->setupUi(this);

    connect(ui->designer, &ConnectionDesigner::changed,
            this, &ApplEditor::editedWork);
    connect(ui->nameEditor, &QLineEdit::textChanged,
            this, &ApplEditor::editedWork);

    connect(ui->designer, &ConnectionDesigner::editBlock,
            this, &BlockEditor::editBlock);
}

ApplEditor::~ApplEditor(){
    delete ui;
}

void ApplEditor::load(){
    ui->designer->setMyPath(filePath());
    //Read specs
    auto spec = ApplSpec{};
    SpecCache::fetch(filePath().toStdString(), spec);
    //Insert specs to widgets
    ui->nameEditor->setText(spec.name.c_str());
    ui->designer->insertSpecs(spec.instances, spec.connections, spec.constants);
}

void ApplEditor::save(){
    //Compose specs
    auto spec = ApplSpec{};
    spec.name = ui->nameEditor->text().toStdString();
    ui->designer->collectSpecs(spec.instances, spec.connections, spec.constants);
    //Write specs
    SpecCache::save(filePath().toStdString(), spec);
}

void ApplEditor::build(){
    BlockBuilder::get().buildAppl(filePath().toStdString());
}
