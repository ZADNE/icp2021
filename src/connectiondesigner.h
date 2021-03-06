/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef CONNECTIONDESIGNER_H
#define CONNECTIONDESIGNER_H
#include <set>

#include <QWidget>
#include <QGraphicsView>
#include <QMimeData>
#include <QGraphicsLineItem>

#include "blockspec.h"
#include "portwidget.h"

class QGraphicsScene;
class BlockInstance;

///
/// \brief Designer of block instances and connections among them
///
class ConnectionDesigner: public QGraphicsView{
    Q_OBJECT

public:
    explicit ConnectionDesigner(QWidget* parent = nullptr);
    ~ConnectionDesigner();

    void setMyPath(QString path);

signals:
    void blockEdited(QString path);
    void blockRenamed(QString oldPath, QString newPath);

    void editBlock(QString path);
    void changed();
    void appendInstanceSpecs(InstanceList& instl, ConstantList& cnstl);

public slots:
    //Returns nullptr on error
    BlockInstance* addBlock(QString relPath, QString name, QPoint pos);
    void insertSpecs(InstanceList& instl, ConnectionList& connl, ConstantList& cnstl);
    void connectPort(PortWidget* w, QPointF pos);
    void collectSpecs(InstanceList& instl, ConnectionList& connl, ConstantList& cnstl);

    void reloadPorts(PortType type, const PortList& ports);

protected:
    void resizeEvent(QResizeEvent* event) override;

    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

protected slots:
    void filterGraphicsChanges(const QList<QRectF>& region);

private:
    QString extractPathFromMime(const QMimeData& mime);

    bool validPath(const QString& path);

    QGraphicsScene* m_gScene;
    size_t m_instanceN = 0;
    QString m_myPath;//path of this block
    QPointF m_lastChangePos;

    //My ports
    using PortWidgetList = std::vector<std::pair<PortWidget*, QGraphicsRectItem*>>;
    PortWidgetList m_inputs;
    PortWidgetList m_outputs;
    void rebuildPorts(const PortList& ports, PortType type, PortWidgetList& widgets);
    PortWidget* addPort(const PortSpec& port, PortType type);
    void repositionPorts();

    //Connection related
    struct Connection{
        Connection(PortWidget* from, PortWidget* to, QGraphicsLineItem* line):
            from(from),
            to(to),
            line(line)
        {}

        bool operator<(const Connection& other) const {
            return line < other.line;
        }

        PortWidget* from;
        PortWidget* to;
        QGraphicsLineItem* line;
    };
    std::set<Connection> m_connections;
    QGraphicsLineItem* m_line = nullptr;
    bool m_connecting = false;
    QPointF m_connStart;
    PortWidget* m_connFrom;
    QPen m_connPen{Qt::black, 2, Qt::SolidLine};
    void connectPorts(PortWidget* from, PortWidget* to);
    void redrawConnections();
private slots:
    void disconnectPort(PortWidget* port);
};

#endif // CONNECTIONDESIGNER_H
