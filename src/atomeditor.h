#ifndef ATOMEDITOR_H
#define ATOMEDITOR_H

#include <QWidget>

namespace Ui {
class AtomEditor;
}

class AtomEditor : public QWidget{
    Q_OBJECT

public:
    explicit AtomEditor(QWidget *parent = nullptr);
    ~AtomEditor();

public slots:
    void saveWork();


private:
    Ui::AtomEditor *ui;
};

#endif // ATOMEDITOR_H
