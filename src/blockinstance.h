/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef BLOCKINSTANCE_H
#define BLOCKINSTANCE_H

#include <map>

#include <QWidget>
#include <QVector>

#include "blockspec.h"
#include "portwidget.h"

namespace Ui {
class BlockInstance;
}

class QGraphicsRectItem;
class QVBoxLayout;
class QGraphicsScene;

class BlockInstance: public QWidget{
    Q_OBJECT

public:
    explicit BlockInstance(QWidget* parent = nullptr);
    ~BlockInstance();

    bool init(QString relPath, QString name, QGraphicsScene* scene, QGraphicsRectItem* parentRect);

signals:
    void editBlock(QString path);
    void changed();
    void connectPort(PortWidget* w, QPointF offset);

public slots:
    void editPath();
    void editClass();
    void appendMySpecs(InstanceList& instl, ConstantList& cnstl);
    void setConstPort(QString portName, QString value);
    PortWidget* getPort(QString portName);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::BlockInstance *ui;

    bool reload();
    void rebuildPorts(const PortList& inputs, const PortList& outputs);
    void addPorts(QVBoxLayout* layout, const PortList& ports, PortType type);

    QString m_relPath;
    QGraphicsRectItem* m_parentRect = nullptr;
    QGraphicsScene* m_scene = nullptr;
    QVector<PortWidget*> m_portWidgets;
};

#endif // BLOCKINSTANCE_H
