/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#ifndef COMPEDITOR_H
#define COMPEDITOR_H

#include <QWidget>

#include "blockeditor.h"

namespace Ui {
class CompEditor;
}

class CompEditor: public BlockEditor{
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
