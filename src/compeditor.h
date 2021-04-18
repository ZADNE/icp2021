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
    ~CompEditor() override;

protected:
    void load() override;
    void save() override;
    void compile() override;

private:
    Ui::CompEditor *ui;
};

#endif // COMPEDITOR_H
