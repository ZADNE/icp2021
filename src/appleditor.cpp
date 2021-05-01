/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "appleditor.h"
#include "ui_appleditor.h"

#include <QDebug>

#include "blockcompiler.h"
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
}

ApplEditor::~ApplEditor(){
    delete ui;
}

void ApplEditor::load(){
    ui->designer->setMyPath(filePath());
    qDebug() << "Appl load " << filePath();
}

void ApplEditor::save(){
    //Compose specs
    auto spec = ApplSpec{};
    spec.name = "Aplikace";
    spec.instances.emplace_back("com", "cmp.comp", 0, 0);
    spec.constants.emplace_back("5", "com", "in0");
    //Write specs
    SpecCache::save(filePath().toStdString(), spec);
}

void ApplEditor::build(){
    BlockCompiler::get().buildAppl(filePath().toStdString());
}
