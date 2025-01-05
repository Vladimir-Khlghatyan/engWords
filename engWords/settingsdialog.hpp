#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <string>
#include <vector>
#include <QDialog>
#include <QString>

class QPushButton;
class QLabel;

class SettingsDialog : public QDialog
{
    using baseClass = QDialog;
public:
    SettingsDialog(bool& isEngArmMode, QWidget* parent);
    ~SettingsDialog(){}

private slots:
    void onToggle();
    void onCencel();
    void onSave();

private:
    QLabel*      m_toggleText;
    QPushButton* m_toggleBtn;
    QPushButton* m_cancelBtn;
    QPushButton* m_saveBtn;
    bool         m_isEngArmMode;
    bool&        m_parentMode;
};

#endif // SETTINGSDIALOG_HPP
