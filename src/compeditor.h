#ifndef COMPEDITOR_H
#define COMPEDITOR_H

#include <QWidget>

namespace Ui {
class CompEditor;
}

class CompEditor : public QWidget{
    Q_OBJECT

public:
    explicit CompEditor(QWidget *parent = nullptr);
    ~CompEditor();

public slots:
    void saveWork();

private:
    Ui::CompEditor *ui;
};

#endif // COMPEDITOR_H
