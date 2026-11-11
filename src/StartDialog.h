#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>

class StartDialog : public QDialog {
    Q_OBJECT
public:
    explicit StartDialog(QWidget *parent = nullptr);

    QString leaderName() const;
    int occupation() const; // 0 Banker, 1 Carpenter, 2 Farmer
    QStringList partyNames() const;

private:
    QLineEdit *m_leader = nullptr;
    QComboBox *m_occ = nullptr;
    QLineEdit *m_p2 = nullptr;
    QLineEdit *m_p3 = nullptr;
    QLineEdit *m_p4 = nullptr;
};
