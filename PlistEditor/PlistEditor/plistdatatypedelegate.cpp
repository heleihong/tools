#include "PlistDataTypeDelegate.h"

PlistDataTypeDelegate::PlistDataTypeDelegate(QObject * parent) 
    : QItemDelegate(parent) 
{
	
}

PlistDataTypeDelegate::~PlistDataTypeDelegate() 
{
	
}

QWidget * PlistDataTypeDelegate::createEditor(QWidget *parent, 
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    QStringList list;
    list << "array" << "boolean" << "data" << "date" 
        << "dict" << "integer" << "real" << "string";
    editor->insertItems(0, list);

    return editor;
}

void PlistDataTypeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = index.data().toString();
    int n = comboBox->findText(value);
    comboBox->setCurrentIndex(n);
}

void PlistDataTypeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, 
    const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString val = comboBox->currentText();

    // model->setData(index, val, Qt::EditRole);
    emit PlistDataTypeDelegate::dataChanged(QModelIndex(index), val);
}

void PlistDataTypeDelegate::updateEditorGeometry(QWidget *editor, 
    const QStyleOptionViewItem &option, const QModelIndex &index) const 
{
    editor->setGeometry(option.rect);
}