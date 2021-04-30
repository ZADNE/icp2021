/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef CONNECTIONDESIGNER_H
#define CONNECTIONDESIGNER_H

#include <QWidget>
#include <QGraphicsView>

#include "blockspec.h"

class QGraphicsScene;

class ConnectionDesigner: public QGraphicsView{
    Q_OBJECT

public:
    explicit ConnectionDesigner(QWidget* parent = nullptr);
    ~ConnectionDesigner();

public slots:
    bool addBlock(QString filePath, QString name, QPoint pos);

signals:
    void changed();

protected:
    virtual void resizeEvent(QResizeEvent* event) override;

    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dragMoveEvent(QDragMoveEvent* event) override;
    virtual void dragLeaveEvent(QDragLeaveEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;

private:
    QGraphicsScene* m_gScene;
    //This block
    QString m_filePath;

    InstanceList m_il;
};

#endif // CONNECTIONDESIGNER_H
