#include "songlistdialog.h"

#include "songdialog.h"
#include "ui_songlistdialog.h"

SongListDialog::SongListDialog(QWidget *parent): QDialog(parent)
{
	m_ui = new Ui::SongListDialog();
	m_ui->setupUi(this);

    counter_name_ = 0;

    CreateSongTable(); // Инициализация таблицы
    LoadData(); // Загрузка данных из файла
}

SongListDialog::~SongListDialog()
{
    delete m_ui;
}

// Инициализация таблицы
void SongListDialog::CreateSongTable()
{
    connect(m_ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(clicked_on_table_slot(QModelIndex)));

    model = new QStandardItemModel;
    m_header_column_ << "Превью альбома" << "Название" << "Исполнитель" << "Продолжительность" << "Рейтинг";
    model->setHorizontalHeaderLabels(m_header_column_);

    m_ui->tableView->setModel(model);
    m_ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_ui->tableView->setColumnHidden(4, true);
}

// Клик по кнопке "Создать"
void SongListDialog::on_btnCreate_clicked()
{
    SongDialog *dlg = new SongDialog(counter_name_);
    if (dlg->exec() == QDialog::Accepted)
    {
        counter_name_++;

        DataOfSong data;
        data.name = dlg->GetName();
        data.executor = dlg->GetExecutor();
        data.duration = dlg->GetDuration();
        data.rating = dlg->GetRating();
        data.path = dlg->GetPath();

        songs_vector_.push_back(data);

        AddRow(m_ui->tableView->model()->rowCount());
    }

    delete dlg;
}

// Добавление строки
void SongListDialog::AddRow(int _new_row_index)
{
    if(!(m_ui->tableView->model() == nullptr))
    {
        m_ui->tableView->model()->insertRow(_new_row_index);

        if (!songs_vector_[_new_row_index].path.isEmpty())
        {
            auto fullImagePath = "./images/" + songs_vector_[_new_row_index].path;
            if (QFile::exists(fullImagePath))
            {
                QLabel *imageLabel = new QLabel();
                imageLabel->setPixmap(fullImagePath);
                m_ui->tableView->setIndexWidget(m_ui->tableView->model()->index(_new_row_index,0), imageLabel);
                imageLabel->setToolTip(QString::number(songs_vector_[_new_row_index].rating));
            }
        }

        item = new QStandardItem(songs_vector_[_new_row_index].name);
        model->setItem(_new_row_index, 1, item);
        item->setToolTip(QString::number(songs_vector_[_new_row_index].rating));
        item = new QStandardItem(songs_vector_[_new_row_index].executor);
        model->setItem(_new_row_index, 2, item);
        item->setToolTip(QString::number(songs_vector_[_new_row_index].rating));
        item = new QStandardItem(songs_vector_[_new_row_index].duration);
        model->setItem(_new_row_index, 3, item);
        item->setToolTip(QString::number(songs_vector_[_new_row_index].rating));
        item = new QStandardItem(QString::number(songs_vector_[_new_row_index].rating));
        model->setItem(_new_row_index, 4, item);
    }
}

void SongListDialog::on_edit_composition_button_clicked()
{
    if(songs_vector_.isEmpty())
    {
        return;
    }
    SongDialog *dlg = new SongDialog(last_click_on_table_row_,
                                     songs_vector_[last_click_on_table_row_].path,
                                     songs_vector_[last_click_on_table_row_].name,
                                     songs_vector_[last_click_on_table_row_].executor,
                                     songs_vector_[last_click_on_table_row_].duration,
                                     songs_vector_[last_click_on_table_row_].rating,
                                     true);
    if (dlg->exec() == QDialog::Accepted)
    {
        m_ui->tableView->model()->removeRow(last_click_on_table_row_);
        songs_vector_.removeAt(last_click_on_table_row_);

        DataOfSong data;
        data.name = dlg->GetName();
        data.executor = dlg->GetExecutor();
        data.duration = dlg->GetDuration();
        data.rating = dlg->GetRating();
        data.path = dlg->GetPath();

        songs_vector_.insert(last_click_on_table_row_, data);

        AddRow(last_click_on_table_row_);
    }
    delete dlg;
}

void SongListDialog::on_save_button_clicked()
{
    QString path_save = "./song_data.json";

    QJsonArray json_objects;
    for(int i = 0; i < counter_name_; i++)
    {
        QJsonObject obj_coord;
        obj_coord.insert("Name", QJsonValue::fromVariant(songs_vector_[i].name));
        obj_coord.insert("Executor", QJsonValue::fromVariant(songs_vector_[i].executor));
        obj_coord.insert("Duration", QJsonValue::fromVariant(songs_vector_[i].duration));
        obj_coord.insert("Rating", QJsonValue::fromVariant(songs_vector_[i].rating));
        obj_coord.insert("Path", QJsonValue::fromVariant(songs_vector_[i].path));
        json_objects.append(obj_coord);
    }

    QJsonDocument doc(json_objects);
    QString jsonString = doc.toJson(QJsonDocument::Indented);

    QFile save_file(path_save);

    QTextStream save_stream(&save_file);
    save_file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream( &save_file );
    stream << jsonString;
    save_file.close();
}

void SongListDialog::LoadData()
{
    QString load_path = "./song_data.json";

    QFile load_file(load_path);
    if (!load_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Не удалось открыть файл для загрузки";
        return;
    }

    QString bufer_string = load_file.readAll();
    load_file.close();

    QJsonArray json_objects;
    QJsonDocument doc = QJsonDocument::fromJson(bufer_string.toUtf8());
    if(doc.isArray())
    {
        json_objects = doc.array();

        for(int i = 0; i < json_objects.size(); i++)
        {
            //coordinate.append(QPointF(json_objects[i].toObject().value("Latitude").toDouble(), json_objects[i].toObject().value("Longitude").toDouble()));
            DataOfSong data;
            data.name = json_objects[i].toObject().value("Name").toString();
            data.duration = json_objects[i].toObject().value("Duration").toString();
            data.executor = json_objects[i].toObject().value("Executor").toString();
            data.rating = json_objects[i].toObject().value("Rating").toInt(0);
            data.path = json_objects[i].toObject().value("Path").toString();

            songs_vector_.append(data);
            counter_name_++;

            AddRow(i);
        }
    }
}

void SongListDialog::clicked_on_table_slot(const QModelIndex &index)
{
    last_click_on_table_row_ = index.row();
}

void SongListDialog::on_sort_pushButton_clicked()
{
    std::sort(songs_vector_.begin(), songs_vector_.end());
    m_ui->tableView->model()->removeRows(0, m_ui->tableView->model()->rowCount());
    for(int i = 0; i < songs_vector_.size(); i++)
    {
        AddRow(i);
    }
}

