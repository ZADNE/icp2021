/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "connectiondesigner.h"

#include <unordered_map>

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDropEvent>
#include <QMimeData>
#include <QGraphicsProxyWidget>

#include "libraryexplorer.h"
#include "blockinstance.h"
#include "speccache.h"

ConnectionDesigner::ConnectionDesigner(QWidget* parent) :
    QGraphicsView(parent),
    m_gScene(new QGraphicsScene(this))
{
    setAcceptDrops(true);
    setScene(m_gScene);
    m_gScene->setSceneRect(QRectF(QPointF(0, 0), size()));

    connect(m_gScene, &QGraphicsScene::changed,
            this, &ConnectionDesigner::filterGraphicsChanges);

}

ConnectionDesigner::~ConnectionDesigner(){

}

void ConnectionDesigner::setMyPath(QString path){
    m_myPath = path;
}

BlockInstance* ConnectionDesigner::addBlock(QString relPath, QString name, QPoint pos){
    if (m_myPath == relPath) return nullptr; //Cannot add itself
    auto* inst = new BlockInstance();
    auto* item = new QGraphicsRectItem();
    if (!inst->init(relPath, name, m_gScene, item)){
        //Could not init the instance (probably wrong path)
        delete inst;
        delete item;
        return nullptr;
    }
    connect(inst, &BlockInstance::changed,
            this, &ConnectionDesigner::changed);
    connect(inst, &BlockInstance::editBlock,
            this, &ConnectionDesigner::editBlock);
    connect(inst, &BlockInstance::connectPort,
            this, &ConnectionDesigner::connectPort);

    connect(this, &ConnectionDesigner::appendInstanceSpecs,
            inst, &BlockInstance::appendMySpecs);
    connect(this, &ConnectionDesigner::blockEdited,
            inst, &BlockInstance::classEdited);
    connect(this, &ConnectionDesigner::blockRenamed,
            inst, &BlockInstance::classRenamed);
    auto* proxy = new QGraphicsProxyWidget(item);
    proxy->setWidget(inst);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setPos(pos);
    m_gScene->addItem(item);
    emit changed();
    m_instanceN++;
    return inst;
}

void ConnectionDesigner::insertSpecs(InstanceList& instl, ConnectionList& connl, ConstantList& cnstl){
    std::unordered_map<std::string, PortWidget*> ports;
    //Instances
    for (auto& inst: instl){
        QString path = QString::fromStdString(inst.path);
        QString name = QString::fromStdString(inst.name);
        auto* block = addBlock(path, name, QPoint(inst.x, inst.y));
        if (!block) continue;
        //Constants of this instance
        for (auto& cnst: cnstl){
            if (cnst.to == inst.name){
                block->setConstPort(
                    QString::fromStdString(cnst.to_port),
                    QString::fromStdString(cnst.value));
            }
        }
        //Connections from/to this instance
        for (auto& conn: connl){
            if (conn.from == inst.name){
                std::string portName = conn.from_port;
                auto* port = block->getPort(QString::fromStdString(portName));
                if (port){
                    ports.insert(std::make_pair(inst.name + "." + portName, port));
                }
            }
            if (conn.to == inst.name){
                std::string portName = conn.to_port;
                auto* port = block->getPort(QString::fromStdString(portName));
                if (port){
                    ports.insert(std::make_pair(inst.name + "." + portName, port));
                }
            }
        }
    }
    //Connections
    for (auto& conn: connl){
        PortWidget* from = nullptr;
        if (conn.from != ""){
            auto it = ports.find(conn.from + "." + conn.from_port);
            if (it == ports.end()) continue;
            from = it->second;
        } else {
            for (auto& pair: m_inputs){
                if (pair.first->getPortName().toStdString() == conn.from_port){
                    from = pair.first;
                    break;
                }
            }
        }
        PortWidget* to = nullptr;
        if (conn.to != ""){
            auto it = ports.find(conn.to + "." + conn.to_port);
            if (it == ports.end()) continue;
            to = it->second;
        } else {
            for (auto& pair: m_outputs){
                if (pair.first->getPortName().toStdString() == conn.to_port){
                    to = pair.first;
                    break;
                }
            }
        }
        if (!from || !to) continue;
        connectPorts(from, to);
    }
    redrawConnections();
}

