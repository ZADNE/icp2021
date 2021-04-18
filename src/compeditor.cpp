#include "compeditor.h"
#include "ui_compeditor.h"

#include <QDebug>

CompEditor::CompEditor(QWidget *parent):
    BlockEditor(parent),
    ui(new Ui::CompEditor)
{
    ui->setupUi(this);
}

CompEditor::~CompEditor(){
    CompEditor::saveWork();
    delete ui;
}

void CompEditor::load(){

}

void CompEditor::save(){
    qDebug() << filePath();
}

void CompEditor::build(){

}
