/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QListWidgetItem>

namespace Ui {
class NewFileDialog;
}

enum class FileType{
    none,
    dir,
    atom,
    comp,
    appl
};

///
/// \brief Result a new file request dialog
///
struct NewFileRequest{
    NewFileRequest(FileType type, QString path):
        type(type), path(path){}

    FileType type;
    QString path;
};

///
/// \brief Dialog for creating new file in library
///
class NewFileDialog : public QDialog{
    Q_OBJECT

public:
    explicit NewFileDialog(QWidget *parent, QString libPath, QString relPath);
    ~NewFileDialog();

    NewFileRequest getResult();

private:
    Ui::NewFileDialog *ui;

    QString constructRelPath();

    QString m_libPath;
    QString m_relPath;
    bool m_fileValid = false;
    FileType m_type = FileType::none;

private slots:
    void validate();
    void typeChosen(QListWidgetItem* item);
    void leave();
};

#endif // NEWFILEDIALOG_H
