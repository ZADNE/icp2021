/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "newfiledialog.h"
#include "ui_newfiledialog.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>

NewFileDialog::NewFileDialog(QWidget *parent, QString libPath, QString relPath) :
    QDialog(parent),
    ui(new Ui::NewFileDialog),
    m_libPath(libPath),
    m_relPath(relPath)
{
    ui->setupUi(this);
    m_type = FileType::dir;
}

NewFileDialog::~NewFileDialog(){
    delete ui;
}

NewFileRequest NewFileDialog::getResult(){
    if (m_fileValid){
        return NewFileRequest{m_type, constructRelPath()};
    } else {
        return NewFileRequest{FileType::none, ""};
    }
}

QString NewFileDialog::constructRelPath(){
    qDebug() << m_relPath + "/" + ui->name->text() + ui->suffix->text();
    return m_relPath + "/" + ui->name->text() + ui->suffix->text();
}

void NewFileDialog::validate(){
    QFileInfo fi{m_libPath + constructRelPath()};

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
    } else if (item->text() == tr("Application")){
        m_type = FileType::appl;
        ui->suffix->setText(".appl");
    } else {
        m_type = FileType::dir;
        ui->suffix->setText("");
    }
    validate();
}

void NewFileDialog::leave(){
    done(m_fileValid);
}
