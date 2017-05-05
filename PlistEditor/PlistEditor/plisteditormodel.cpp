#include "plisteditormodel.h"

//////////////////////////////////////////////////////////////////////////
PListEditorModelItem *PlistEditorModel::itemForIndex(const QModelIndex &index)
{
    return static_cast<PListEditorModelItem*>(index.internalPointer());
}

PlistEditorModel::PlistEditorModel(QObject * parent)
    : QAbstractItemModel(parent) 
{
    m_rootItem = new PListEditorModelItem();
}

PlistEditorModel::~PlistEditorModel() 
{
    delete m_rootItem;
}

PListEditorModelItem *PlistEditorModel::getRoot() const
{
    return m_rootItem;
}

QModelIndex PlistEditorModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    PListEditorModelItem *parentItem = Q_NULLPTR;

    if (!parent.isValid())
        parentItem = getRoot();
    else
        parentItem = itemForIndex(parent);

    PListEditorModelItem *childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex PlistEditorModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    PListEditorModelItem *childItem = itemForIndex(child);
    PListEditorModelItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == getRoot())
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int PlistEditorModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    PListEditorModelItem *parentItem = Q_NULLPTR;

    if (!parent.isValid())
        parentItem = getRoot();
    else
        parentItem = itemForIndex(parent);

    return parentItem->childCount();
}

int PlistEditorModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 3;
}

QVariant PlistEditorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    PListEditorModelItem *item = itemForIndex(index);

    switch (index.column()) 
    {
    case 0:
        return item->name();
    case 1:
        return item->type();
    case 2:
        return item->value();
    default:
        return QVariant();
    }
}

bool PlistEditorModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (!index.isValid())
        return false;

    PListEditorModelItem *item = itemForIndex(index);

    QString str = value.toString();
    switch (index.column())
    {
    case 0:
        item->setName(str);
        break;
    case 1:
        item->setType(str);
        break;
    case 2:
        item->setValue(str);
        break;
    default:
        break;
    };

    emit dataChanged(index, index);
    return true;
}

QVariant PlistEditorModel::headerData(int section, Qt::Orientation orientation, int role) const 
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) 
    {
        switch (section) 
        {
        case 0:
            return tr("Key");
        case 1:
            return tr("Type");
        case 2:
            return tr("Value");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

Qt::ItemFlags PlistEditorModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    int column = index.column();
    switch (column)
    {
    case 1:
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    case 0:
    case 2:
    {
        PListEditorModelItem *item = itemForIndex(index);
        if (column == 0)
        {
            if (item->parent() == m_rootItem)
                return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
            else
                return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
        }
        else
        {
            QString type = item->type();
            if (type == QLatin1String("dict") ||
                type == QLatin1String("array"))
            {
                return Qt::ItemIsSelectable;
            }
            else {
                return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
            }
        }
        break;
    }
    default:
        break;
    }
    return __super::flags(index);
}