#include "StartDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>

StartDialog::StartDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Start Your Journey");
    setMinimumWidth(420);

    auto *layout = new QVBoxLayout(this);
    auto *form = new QFormLayout;

    m_leader = new QLineEdit("Wagon Leader");
    m_occ = new QComboBox;
    m_occ->addItems({"Banker ($1600)", "Carpenter ($800)", "Farmer ($400)"});
    m_occ->setCurrentIndex(1);

    m_p2 = new QLineEdit("Partner");
    m_p3 = new QLineEdit("Child");
    m_p4 = new QLineEdit("Child");

    form->addRow("Your name:", m_leader);
    form->addRow("Occupation:", m_occ);
    form->addRow("Party member 2:", m_p2);
    form->addRow("Party member 3:", m_p3);
    form->addRow("Party member 4:", m_p4);

    layout->addLayout(form);

    auto *btns = new QHBoxLayout;
    auto *ok = new QPushButton("Start Trail");
    auto *cancel = new QPushButton("Cancel");
    ok->setMinimumHeight(50);
    cancel->setMinimumHeight(50);
    btns->addWidget(ok);
    btns->addWidget(cancel);
    layout->addLayout(btns);

    connect(ok, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, this, &QDialog::reject);
}

QString StartDialog::leaderName() const { return m_leader->text().trimmed().isEmpty() ? "Leader" : m_leader->text().trimmed(); }
int StartDialog::occupation() const { return m_occ->currentIndex(); }
QStringList StartDialog::partyNames() const {
    return {leaderName(), m_p2->text().trimmed().isEmpty() ? "Partner" : m_p2->text().trimmed(),
            m_p3->text().trimmed().isEmpty() ? "Child" : m_p3->text().trimmed(),
            m_p4->text().trimmed().isEmpty() ? "Child" : m_p4->text().trimmed()};
}
