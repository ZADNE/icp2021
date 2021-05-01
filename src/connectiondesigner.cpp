/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "connectiondesigner.h"

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

    /*connect(m_gScene, &QGraphicsScene::changed,
            this, &ConnectionDesigner::changed);*/

}

ConnectionDesigner::~ConnectionDesigner(){

}

void ConnectionDesigner::setMyPath(QString path){
    m_myPath = path;
}

bool ConnectionDesigner::addBlock(QString relPath, QString name, QPoint pos){
    if (m_myPath == relPath) return false; //Cannot add itself
    auto* inst = new BlockInstance();
    auto* item = new QGraphicsRectItem(1, 1, inst->size().width() - 2, inst->size().height() - 2);
    if (!inst->init(relPath, name, item)){
        //Could not init the instance (probably wrong path)
        delete inst;
        delete item;
        return false;
    }
    auto* proxy = new QGraphicsProxyWidget(item);
    proxy->setWidget(inst);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setPos(pos);
    m_gScene->addItem(item);
    return true;
}

void ConnectionDesigner::resizeEvent(QResizeEvent* event){
    m_gScene->setSceneRect(QRectF(QPointF(0, 0), event->size()));
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
        addBlock(relPath, "i" + QString::number(m_instanceN++), event->pos());
    } else {
        event->ignore();
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
