#include "HuntingMinigame.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QRandomGenerator>
#include <QFont>

class HuntCanvas : public QWidget {
public:
    int animalX = 50;
    int animalType = 0;
    bool animalVisible = false;
    int flash = 0;
    bool lastHit = false;

    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        // Prairie background
        QLinearGradient sky(0, 0, 0, height());
        sky.setColorAt(0.0, QColor(135, 180, 220));
        sky.setColorAt(0.55, QColor(180, 200, 160));
        sky.setColorAt(1.0, QColor(90, 130, 60));
        p.fillRect(rect(), sky);

        // Ground
        p.setBrush(QColor(70, 100, 45));
        p.setPen(Qt::NoPen);
        p.drawRect(0, height() * 0.62, width(), height());

        // Distant hills
        p.setBrush(QColor(60, 90, 40, 180));
        QPainterPath hills;
        hills.moveTo(0, height() * 0.58);
        hills.quadTo(width() * 0.25, height() * 0.48, width() * 0.5, height() * 0.55);
        hills.quadTo(width() * 0.75, height() * 0.62, width(), height() * 0.50);
        hills.lineTo(width(), height());
        hills.lineTo(0, height());
        p.drawPath(hills);

        // Simple bushes
        p.setBrush(QColor(40, 70, 30));
        for (int i = 0; i < 5; ++i) {
            int bx = (i * width() / 5) + 40;
            int by = height() * 0.58;
            p.drawEllipse(bx, by, 50, 30);
        }

        // Flash overlay on hit/miss
        if (flash > 0) {
            QColor flashColor = lastHit ? QColor(255, 220, 100, 80) : QColor(200, 50, 50, 60);
            p.fillRect(rect(), flashColor);
        }

        // Animal
        if (animalVisible) {
            int x = width() * animalX / 100 - 30;
            int y = height() * 0.52;
            drawAnimal(p, x, y, animalType);
        }

        // Scope / crosshair
        int cx = width() / 2;
        int cy = height() / 2 + 10;
        p.setPen(QPen(QColor(255, 60, 60), 2));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(cx - 28, cy - 28, 56, 56);
        p.drawLine(cx - 40, cy, cx - 12, cy);
        p.drawLine(cx + 12, cy, cx + 40, cy);
        p.drawLine(cx, cy - 40, cx, cy - 12);
        p.drawLine(cx, cy + 12, cx, cy + 40);
        p.setBrush(QColor(255, 60, 60));
        p.drawEllipse(cx - 3, cy - 3, 6, 6);

        // Instructions
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 14, QFont::Bold));
        p.drawText(12, 28, "Aim the crosshair and SHOOT when the animal is centered!");
    }

private:
    void drawAnimal(QPainter &p, int x, int y, int type) {
        if (type == 0) { // Deer
            p.setBrush(QColor(139, 90, 43));
            p.setPen(Qt::NoPen);
            p.drawEllipse(x, y, 50, 28);           // body
            p.drawEllipse(x + 38, y - 18, 22, 22); // head
            p.setBrush(QColor(90, 50, 20));
            p.drawEllipse(x + 46, y - 22, 6, 10);  // ear
            // legs
            p.setPen(QPen(QColor(100, 60, 30), 3));
            p.drawLine(x + 10, y + 26, x + 8, y + 42);
            p.drawLine(x + 22, y + 26, x + 20, y + 42);
            p.drawLine(x + 32, y + 26, x + 34, y + 42);
            p.drawLine(x + 42, y + 26, x + 44, y + 42);
        } else if (type == 1) { // Buffalo
            p.setBrush(QColor(80, 50, 30));
            p.setPen(Qt::NoPen);
            p.drawEllipse(x - 10, y + 4, 80, 40);  // body
            p.drawEllipse(x + 50, y - 8, 30, 28);  // head
            p.setBrush(QColor(60, 40, 20));
            p.drawEllipse(x + 62, y - 4, 8, 8);    // horn hint
            p.setPen(QPen(QColor(60, 40, 20), 4));
            p.drawLine(x + 5, y + 42, x + 2, y + 58);
            p.drawLine(x + 20, y + 42, x + 18, y + 58);
            p.drawLine(x + 45, y + 42, x + 48, y + 58);
            p.drawLine(x + 60, y + 42, x + 64, y + 58);
        } else { // Rabbit
            p.setBrush(QColor(180, 160, 130));
            p.setPen(Qt::NoPen);
            p.drawEllipse(x + 10, y + 8, 30, 20);
            p.drawEllipse(x + 28, y - 4, 16, 16);
            p.setBrush(QColor(200, 180, 150));
            p.drawEllipse(x + 30, y - 12, 5, 12); // ear
            p.drawEllipse(x + 36, y - 12, 5, 12);
            p.setPen(QPen(QColor(150, 120, 90), 2));
            p.drawLine(x + 16, y + 26, x + 14, y + 36);
            p.drawLine(x + 26, y + 26, x + 28, y + 36);
        }
    }
};

