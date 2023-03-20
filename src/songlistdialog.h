#pragma once

#include <QDialog>
#include <QTableView>
#include <QTableWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVector>
#include <QLabel>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <algorithm>

namespace Ui 
{
    class SongListDialog;
}

struct DataOfSong
{
    QString name;
    QString executor;
    QString duration;
    int rating;
    QString path;

    // Перегрузка оператора сравнения для сортировки по рейтингу
    bool operator<(const DataOfSong& other) const
    {
        return rating > other.rating;
    }
};

class SongListDialog : public QDialog 
{
    Q_OBJECT
public:
	SongListDialog(QWidget *parent = nullptr);
	~SongListDialog();

    void CreateSongTable(); // Инициализация таблицы
    void AddRow(int _new_row_index); // Добавление строки
    void LoadData(); // Загрузка сохраненных данных

public slots:
    void on_btnCreate_clicked(); // Слот кнопки "Создать"
    void on_edit_composition_button_clicked(); // Слот кнопки "Редактировать"
    void on_save_button_clicked(); // Слот кнопки "Сохранить"
    void clicked_on_table_slot(const QModelIndex &index); // Слот для сохранения индекса строки, по которой был клик
    void on_sort_pushButton_clicked(); // Слот кнопки "Сортировать"
private:
	Ui::SongListDialog* m_ui = nullptr;

    int counter_name_; // Счётчик для названия картинок
    QStringList m_header_column_; // Заголовки столбцов
    QStandardItemModel *model;
    QStandardItem *item;
    QVector<DataOfSong> songs_vector_; // Контейнер для хранения добавленных композиций
    int last_click_on_table_row_; // Индекс строки последнего клика по таблице
};


