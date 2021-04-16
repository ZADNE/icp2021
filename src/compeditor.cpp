#include "compeditor.h"
#include "ui_compeditor.h"

#include <QDebug>

CompEditor::CompEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompEditor)
{
    ui->setupUi(this);
}

CompEditor::~CompEditor(){
    saveWork();
    delete ui;
}

void CompEditor::saveWork(){
    qDebug() << accessibleDescription();
}
