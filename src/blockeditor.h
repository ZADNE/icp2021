/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef BLOCKEDITOR_H
#define BLOCKEDITOR_H

#include <QWidget>
#include <QFileInfo>


///
/// \brief Abstract block editor
///
class BlockEditor: public QWidget{
    Q_OBJECT
public:
    explicit BlockEditor(QWidget *parent = nullptr);
    virtual ~BlockEditor();

    static void setLibPath(QString libPath);

    void setFilePath(QString relPath, bool loadFile);
    QString filePath() const;

    bool hasUnsavedChanges() { return m_unsavedChanges; }

signals:
    void havingUnsavedChanges(BlockEditor* editor);
    void withoutUnsavedChanges(BlockEditor* editor);
    void editBlock(QString path);//Request to edit other block

public slots:
    virtual void saveWork() final;
    virtual void buildWork() final;
    virtual void editedWork() final;

protected:
    virtual void load() = 0;
    virtual void save() = 0;
    virtual void build() = 0;

private:
    QString m_relPath;

    bool m_unsavedChanges = false;

    static QString m_libPath;
};

#endif // BLOCKEDITOR_H
