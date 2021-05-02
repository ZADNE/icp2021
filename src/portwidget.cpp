/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "portwidget.h"
#include "ui_portwidget.h"

#include <QToolButton>
#include <QPushButton>
#include <QInputDialog>
#include <QDebug>
#include <QGraphicsRectItem>

//Little helper
void setFontSize(QWidget* w, int size){
    QFont font = w->font();
    font.setPointSize(size);
    w->setFont(font);
}

PortWidget::PortWidget(PortType portType, QString dataType,
                       QString portName, QString instName,
                       QGraphicsRectItem* parentRect, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::PortWidget),
    m_type(portType),
    m_dataType(dataType),
    m_portName(portName),
    m_instName(instName),
    m_parentRect(parentRect)
{
    ui->setupUi(this);
    //Text
    ui->button->setText(dataType + " " + portName);
    if (portType == PortType::Input){
        ui->toolButton->addAction(ui->actionSetConst);
        ui->toolButton->addAction(ui->actionDisconnect);
    } else {//Output
        ui->toolButton->addAction(ui->actionDisconnect);
        ui->layout->setDirection(QBoxLayout::RightToLeft);
    }

}

PortWidget::~PortWidget(){
    emit disconnectPort(this);
    delete ui;
}

PortType PortWidget::getType() const {
    return m_type;
}

QString PortWidget::getPortName() const {
    return m_portName;
}

bool PortWidget::hasConst() const {
    return m_type == PortType::Input && m_constValue != "";
}

QString PortWidget::getConstValue() const {
    return m_constValue;
}

QPointF PortWidget::getPosition() const {
    QPoint offset = ui->button->pos();
    if (m_type == PortType::Output){
        offset.rx() += ui->button->size().width() - 8;
    } else {
        offset.rx() += 8;
    }
    offset.ry() += ui->button->size().height() / 2;
    offset.rx() += geometry().x();
    offset.ry() += geometry().y();
    QPointF pos = offset;
    pos += m_parentRect->pos();
    return pos;
}

QString PortWidget::getInstName() const{
    return m_instName;
}

void PortWidget::setConst(QString value){
    bool haveConst = value != "";
    ui->button->setEnabled(!haveConst);
    if (haveConst){
        emit disconnectPort(this);
    }
    m_constValue = value;
    emit changed();
}

void PortWidget::openConstDialog(){
    QString newValue = QInputDialog::getText(
                nullptr, tr("Set const..."),
                tr("Enter new constant for ") + m_portName + ":", QLineEdit::Normal,
                m_constValue);
    if (!newValue.isNull() && newValue != m_constValue){
        setConst(newValue);
    }
}

void PortWidget::connectPort_private(){
    emit connectPort(this, getPosition());
}

void PortWidget::disconnectPort_private(){
    emit disconnectPort(this);
}
