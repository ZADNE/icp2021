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
    comp
};

struct NewFileRequest{
    NewFileRequest(FileType type, QString path):
        type(type), path(path){}

    FileType type;
    QString path;
};

class NewFileDialog : public QDialog{
    Q_OBJECT

public:
    explicit NewFileDialog(QWidget *parent = nullptr, QString workdir = "");
    ~NewFileDialog();

    NewFileRequest getResult();

private:
    Ui::NewFileDialog *ui;

    QString constructFilePath();

    QString m_workDir;
    bool m_fileValid = false;
    FileType m_type = FileType::none;

private slots:
    void validate();
    void typeChosen(QListWidgetItem* item);
    void leave();
};

#endif // NEWFILEDIALOG_H
