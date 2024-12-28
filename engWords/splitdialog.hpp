#ifndef SPLITDIALOG_HPP
#define SPLITDIALOG_HPP

#include <string>
#include <QDialog>
#include <QString>

class QPushButton;
class QLabel;

class SplitDialog : public QDialog
{
    using baseClass = QDialog;
public:
    SplitDialog(const QString& path = "", QWidget* parent = nullptr);
    ~SplitDialog(){}

private:
    void parseTheFile();
    std::pair<std::string, std::string> getWordsPair(const std::string& str);

private slots:
    void onBrows();
    void onCencel();
    void onSplit();

private:
    QLabel*      m_text;
    QPushButton* m_browsBtl;
    QPushButton* m_cancelBtl;
    QPushButton* m_splitBtl;
    QString      m_path;
    QString      m_filePath;
    std::string  m_errorText;
};

#endif // SPLITDIALOG_HPP
