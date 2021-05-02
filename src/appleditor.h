/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef APPLEDITOR_H
#define APPLEDITOR_H

#include "blockeditor.h"

namespace Ui {
class ApplEditor;
}

///
/// \brief Application editor
///
class ApplEditor: public BlockEditor{
    Q_OBJECT

public:
    explicit ApplEditor(QWidget *parent = nullptr);
    ~ApplEditor() override;

protected:
    void load() override;
    void save() override;
    void build() override;

private:
    Ui::ApplEditor *ui;
};

#endif // APPLEDITOR_H
