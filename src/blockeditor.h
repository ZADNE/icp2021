/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#ifndef BLOCKEDITOR_H
#define BLOCKEDITOR_H

#include <QWidget>
#include <QFileInfo>

class BlockEditor: public QWidget{
    Q_OBJECT
public:
    explicit BlockEditor(QWidget *parent = nullptr);
    virtual ~BlockEditor();


    void setFilePath(QString filePath, bool loadFile);
    QString filePath() const;

    bool hasUnsavedChanges() { return m_unsavedChanges; }

signals:
    void unsavedChanges();
    void savedChanges();

public slots:
    virtual void saveWork() final;
    virtual void compileWork() final;
    virtual void editedWork() final;

protected:
    virtual void load() = 0;
    virtual void save() = 0;
    virtual void compile() = 0;

private:
    QFileInfo m_fileInfo;

    bool m_unsavedChanges = false;
};

#endif // BLOCKEDITOR_H
