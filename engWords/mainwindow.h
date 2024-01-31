#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QPushButton>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString     getExecutableGrandparentDirPath(void);
    QStringList readFromFile(const QString& filepath);
    int         getRandomNumber(int lower, int upper);
    void        writeToFileInAppendMode(const QString& filePath, const QStringList& source);
    void        writeToFileInTranscateMode(const QString& filePath, const QStringList& source);
    void        parse(QStringList& source);
    void        newWordButtonPushAction(void);
    void        clearButtonPushAction(void);
    void        deleteButtonPushAction(void);
    void        buttonPushAction(int index);

private:
    Ui::MainWindow          *ui;
    QVector<QPushButton*>    _button;
    QStringList             _engWords;
    QStringList             _armWords;
    QString                 _filePath;
    int                     _correctIndex;
    int                     _totalAnswers;
    int                     _rightAnswers;
    int                     _deletedWords;
    int                     _currentWordIndex;

};
#endif // MAINWINDOW_H
