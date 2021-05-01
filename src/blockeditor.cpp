/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "blockeditor.h"

#include <QDebug>

QString BlockEditor::m_libPath = QString{};

BlockEditor::BlockEditor(QWidget *parent) : QWidget(parent){

}

BlockEditor::~BlockEditor(){

}

void BlockEditor::setLibPath(QString libPath){
    m_libPath = libPath;
}

void BlockEditor::setFilePath(QString relPath, bool loadFile){
    m_relPath = relPath;
    if (loadFile){
        QFile file{m_libPath + relPath};
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
    return m_relPath;
}

void BlockEditor::saveWork(){
    if (m_unsavedChanges == true){
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
