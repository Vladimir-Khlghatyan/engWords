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
    SettingsDialog(bool& isEngArmMode, bool& showPossAnsImmediately, QWidget* parent);
    ~SettingsDialog(){}

private slots:
    void onSwap();
    void onAutoSlie();
    void onCencel();
    void onSave();

private:
    QLabel*      m_swapText;
    QLabel*      m_autoSlieText;
    QPushButton* m_autoSlieBtn;
    QPushButton* m_swapBtn;
    QPushButton* m_cancelBtn;
    QPushButton* m_saveBtn;
    bool         m_isEngArmMode;
    bool&        m_parentEngArmMode;
    bool         m_autoSlide;
    bool&        m_parentAutoSlide;
};

#endif // SETTINGSDIALOG_HPP
