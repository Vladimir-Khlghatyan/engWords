#ifndef SPLITDIALOG_HPP
#define SPLITDIALOG_HPP

#include <QDialog>
#include <QString>

class QPushButton;

class SplitDialog : public QDialog
{
    using baseClass = QDialog;
public:
    SplitDialog(const QString& path = "", QWidget* parent = nullptr);
    ~SplitDialog(){}

private slots:
    void onBrows();
    void onCencel();
    void onSplit();

private:
    QPushButton* m_browsBtl;
    QPushButton* m_cancelBtl;
    QPushButton* m_splitBtl;
    QString      m_path;
    QString      m_filePath;

};

#endif // SPLITDIALOG_HPP
