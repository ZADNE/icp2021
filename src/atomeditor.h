/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#ifndef ATOMEDITOR_H
#define ATOMEDITOR_H

#include <QWidget>

#include "blockeditor.h"

namespace Ui {
class AtomEditor;
}

class AtomEditor: public BlockEditor{
    Q_OBJECT

public:
    explicit AtomEditor(QWidget *parent = nullptr);
    ~AtomEditor();

signals:
    void unsavedChange();
    void saved();

public slots:
    void saveWork();


private:
    Ui::AtomEditor *ui;
};

#endif // ATOMEDITOR_H
