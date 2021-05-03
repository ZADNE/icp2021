/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef PORTWIDGET_H
#define PORTWIDGET_H

#include <QWidget>

class QPushButton;

namespace Ui {
class PortWidget;
}

class QGraphicsRectItem;

enum class PortType{
    Input, Output
};

///
/// \brief Small widget representing a port inside instance of a block
///
class PortWidget: public QWidget{
    Q_OBJECT

public:
    explicit PortWidget(
            PortType portType, QString dataType,
            QString portName, QString instName,
            QGraphicsRectItem* parentRect, QWidget* parent = nullptr);
    ~PortWidget();

    PortType getType() const;
    QString getPortName() const;
    bool hasConst() const;
    QString getConstValue() const;
    QPointF getPosition() const;
    QString getInstName() const;

    void setPortName(QString name);
    void setDataType(QString dataType);
    void setInstName(QString name);


signals:
    void changed();
    void connectPort(PortWidget* w, QPointF offset);
    void disconnectPort(PortWidget* w);

public slots:
    void setConst(QString value);

protected slots:
    void openConstDialog();

private:
    Ui::PortWidget *ui;

    PortType m_type;
    QString m_dataType;
    QString m_portName;
    QString m_constValue;
    QString m_instName;

    QGraphicsRectItem* m_parentRect = nullptr;
    void updateButtonText();
private slots:
    void connectPort_private();
    void disconnectPort_private();
};

#endif // PORTWIDGET_H
