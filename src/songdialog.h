#pragma once
#include <QDialog>
#include <QDebug>

namespace Ui 
{	
class SongDialog;
}

class SongDialog : public QDialog
{
	Q_OBJECT
public:
    SongDialog(int _counter_name,
               QString _path = "",
               QString _name = "",
               QString _executor = "",
               QString _duration = "",
               int _rating = 0,
               bool edit_copmosition = false,
               QWidget* parent = nullptr);
	~SongDialog();

    QString GetName();     // Получение названия композиции
    QString GetExecutor(); // Получение исполнителя композиции
    QString GetDuration(); // Получение продолжительности композиции
    int GetRating();       // Получение рейтинга композиции
    QString GetPath();     // Получение пути до превью альбома
    void DialogForEdit(QString _counter_name, // Настройка диалогового окна для редактирования данных о композиции
                       QString _name,
                       QString _executor,
                       QString _duration,
                       int _rating);

private:
	void choosePreview();
	QString copiedPreview(const QString& filePath) const;
	void showPreview(const QString& relativePath) const;
	QString fullImagePath(const QString& relativePath) const;
	
    Ui::SongDialog* m_ui = nullptr;
    const QString m_imageDir = "./images";

    QString path_;     // Путь до превью
    int counter_name_; // Счетчик композиций для задания порядкового номера превью альбома
};

