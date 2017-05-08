#include "commands.h"

EditCommand::EditCommand(QString val, PlistEditorModel *model,
    const QModelIndex &index, QUndoCommand *parent)
{
    m_model = model;
    m_index = index;
    m_oldVal = model->data(index, Qt::DisplayRole).toString();
    m_newVal = val;

    QString subject;
    switch (index.column())
    {
    case 0:
        subject = QObject::tr("key");
        break;
    case 1:
        subject = QObject::tr("type");
        break;
    case 2:
        subject = QObject::tr("value");
        break;
    };

    setText(QObject::tr("Edit item %1").arg(subject));
}

void EditCommand::undo()
{
    m_model->setData(m_index, m_oldVal);
}

void EditCommand::redo()
{
    m_model->setData(m_index, m_newVal);
}