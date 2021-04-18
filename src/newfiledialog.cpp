#include "newfiledialog.h"
#include "ui_newfiledialog.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>

NewFileDialog::NewFileDialog(QWidget *parent, QString workdir) :
    QDialog(parent),
    ui(new Ui::NewFileDialog),
    m_workDir(workdir)
{
    ui->setupUi(this);
    m_type = FileType::dir;
}

NewFileDialog::~NewFileDialog(){
    delete ui;
}

NewFileRequest NewFileDialog::getResult(){
    if (m_fileValid){
        return NewFileRequest{m_type, constructFilePath()};
    } else {
        return NewFileRequest{FileType::none, ""};
    }
}

QString NewFileDialog::constructFilePath(){
    return m_workDir + "/" + ui->name->text() + ui->suffix->text();
}

void NewFileDialog::validate(){
    QFileInfo fi{constructFilePath()};

    m_fileValid = !fi.exists() && !ui->name->text().isEmpty();
    ui->add->setEnabled(m_fileValid);
}

void NewFileDialog::typeChosen(QListWidgetItem* item){
    if (item->text() == tr("Atomic block")){
        m_type = FileType::atom;
        ui->suffix->setText(".atom");
    } else if (item->text() == tr("Composite block")){
        m_type = FileType::comp;
        ui->suffix->setText(".comp");
    } else {
        m_type = FileType::dir;
        ui->suffix->setText("");
    }
    validate();
}

void NewFileDialog::leave(){
    done(m_fileValid);
}
