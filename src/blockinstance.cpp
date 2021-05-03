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
    connect(ui->instanceName, &QLineEdit::textChanged,
            this, &BlockInstance::renamedInstance);

    ui->toolButton->addAction(ui->actionEditClass);
    ui->toolButton->addAction(ui->actionEditClassPath);
    ui->toolButton->addAction(ui->actionDelete);

    setAttribute(Qt::WA_DeleteOnClose, true);
}

BlockInstance::~BlockInstance(){
    delete ui;
}

bool BlockInstance::init(QString relPath, QString name, QGraphicsScene* scene, QGraphicsRectItem* parentRect){
    m_scene = scene;
    m_parentRect = parentRect;
    ui->instanceName->setText(name);
    return reload(relPath);
}

void BlockInstance::editPath(){
    QString newPath = QInputDialog::getText(
                nullptr, tr("Change path..."),
                tr("Enter new filepath to class:"), QLineEdit::Normal,
                m_relPath);
    if (!newPath.isNull() && m_relPath != newPath){
        reload(newPath);
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
    for (auto& portWidget: m_inputs) {
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
    //Search input ports
    for (auto& port: m_inputs){
        if (port->getPortName() == portName){
            return port;
        }
    }
    //Search output ports
    for (auto& port: m_outputs){
        if (port->getPortName() == portName){
            return port;
        }
    }
    return nullptr;
}

void BlockInstance::classEdited(QString path){
    if (path == m_relPath || path == ""){
        reload(m_relPath);
    }
}

void BlockInstance::classRenamed(QString oldPath, QString newPath){
    if (oldPath == m_relPath){
        m_relPath = newPath;
        emit changed();
    }
}

void BlockInstance::resizeEvent(QResizeEvent* event){
    QSize size = event->size();
    m_parentRect->setRect(1, 1, size.width() - 2, size.height() - 2);
}

void BlockInstance::closeEvent(QCloseEvent* event){
    //Disconnect inputs
    for (auto& port: m_inputs){
        emit port->disconnectPort(port);
    }
    //Disconnect outputs
    for (auto& port: m_outputs){
        emit port->disconnectPort(port);
    }
    emit changed();
    m_scene->removeItem(m_parentRect);
    event->accept();
}

bool BlockInstance::reload(QString relPath){
    m_relPath = relPath;
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
    //Rebuild ports
    rebuildPorts(inputs, PortType::Input, m_inputs);
    rebuildPorts(outputs, PortType::Output, m_outputs);
    emit changed();
    return rvalue;
}

void BlockInstance::rebuildPorts(const PortList& ports, PortType type, std::vector<PortWidget*>& widgets){
    //Remove ports
    for (size_t i = ports.size(); i < widgets.size(); ++i) {
        emit widgets[i]->disconnectPort(widgets[i]);
        delete widgets[i];
    }
    if (widgets.size() > ports.size()){
        widgets.resize(ports.size());
    }
    //Rename ports
    for (size_t i = 0; i < ports.size() && i < widgets.size(); ++i) {
        widgets[i]->setDataType(QString::fromStdString(ports[i].type));
        widgets[i]->setPortName(QString::fromStdString(ports[i].name));
    }
    //Add new ports
    for (size_t i = widgets.size(); i < ports.size(); ++i) {
        addPort(ports[i], type);
    }
}

void BlockInstance::addPort(const PortSpec& port, PortType type){
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
    if (type == PortType::Input){
        m_inputs.push_back(button);
        ui->inputs->addWidget(button);
    } else {
        m_outputs.push_back(button);
        ui->outputs->addWidget(button);
    }
}

void BlockInstance::renamedInstance(){
    //Inform ports of change
    QString newName = ui->instanceName->text();
    for (auto* w: m_inputs){
        w->setInstName(newName);
    }
    for (auto* w: m_outputs){
        w->setInstName(newName);
    }
}
