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
#include <QToolButton>
#include <QPushButton>
#include <QGraphicsScene>

#include "blockspec.h"
#include "speccache.h"

BlockInstance::BlockInstance(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::BlockInstance)
{
    ui->setupUi(this);

    connect(ui->instanceName, &QLineEdit::textChanged,
            this, &BlockInstance::changed);

    ui->toolButton->addAction(ui->actionEditClass);
    ui->toolButton->addAction(ui->actionEditClassPath);
    ui->toolButton->addAction(ui->actionDelete);

    setAttribute(Qt::WA_DeleteOnClose, true);
}

BlockInstance::~BlockInstance(){
    delete ui;
}

bool BlockInstance::init(QString relPath, QString name, QGraphicsScene* scene, QGraphicsRectItem* parentRect){
    m_relPath = relPath;
    m_scene = scene;
    m_parentRect = parentRect;
    ui->instanceName->setText(name);
    return reload();
}

void BlockInstance::editPath(){
    QString newPath = QInputDialog::getText(
                nullptr, tr("Change path..."),
                tr("Enter new filepath to class:"), QLineEdit::Normal,
                m_relPath);
    if (!newPath.isNull() && m_relPath != newPath){
        m_relPath = newPath;
        reload();
    }
}

void BlockInstance::editClass(){
    emit editBlock(m_relPath);
}

void BlockInstance::appendMySpecs(InstanceList& instl, ConstantList& cnstl){
    //Instance
    std::string instName = ui->instanceName->text().toStdString();
    instl.emplace_back(instName, m_relPath.toStdString(), m_parentRect->pos().x(), m_parentRect->pos().y());
    //Constants
    for (auto& portWidget: m_portWidgets) {
        if (portWidget->hasConst()){
            cnstl.emplace_back(portWidget->getConstValue().toStdString(),
                               instName,
                               portWidget->getPortName().toStdString());
        }
    }
}

void BlockInstance::setConstPort(QString portName, QString value){
    auto* port = getPort(portName);
    if (port){
        port->setConst(value);
    }
}

PortWidget* BlockInstance::getPort(QString portName){
    for (int i = 0; i < m_portWidgets.size(); ++i) {
        if (m_portWidgets[i]->getPortName() == portName){
            return m_portWidgets[i];
        }
    }
    return nullptr;
}

void BlockInstance::resizeEvent(QResizeEvent* event){
    QSize size = event->size();
    m_parentRect->setRect(1, 1, size.width() - 2, size.height() - 2);
}

void BlockInstance::closeEvent(QCloseEvent* event){
    for (int i = 0; i < m_portWidgets.size(); ++i) {
        emit m_portWidgets[i]->disconnectPort(m_portWidgets[i]);
    }
    emit changed();
    m_scene->removeItem(m_parentRect);
    event->accept();
}

bool BlockInstance::reload(){
    QString className = tr("<invalid-path>");
    PortList inputs;
    PortList outputs;
    QString icon;
    bool rvalue = true;
    const AnySpec& spec = SpecCache::fetchAny(m_relPath.toStdString());
    if (std::holds_alternative<AtomSpec>(spec)){
        AtomSpec atom = std::get<AtomSpec>(spec);
        className = QString::fromStdString(atom.name);
        inputs = atom.inputs;
        outputs = atom.outputs;
        icon = "atom";
    } else if (std::holds_alternative<CompSpec>(spec)) {
        CompSpec comp = std::get<CompSpec>(spec);
        className = QString::fromStdString(comp.name);
        inputs = comp.inputs;
        outputs = comp.outputs;
        icon = "comp";
    } else {
        rvalue = false;
    }
    ui->iconLabel->setPixmap(QIcon{icon}.pixmap(16, 16));
    ui->classLabel->setText(className);
    //Rebuild slots
    rebuildPorts(inputs, outputs);
    emit changed();
    return rvalue;
}

void BlockInstance::rebuildPorts(const PortList& inputs, const PortList& outputs){
    //Clear previous buttons
    for (int i = 0; i < m_portWidgets.size(); ++i) {
        emit m_portWidgets[i]->disconnectPort(m_portWidgets[i]);
        delete m_portWidgets[i];
    }
    m_portWidgets.clear();
    //Create new buttons
    addPorts(ui->inputs, inputs, PortType::Input);
    addPorts(ui->outputs, outputs, PortType::Output);
}

void BlockInstance::addPorts(QVBoxLayout* layout, const PortList& ports, PortType type){
    for (auto& port: ports) {
        //Create button
        auto* button = new PortWidget(
                    type,
                    QString::fromStdString(port.type),
                    QString::fromStdString(port.name),
                    ui->instanceName->text(),
                    m_parentRect);
        connect(button, &PortWidget::changed,
                this, &BlockInstance::changed);
        connect(button, &PortWidget::connectPort,
                this, &BlockInstance::connectPort);
        //Add the button to layout
        layout->addWidget(button);
        m_portWidgets.append(button);
    }
}
