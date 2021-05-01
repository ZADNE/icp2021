/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef BLOCKINSTANCE_H
#define BLOCKINSTANCE_H

#include <QWidget>

namespace Ui {
class BlockInstance;
}

class QGraphicsRectItem;

class BlockInstance: public QWidget{
    Q_OBJECT

public:
    explicit BlockInstance(QWidget* parent = nullptr);
    ~BlockInstance();

    bool init(QString relPath, QString name, QGraphicsRectItem* parentRect);

public slots:
    void editPath();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::BlockInstance *ui;

    bool reload();

    QString m_relPath;
    QGraphicsRectItem* m_parentRect = nullptr;
};

#endif // BLOCKINSTANCE_H
