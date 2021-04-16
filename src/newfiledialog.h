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

struct NewFileResult{
    NewFileResult(FileType type, QString path):
        type(type), path(path){}

    FileType type;
    QString path;
};

class NewFileDialog : public QDialog{
    Q_OBJECT

public:
    explicit NewFileDialog(QWidget *parent = nullptr, QString workdir = "");
    ~NewFileDialog();

    NewFileResult getResult();
private:
    Ui::NewFileDialog *ui;

    QString workDir;

    bool fileValid = false;
    FileType type = FileType::none;

    QString constructFilePath();

private slots:
    void validate();
    void typeChosen(QListWidgetItem* item);
    void leave();
};

#endif // NEWFILEDIALOG_H
