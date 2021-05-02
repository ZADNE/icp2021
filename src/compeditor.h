/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef COMPEDITOR_H
#define COMPEDITOR_H

#include "blockeditor.h"

struct CompSpec;

namespace Ui {
class CompEditor;
}

///
/// \brief Composite block editor
///
class CompEditor: public BlockEditor{
    Q_OBJECT

public:
    explicit CompEditor(QWidget *parent = nullptr);
    ~CompEditor() override;

protected:
    void load() override;
    void save() override;
    void build() override;

private:
    Ui::CompEditor *ui;
};

#endif // COMPEDITOR_H
