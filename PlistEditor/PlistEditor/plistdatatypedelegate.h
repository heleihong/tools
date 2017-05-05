#ifndef PLIST_EDITOR_PLIST_DATA_DELEGATE_H_
#define PLIST_EDITOR_PLIST_DATA_DELEGATE_H_

#include <QItemDelegate>
#include <QComboBox>

class PlistDataTypeDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    PlistDataTypeDelegate(QObject * parent = Q_NULLPTR);
	~PlistDataTypeDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
        const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const Q_DECL_OVERRIDE;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

signals:
    void dataChanged(const QModelIndex&, QString) const;
};

#endif // PLIST_EDITOR_PLIST_DATA_DELEGATE_H_