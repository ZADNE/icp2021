/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef CONNECTIONDESIGNER_H
#define CONNECTIONDESIGNER_H

#include <QWidget>
#include <QGraphicsView>
#include <QMimeData>

class QGraphicsScene;

class ConnectionDesigner: public QGraphicsView{
    Q_OBJECT

public:
    explicit ConnectionDesigner(QWidget* parent = nullptr);
    ~ConnectionDesigner();

    void setMyPath(QString path);

public slots:
    bool addBlock(QString relPath, QString name, QPoint pos);

signals:
    void changed();

protected:
    virtual void resizeEvent(QResizeEvent* event) override;

    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dragMoveEvent(QDragMoveEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;

private:
    QString extractPathFromMime(const QMimeData& mime);

    bool validPath(const QString& path);

    QGraphicsScene* m_gScene;
    int m_instanceN = 0;
    //This block
    QString m_myPath;
};

#endif // CONNECTIONDESIGNER_H
