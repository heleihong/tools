#include "lineeditdelegate.h"

#include <QDebug>
#include <QDateTime>

LineEditDelegate::LineEditDelegate(QObject *parent)
    : QItemDelegate(parent)
{

}

LineEditDelegate::~LineEditDelegate()
{

}

QWidget *LineEditDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &/*index*/) const
{
    QLineEdit *editor = new QLineEdit(parent);
    return editor;
}

void LineEditDelegate::setEditorData(QWidget *editor,
    const QModelIndex &index) const
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
    QString value = index.data().toString();
    lineEdit->setText(value);
}

void LineEditDelegate::setModelData(QWidget *editor,
    QAbstractItemModel *model, const QModelIndex &index) const
{
    // get editor
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

    // get old val form the model
    QString oldVal = model->data(index).toString();

    // get new val from editor
    QString newVal = lineEdit->text();

    // get index of type item
    QModelIndex typeIndex = model->index(index.row(), 1, index.parent());

    // get type val
    QString type = model->data(typeIndex).toString();

    if (newVal != oldVal && 
        checkValueValidity(newVal, type, index.column()))
    {
        // set data
        emit dataChanged(index, newVal);
    }
}

void LineEditDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

bool LineEditDelegate::checkValueValidity(const QString& value, const QString &type, int col)
{
    bool ok = false;
    if (col == 0) 
    {
        ok = true;
    }
    else if (col == 2)
    {
        if (type == QLatin1String("boolean"))
        {
            if (value == QLatin1String("true") ||
                value == QLatin1String("false"))
            {
                ok = true;
            }
        }
        else if (type == QLatin1String("integer"))
        {
            value.toLongLong(&ok);
        }
        else if (type == QLatin1String("real"))
        {
            value.toDouble(&ok);
        }
        else if (type == QLatin1String("string"))
        {
            ok = true;
        }
        else if (type == QLatin1String("date"))
        {
            ok = QDateTime::fromString(value, Qt::ISODate).isValid();
        }
        else
        {
            qWarning() << "type(" << type << ") is invalid or not support changes.";
        }
    }
    else
    {
        qWarning() << "column(" << col << ") is invalid.";
    }

    return ok;
}