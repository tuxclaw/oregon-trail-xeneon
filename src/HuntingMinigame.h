#pragma once
#include <QDialog>
#include <QTimer>
#include <QLabel>
#include <QPushButton>

class HuntingMinigame : public QDialog {
    Q_OBJECT
public:
    explicit HuntingMinigame(QWidget *parent = nullptr);
    int foodGained() const { return m_food; }
    int bulletsUsed() const { return m_bulletsUsed; }

private slots:
    void onTick();
    void onShoot();

private:
    QTimer m_timer;
    QLabel *m_status = nullptr;
    QPushButton *m_shoot = nullptr;
    int m_timeLeft = 20; // seconds
    int m_food = 0;
    int m_bulletsUsed = 0;
    int m_target = 0; // 0-100 position
    bool m_visible = false;
};