HuntingMinigame::HuntingMinigame(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Hunting");
    resize(900, 520);
    setStyleSheet("background: #1a1a1a;");

    auto *layout = new QVBoxLayout(this);
    m_canvas = new HuntCanvas;
    m_canvas->setMinimumHeight(340);

    m_status = new QLabel("Time: 25s   Food: 0 lb   Bullets used: 0");
    m_status->setFont(QFont("Consolas", 16));
    m_status->setStyleSheet("color: #e0e0e0; padding: 6px;");

    m_shoot = new QPushButton("SHOOT");
    m_shoot->setMinimumHeight(72);
    m_shoot->setFont(QFont("Arial", 22, QFont::Bold));
    m_shoot->setStyleSheet(
        "QPushButton { background: #8B4513; color: white; border-radius: 12px; }"
        "QPushButton:hover { background: #A0522D; }"
        "QPushButton:pressed { background: #6B3410; }"
    );

    layout->addWidget(m_canvas);
    layout->addWidget(m_status);
    layout->addWidget(m_shoot);

    connect(&m_timer, &QTimer::timeout, this, &HuntingMinigame::onTick);
    connect(m_shoot, &QPushButton::clicked, this, &HuntingMinigame::onShoot);

    m_timer.start(400);
}

void HuntingMinigame::onTick()
{
    if (m_flash > 0) m_flash--;

    m_timeLeft--;
    if (m_timeLeft <= 0) {
        m_timer.stop();
        accept();
        return;
    }

    // Spawn / move animal
    if (QRandomGenerator::global()->bounded(100) < 28 || !m_animalVisible) {
        m_animalX = QRandomGenerator::global()->bounded(15, 85);
        m_animalType = QRandomGenerator::global()->bounded(100) < 60 ? 0 : (QRandomGenerator::global()->bounded(100) < 70 ? 1 : 2);
        m_animalVisible = true;
    } else if (QRandomGenerator::global()->bounded(100) < 20) {
        m_animalVisible = false;
    }

    m_canvas->animalX = m_animalX;
    m_canvas->animalType = m_animalType;
    m_canvas->animalVisible = m_animalVisible;
    m_canvas->flash = m_flash;
    m_canvas->lastHit = m_lastHit;
    m_canvas->update();

    m_status->setText(QString("Time: %1s   Food: %2 lb   Bullets used: %3")
                          .arg(m_timeLeft).arg(m_food).arg(m_bulletsUsed));
}

void HuntingMinigame::onShoot()
{
    m_bulletsUsed += 2;
    m_flash = 4;

    bool hit = m_animalVisible && qAbs(m_animalX - 50) < 14;
    m_lastHit = hit;

    if (hit) {
        int base = (m_animalType == 1) ? 70 : (m_animalType == 0 ? 45 : 25);
        int gain = base + QRandomGenerator::global()->bounded(40);
        m_food += gain;
        m_status->setText(QString("HIT! +%1 lb   Time: %2s   Food: %3")
                              .arg(gain).arg(m_timeLeft).arg(m_food));
        m_animalVisible = false; // animal flees
    } else {
        m_status->setText(QString("Miss!   Time: %1s   Food: %2 lb")
                              .arg(m_timeLeft).arg(m_food));
    }

    m_canvas->flash = m_flash;
    m_canvas->lastHit = m_lastHit;
    m_canvas->animalVisible = m_animalVisible;
    m_canvas->update();
}
