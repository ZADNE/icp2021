/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "blockeditor.h"

#include <QDebug>

QString BlockEditor::m_libPath = QString{};
BlockEditSignaller BlockEditor::sig = BlockEditSignaller{};

BlockEditor::BlockEditor(QWidget* parent): QWidget(parent){
    connect(&sig, &BlockEditSignaller::blockEdited,
            this, &BlockEditor::blockEdited);
    connect(&sig, &BlockEditSignaller::blockRenamed,
            this, &BlockEditor::blockRenamed);
}

BlockEditor::~BlockEditor(){

}

void BlockEditor::setLibPath(QString libPath){
    m_libPath = libPath;
}

void BlockEditor::setFilePath(QString relPath, bool loadFile){
    if (loadFile){
        //Loading file
        m_relPath = relPath;
        QFile file{m_libPath + relPath};
        if (file.exists()){
            load();
        } else {
            //File does not exist
            if (file.open(QIODevice::WriteOnly)){
                //Create it by saving empty block
                file.close();
                save();
                load();
            }
        }
        m_unsavedChanges = false;
    } else {
        //Renamed this file
        QString oldPath = m_relPath;
        m_relPath = relPath;
        emit sig.blockRenamed(oldPath, relPath);
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
        emit sig.blockEdited(filePath());
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
