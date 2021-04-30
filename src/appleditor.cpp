/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "appleditor.h"
#include "ui_appleditor.h"

ApplEditor::ApplEditor(QWidget *parent):
    BlockEditor(parent),
    ui(new Ui::ApplEditor)
{
    ui->setupUi(this);


}

ApplEditor::~ApplEditor(){
    delete ui;
}

void ApplEditor::load(){

}

void ApplEditor::save(){

}

void ApplEditor::build(){

}
