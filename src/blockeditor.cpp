/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#include "blockeditor.h"

#include <QDebug>

BlockEditor::BlockEditor(QWidget *parent) : QWidget(parent){

}

void BlockEditor::setFilePath(QString filePath){
    m_filePath = filePath;
}

QString BlockEditor::filePath() const{
    return m_filePath;
}

void BlockEditor::saveWork(){
    qDebug() << "BlockEditor::saveWork()";
}
