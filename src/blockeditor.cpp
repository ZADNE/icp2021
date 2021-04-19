/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#include "blockeditor.h"

#include <QDebug>

BlockEditor::BlockEditor(QWidget *parent) : QWidget(parent){

}

BlockEditor::~BlockEditor(){

}

void BlockEditor::setFilePath(QString filePath, bool loadFile){
    m_fileInfo = filePath;
    m_fileInfo.makeAbsolute();
    if (loadFile){
        QFile file{filePath};
        if (file.exists()){
            load();
        } else {
            //File does not exist
            if (file.open(QIODevice::WriteOnly)){
                //Create it by saving empty block
                file.close();
                save();
            }
        }
        m_unsavedChanges = false;
    }
}

QString BlockEditor::filePath() const{
    return m_fileInfo.absoluteFilePath();
}

void BlockEditor::saveWork(){
    m_fileInfo.refresh();
    if (m_unsavedChanges == true && m_fileInfo.exists()){
        m_unsavedChanges = false;
        save();
        emit withoutUnsavedChanges(this);
    }
}

void BlockEditor::buildWork(){
    saveWork();
    build();
}

void BlockEditor::editedWork(){
    if (m_unsavedChanges == false){
        m_unsavedChanges = true;
        emit havingUnsavedChanges(this);
    }
}
