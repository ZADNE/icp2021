/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#ifndef ATOMEDITOR_H
#define ATOMEDITOR_H

#include <QWidget>
#include <QTreeWidgetItem>

#include "blockeditor.h"

namespace Ui {
class AtomEditor;
}

class AtomEditor: public BlockEditor{
    Q_OBJECT

public:
    explicit AtomEditor(QWidget *parent = nullptr);
    ~AtomEditor() override;

protected:
    void load() override;
    void save() override;
    void build() override;

private:
    Ui::AtomEditor *ui;
};

#endif // ATOMEDITOR_H
