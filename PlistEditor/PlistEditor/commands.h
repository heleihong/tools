#ifndef PLIST_EDITOR_COMMANDS_H_
#define PLIST_EDITOR_COMMANDS_H_

#include <QUndoCommand>

#include "plisteditormodel.h"

class EditCommand : public QUndoCommand
{
public:
    EditCommand(QString val, PlistEditorModel *model, 
        const QModelIndex &index, QUndoCommand *parent = Q_NULLPTR);

    void undo();
    void redo();

private:
    PlistEditorModel *m_model;
    QModelIndex m_index;

    QString m_oldVal;
    QString m_newVal;
};

#endif // PLIST_EDITOR_COMMANDS_H_