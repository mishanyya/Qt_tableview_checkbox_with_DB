#include "delegate.h"
#include "QCheckBox"
#include "QApplication"

CheckBoxDelegate::CheckBoxDelegate(QObject *parent)
    :QStyledItemDelegate (parent)
{

}

QWidget *CheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    //Cоздать checkbox editor
    QCheckBox *editor = new QCheckBox(parent);
    return editor;
}

void CheckBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //Установить выбрано/не выбрано
    QCheckBox *cb = qobject_cast<QCheckBox *>(editor);
    cb->setChecked(index.data().toBool());
}

void CheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //Записать данные в модель, если нажата пишет 1 иначе 0
    QCheckBox *cb = static_cast<QCheckBox *>(editor);
    int value = (cb->checkState()==Qt::Checked)? 1 : 0;
    model->setData(index, value, Qt::EditRole);
}

void CheckBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    QStyleOptionButton checkboxstyle;
    QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &checkboxstyle);

    //Центрировать
    checkboxstyle.rect = option.rect;
    checkboxstyle.rect.setLeft(option.rect.x() +
                               option.rect.width()/2 - checkbox_rect.width()/2);

    editor->setGeometry(checkboxstyle.rect);
}


void CheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //Получить данные
    bool data = index.model()->data(index, Qt::DisplayRole).toBool();

    //Создать стиль checkBox
    QStyleOptionButton checkboxstyle;
    QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &checkboxstyle);

    //Центрировать
    checkboxstyle.rect = option.rect;
    checkboxstyle.rect.setLeft(option.rect.x() +
                               option.rect.width()/2 - checkbox_rect.width()/2);
    //Выбрано или не выбрано
    if(data)
        checkboxstyle.state = QStyle::State_On|QStyle::State_Enabled;
    else
        checkboxstyle.state = QStyle::State_Off|QStyle::State_Enabled;

    //Готово! Отобразить
    QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkboxstyle, painter);
}

//createEditor() returns the widget used to change data from the model and can be reimplemented to customize editing behavior.
//setEditorData() provides the widget with data to manipulate.
//updateEditorGeometry() ensures that the editor is displayed correctly with respect to the item view.
//setModelData() returns updated data to the model.
