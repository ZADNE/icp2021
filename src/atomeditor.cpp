/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#include "atomeditor.h"
#include "ui_atomeditor.h"

#include <QDebug>
#include <QFile>

AtomEditor::AtomEditor(QWidget *parent):
    BlockEditor(parent),
    ui(new Ui::AtomEditor)
{
    ui->setupUi(this);
}

AtomEditor::~AtomEditor(){
    saveWork();
    delete ui;
}

void AtomEditor::saveWork(){
    qDebug() << filePath();
}
