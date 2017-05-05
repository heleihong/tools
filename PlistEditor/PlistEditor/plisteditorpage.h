#ifndef PLIST_EDITOR_PLIST_EDITOR_PAGE_H_
#define PLIST_EDITOR_PLIST_EDITOR_PAGE_H_

#include <QFileInfo>
#include <QTreeView>
#include <QUndoStack>
#include <QWidget>
#include <QResizeEvent>

#include "PlistEditorModel.h"

class PlistEditorPage : public QWidget
{
    Q_OBJECT

public:
    PlistEditorPage(PlistEditorModel *model, QWidget *parent = Q_NULLPTR);
    ~PlistEditorPage();

    QString path();
    void setPath(const QString& path);
    QString getFileName();

    PlistEditorModel *model();
    void setModel(PlistEditorModel *m);
    void clearModel();

    QUndoStack *getUndoStack();

protected:
    void initTreeView();

public Q_SLOTS:
    void on_treeView_expanded();
    void on_treeView_collapsed();

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    PlistEditorModel *m_model;
    QFileInfo m_fileInfo;
    QTreeView *m_treeView;
    QUndoStack *m_undoStack;
    bool m_treeExpanded;
};

#endif // PLIST_EDITOR_PLIST_EDITOR_PAGE_H_