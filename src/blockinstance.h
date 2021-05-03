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

///
/// \brief Widget representing instance of a block in a connection editor
///
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
    void classEdited(QString path);
    void classRenamed(QString oldPath, QString newPath);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::BlockInstance *ui;

    bool reload(QString relPath);
    void rebuildPorts(const PortList& ports, PortType type, std::vector<PortWidget*>& widgets);
    void addPort(const PortSpec& port, PortType type);

    QString m_relPath;
    QGraphicsRectItem* m_parentRect = nullptr;
    QGraphicsScene* m_scene = nullptr;
    std::vector<PortWidget*> m_inputs;
    std::vector<PortWidget*> m_outputs;
private slots:
    void renamedInstance();
};

#endif // BLOCKINSTANCE_H
