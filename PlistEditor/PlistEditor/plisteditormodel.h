#ifndef PLIST_EDITOR_PLIST_EDITOR_MODEL_H_
#define PLIST_EDITOR_PLIST_EDITOR_MODEL_H_

#include <QAbstractItemModel>
#include <QtXml/QDomDocument>

#include "PListEditorModelItem.h"

class PlistEditorModel : public QAbstractItemModel 
{
	Q_OBJECT

public:
    explicit PlistEditorModel(QObject *parent = Q_NULLPTR);
    ~PlistEditorModel();

    PListEditorModelItem *getRoot() const;

    QModelIndex index(int row, int column,
        const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE;
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    QVariant headerData(int section, Qt::Orientation orientation,
        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

signals:
    void itemAdded(const QModelIndex &index);

protected:
    static PListEditorModelItem *itemForIndex(const QModelIndex &index);

private:
    PListEditorModelItem *m_rootItem;
};

#endif // PLIST_EDITOR_PLIST_EDITOR_MODEL_H_