void ConnectionDesigner::connectPort(PortWidget* w, QPointF pos){
    if (!m_connecting){
        if (w->getType() != PortType::Output) return;
        //Starting connection
        m_connStart = pos;
        m_connFrom = w;
        if (!m_line){
            m_line = m_gScene->addLine(0, 0, 0, 0);
            m_line->setZValue(100);
        }
        m_line->setPos(m_connStart);
        m_line->setPen(m_connPen);
    } else {
        if (w->getType() != PortType::Input) return;
        //Finishing connection
        m_line->setPos(-10000, -10000);
        connectPorts(m_connFrom, w);
        emit changed();
    }

    m_line->setLine(0, 0, 0, 0);
    m_connecting = !m_connecting;
}

void ConnectionDesigner::collectSpecs(InstanceList& instl, ConnectionList& connl, ConstantList& cnstl){
    //Instances
    emit appendInstanceSpecs(instl, cnstl);
    //Connections
    for (auto it = m_connections.begin(); it != m_connections.end(); ++it) {
        auto* from = it->from;
        auto* to = it->to;
        connl.emplace_back(from->getInstName().toStdString(), from->getPortName().toStdString(),
                           to->getInstName().toStdString(), to->getPortName().toStdString());
    }
}

void ConnectionDesigner::reloadPorts(PortType type, const PortList& ports){
    if (type == PortType::Input){
        rebuildPorts(ports, PortType::Input, m_inputs);
    } else {
        rebuildPorts(ports, PortType::Output, m_outputs);
    }
    repositionPorts();
    redrawConnections();
}

void ConnectionDesigner::resizeEvent(QResizeEvent* event){
    m_gScene->setSceneRect(QRectF(QPointF(0, 0), event->size()));
    repositionPorts();
    redrawConnections();
}

void ConnectionDesigner::dragEnterEvent(QDragEnterEvent* event){
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        if (validPath(extractPathFromMime(*event->mimeData()))){
            event->setDropAction(Qt::MoveAction);
            event->accept();
            return;
        }
    }
    event->ignore();
}

void ConnectionDesigner::dragMoveEvent(QDragMoveEvent* event){
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        if (validPath(extractPathFromMime(*event->mimeData()))){
            event->setDropAction(Qt::MoveAction);
            event->accept();
            return;
        }
    }
    event->ignore();
}

void ConnectionDesigner::dropEvent(QDropEvent* event){
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        QString relPath = extractPathFromMime(*event->mimeData());
        addBlock(relPath, "i" + QString::number(m_instanceN), event->pos());
    } else {
        event->ignore();
    }
}

void ConnectionDesigner::mouseMoveEvent(QMouseEvent* event){
    if (m_connecting){
        m_line->setLine(0, 0, event->pos().x() - m_connStart.x(),
                                event->pos().y() - m_connStart.y());
        event->accept();
    } else {
        event->ignore();
        QGraphicsView::mouseMoveEvent(event);
    }
}

void ConnectionDesigner::filterGraphicsChanges(const QList<QRectF>& region){
    //Heuristics to filter 'fake' changes from QGraphicsScene
    if (region.size() > 0){
        const QRectF& rect = region[0];
        if ((rect.x() != 0 && rect.y() != 0) && rect.bottomLeft() != m_lastChangePos){
            m_lastChangePos = rect.bottomLeft();
            redrawConnections();
            emit changed();
        }
    }
}

QString ConnectionDesigner::extractPathFromMime(const QMimeData& mime){
    QByteArray data = mime.data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&data, QIODevice::ReadOnly);
    int dontcare;
    QMap<int,  QVariant> roleDataMap;
    stream >> dontcare >> dontcare >> roleDataMap;
    return roleDataMap[REL_PATH_ROLE].toString();
}

