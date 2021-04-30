/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "connectiondesigner.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QScrollBar>

#include "blockcompiler.h"

ConnectionDesigner::ConnectionDesigner(QWidget* parent) :
    QGraphicsView(parent),
    m_gScene(new QGraphicsScene(this))
{
    setAcceptDrops(true);
    setScene(m_gScene);
    //m_gScene->setSceneRect(QRectF(QPointF(0, 0), size()));

    /*connect(m_gScene, &QGraphicsScene::changed,
            this, &ConnectionDesigner::changed);*/

}

ConnectionDesigner::~ConnectionDesigner(){

}

bool ConnectionDesigner::addBlock(QString filePath, QString name, QPoint pos){
    if (filePath == m_filePath) return false; //Cannot add itself
    QFileInfo fi{filePath};
    if (!fi.isFile()) return false;
    if (fi.suffix() == "atom"){
        AtomSpec spec;
        if (!BlockCompiler::get().readAtom(filePath.toStdString(), spec)) return false;
        m_il.emplace_back(
                    name.toStdString(),
                    filePath.toStdString(),
                    pos.x(), pos.y());
        m_gScene->addRect(pos.x(), pos.y(), 20, 20);
    } else if (fi.suffix() == "comp"){
        return false;
    } else {
        return false;
    }
    return true;
}

void ConnectionDesigner::resizeEvent(QResizeEvent* event){
    //m_gScene->setSceneRect(QRectF(QPointF(0, 0), event->size()));
}

void ConnectionDesigner::dragEnterEvent(QDragEnterEvent* event){
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void ConnectionDesigner::dragMoveEvent(QDragMoveEvent* event){
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void ConnectionDesigner::dragLeaveEvent(QDragLeaveEvent* event){
    event->ignore();
}

void ConnectionDesigner::dropEvent(QDropEvent* event){
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        QByteArray mime = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
        QDataStream stream(&mime, QIODevice::ReadOnly);
        while (!stream.atEnd()){
            int dontcare;
            QMap<int,  QVariant> roleDataMap;
            stream >> dontcare >> dontcare >> roleDataMap;
            QString filePath = roleDataMap[Qt::UserRole].toString();
            addBlock(filePath, "i" + QString::number(m_il.size()),event->pos());
        }
    } else {
        event->ignore();
    }
}
