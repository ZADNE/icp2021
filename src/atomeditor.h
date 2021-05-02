/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef ATOMEDITOR_H
#define ATOMEDITOR_H

#include "blockeditor.h"

namespace Ui {
class AtomEditor;
}

///
/// \brief Atomic block editor
///
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
