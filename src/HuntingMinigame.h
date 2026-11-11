#pragma once
#include <QDialog>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

class HuntCanvas;

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
    HuntCanvas *m_canvas = nullptr;
    int m_timeLeft = 25;
    int m_food = 0;
    int m_bulletsUsed = 0;
    int m_animalX = 50;       // 0-100
    int m_animalType = 0;     // 0=deer, 1=buffalo, 2=rabbit
    bool m_animalVisible = false;
    int m_flash = 0;          // hit/miss flash frames
    bool m_lastHit = false;
};
