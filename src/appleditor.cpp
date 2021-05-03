/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "appleditor.h"
#include "ui_appleditor.h"

#include <QDebug>
#include <QMessageBox>

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
    connect(ui->stepsEditor, &QLineEdit::textChanged,
            this, &ApplEditor::editedWork);

    connect(ui->designer, &ConnectionDesigner::editBlock,
            this, &BlockEditor::editBlock);

    connect(this, &ApplEditor::blockEdited,
            ui->designer, &ConnectionDesigner::blockEdited);
    connect(this, &ApplEditor::blockRenamed,
            ui->designer, &ConnectionDesigner::blockRenamed);
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
    ui->stepsEditor->setText(QString::number(spec.maxSteps));
    ui->designer->insertSpecs(spec.instances, spec.connections, spec.constants);
}

void ApplEditor::save(){
    //Compose specs
    auto spec = ApplSpec{};
    spec.name = ui->nameEditor->text().toStdString();
    try {
        spec.maxSteps = std::stoull(ui->stepsEditor->text().toStdString());
    }  catch (...) {
        spec.maxSteps = 0;
    }
    ui->designer->collectSpecs(spec.instances, spec.connections, spec.constants);
    //Write specs
    SpecCache::save(filePath().toStdString(), spec);
}

void ApplEditor::build(){
    if (!BlockBuilder::get().buildAppl(filePath().toStdString())){
        //Build or compilation failed
        QMessageBox::warning(this, tr("Compilation failed!"),
            tr("Compilation failed.\n"
                "The reason for this can be either wrong code in atomic blocks"
                "\nor wrong class paths in composite blocks.\n"
                "See build.log file in root of library."));
    }
}
