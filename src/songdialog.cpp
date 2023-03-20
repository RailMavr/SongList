#include "songdialog.h"
#include "ui_songdialog.h"
#include <QFileDialog>

SongDialog::SongDialog(int _counter_name,
                       QString _path,
                       QString _name,
                       QString _executor,
                       QString _duration,
                       int _rating,
                       bool edit_copmosition,
                       QWidget* parent): QDialog(parent), counter_name_(_counter_name),  m_ui(new Ui::SongDialog())
{
    m_ui->setupUi(this);
    connect(m_ui->btnPreview, &QPushButton::clicked, this, &SongDialog::choosePreview);
    QDir dir;
    if (!dir.exists(m_imageDir))
        dir.mkpath(m_imageDir);

    // Флаг открытия в режиме редактирования
    if(edit_copmosition)
    {
        DialogForEdit(_path,
                      _name,
                      _executor,
                      _duration,
                      _rating);
    }
}

SongDialog::~SongDialog()
{
    delete m_ui;
}

void SongDialog::choosePreview()
{
    const auto filename = QFileDialog::getOpenFileName(this, tr("Выберите изображение"),
                                                       QString(), "(*.jpeg *.jpg *png *gif)");
    if (filename.isEmpty())
        return;
    const QString copyName = copiedPreview(filename);
    path_ = copyName;
    showPreview(copyName);
}

QString SongDialog::copiedPreview(const QString& filePath) const
{
    const QImage image(filePath);
    const auto smaller = image.scaled(m_ui->labelPreview->width(), m_ui->labelPreview->height(),
                                      Qt::AspectRatioMode::KeepAspectRatioByExpanding);

    QString name = QString::number(counter_name_) +  ".png";
    if (!smaller.save(fullImagePath(name), "PNG"))
        return QString();

    return name;
}

void SongDialog::showPreview(const QString& relativePath) const
{
    if (relativePath.isEmpty())
        return;

    const auto fn = fullImagePath(relativePath);
    if (!QFile::exists(fn))
        return;

    const QPixmap pixmap(fn);
    m_ui->labelPreview->setPixmap(pixmap);
}


QString SongDialog::fullImagePath(const QString& relativePath) const
{
    QDir dir;
    dir.cd(m_imageDir);
    return dir.absoluteFilePath(relativePath);
}

QString SongDialog::GetName()
{
    return m_ui->name_lineEdit->text();
}

QString SongDialog::GetExecutor()
{
    return m_ui->executor_lineEdit->text();
}

QString SongDialog::GetDuration()
{
    return m_ui->duration_timeEdit->text();
}

int SongDialog::GetRating()
{
    return m_ui->rating_horizontalSlider->value();
}

QString SongDialog::GetPath()
{
    return path_;
}

// Настройка диалогового окна для редактирования данных о композиции
void SongDialog::DialogForEdit(QString _path, QString _name, QString _executor, QString _duration, int _rating)
{
    QString fullImagePath = m_imageDir + "/" + _path;
    path_ = _path;
    if (QFile::exists(fullImagePath))
    {
        const QPixmap pixmap(fullImagePath);
        m_ui->labelPreview->setPixmap(pixmap);
    }

    m_ui->name_lineEdit->setText(_name);
    m_ui->executor_lineEdit->setText(_executor);
    m_ui->duration_timeEdit->setTime(QTime::fromString(_duration));
    m_ui->rating_horizontalSlider->setValue(_rating);
}
