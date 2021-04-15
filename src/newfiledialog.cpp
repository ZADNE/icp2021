#include "newfiledialog.h"
#include "ui_newfiledialog.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>

NewFileDialog::NewFileDialog(QWidget *parent, QString workdir) :
    QDialog(parent),
    ui(new Ui::NewFileDialog),
    workDir(workdir)
{
    ui->setupUi(this);
}

NewFileDialog::~NewFileDialog(){
    delete ui;
}

NewFileResult NewFileDialog::getResult(){
    if (fileValid){
        return NewFileResult{type, constructFilePath()};
    } else {
        return NewFileResult{FileType::none, ""};
    }
}

QString NewFileDialog::constructFilePath(){
    return workDir + QDir::separator() +
        ui->name->text() + ui->suffix->text();
}

void NewFileDialog::validate(){
    QFileInfo fi{constructFilePath()};

    fileValid = !fi.exists() && !ui->name->text().isEmpty();
    ui->add->setEnabled(fileValid);
}

void NewFileDialog::typeChosen(QListWidgetItem* item){
    if (item->text() == tr("Atomic block")){
        type = FileType::atom;
        ui->suffix->setText(".atom");
    } else if (item->text() == tr("Composite block")){
        type = FileType::comp;
        ui->suffix->setText(".comp");
    } else {
        type = FileType::dir;
        ui->suffix->setText("");
    }
    validate();
}

void NewFileDialog::leave(){
    done(fileValid);
}