bool ConnectionDesigner::validPath(const QString& path){
    if (path != m_myPath){
        const AnySpec& spec = SpecCache::fetchAny(path.toStdString());
        if (std::holds_alternative<AtomSpec>(spec)
                || std::holds_alternative<CompSpec>(spec)){
            return true;
        }
    }
    return false;
}

void ConnectionDesigner::rebuildPorts(const PortList& ports, PortType type, PortWidgetList& widgets){
    //Remove ports
    for (size_t i = ports.size(); i < widgets.size(); ++i) {
        emit widgets[i].first->disconnectPort(widgets[i].first);
        delete widgets[i].first;
        m_gScene->removeItem(widgets[i].second);
        delete widgets[i].second;
    }
    if (widgets.size() > ports.size()){
        widgets.resize(ports.size());
    }
    //Rename ports
    for (size_t i = 0; i < ports.size() && i < widgets.size(); ++i) {
        widgets[i].first->setDataType(QString::fromStdString(ports[i].type));
        widgets[i].first->setPortName(QString::fromStdString(ports[i].name));
    }
    //Add new ports
    for (size_t i = widgets.size(); i < ports.size(); ++i) {
        addPort(ports[i], type);
    }
}

PortWidget* ConnectionDesigner::addPort(const PortSpec& port, PortType type){
    //Create button
    auto* item = new QGraphicsRectItem();
    auto* portW = new PortWidget(
                type == PortType::Input ? PortType::Output : PortType::Input,
                QString::fromStdString(port.type),
                QString::fromStdString(port.name),
                "",
                item);
    connect(portW, &PortWidget::changed,
            this, &ConnectionDesigner::changed);
    connect(portW, &PortWidget::connectPort,
            this, &ConnectionDesigner::connectPort);
    //Add the button to layout
    if (type == PortType::Input){
        m_inputs.emplace_back(portW, item);
    } else {
        m_outputs.emplace_back(portW, item);
    }
    auto* proxy = new QGraphicsProxyWidget(item);
    proxy->setWidget(portW);
    m_gScene->addItem(item);
    return portW;
}

void ConnectionDesigner::repositionPorts(){
    //Recalculate potitions of port widgets
    qreal i = 0;
    for (auto& pair: m_inputs){
        pair.second->setPos(0, (i + 0.5) / (qreal)m_inputs.size() * (qreal)size().height());
        i++;
    }
    i = 0;
    for (auto& pair: m_outputs){
        pair.second->setPos((qreal)size().width() - (qreal)pair.first->size().width() + 23.0,
                            (i + 0.5) / (qreal)m_outputs.size() * (qreal)size().height());
        i++;
    }
    m_gScene->update();
}

void ConnectionDesigner::connectPorts(PortWidget* from, PortWidget* to){
    auto fromPos = from->getPosition();
    auto toPos = to->getPosition();
    auto* connLine = m_gScene->addLine(
            0, 0, toPos.x() - fromPos.x(), toPos.y() - fromPos.y());
    connLine->setPos(fromPos);
    connLine->setPen(m_connPen);
    m_connections.emplace(from, to, connLine);
    //Connect disconnecting signals
    connect(from, &PortWidget::disconnectPort,
            this, &ConnectionDesigner::disconnectPort);
    connect(to, &PortWidget::disconnectPort,
            this, &ConnectionDesigner::disconnectPort);
}

void ConnectionDesigner::redrawConnections(){
    for (auto it = m_connections.begin(); it != m_connections.end(); ++it) {
        auto fromPos = it->from->getPosition();
        auto toPos = it->to->getPosition();
        it->line->setPos(fromPos);
        it->line->setLine(0, 0, toPos.x() - fromPos.x(),
                                toPos.y() - fromPos.y());
    }
}

void ConnectionDesigner::disconnectPort(PortWidget* port){
    for (auto it = m_connections.begin(); it != m_connections.end(); ) {
        if (it->from == port || it->to == port){
            m_gScene->removeItem(it->line);
            delete it->line;
            it = m_connections.erase(it);
        } else {
            ++it;
        }
    }
    emit changed();
}
