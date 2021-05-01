/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "blockinstance.h"
#include "ui_blockinstance.h"

#include <QIcon>
#include <QDebug>
#include <QInputDialog>
#include <QGraphicsItem>
#include <QResizeEvent>

#include "blockspec.h"
#include "speccache.h"

BlockInstance::BlockInstance(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::BlockInstance)
{
    ui->setupUi(this);
}

BlockInstance::~BlockInstance(){
    delete ui;
}

bool BlockInstance::init(QString relPath, QString name, QGraphicsRectItem* parentRect){
    m_relPath = relPath;
    m_parentRect = parentRect;
    ui->instanceName->setText(name);
    return reload();
}

void BlockInstance::editPath(){
    QString newPath = QInputDialog::getText(
                nullptr, tr("Change path..."),
                tr("Enter new filepath to class:"), QLineEdit::Normal,
                m_relPath);
    if (!newPath.isNull()){
        m_relPath = newPath;
        reload();
    }
}

void BlockInstance::resizeEvent(QResizeEvent* event){
    QSize size = event->size();
    m_parentRect->setRect(1, 1, size.width() - 2, size.height() - 2);
}

bool BlockInstance::reload(){
    QString className = tr("<invalid-path>");
    bool rvalue = true;
    const AnySpec& spec = SpecCache::fetchAny(m_relPath.toStdString());
    if (std::holds_alternative<AtomSpec>(spec)){
        AtomSpec atom = std::get<AtomSpec>(spec);
        className = QString::fromStdString(atom.name);
    } else if (std::holds_alternative<CompSpec>(spec)) {
        CompSpec comp = std::get<CompSpec>(spec);
        className = QString::fromStdString(comp.name);
    } else {
        rvalue = false;
    }
    ui->classLabel->setText(className);
    return rvalue;
}
