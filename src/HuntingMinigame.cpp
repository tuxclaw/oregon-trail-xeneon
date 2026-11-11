#include "HuntingMinigame.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QRandomGenerator>
#include <QFont>
#include <QPainterPath>

class HuntCanvas : public QWidget {
public:
    int animalX = 50;
    int animalType = 0;
    bool animalVisible = false;
    int flash = 0;
    bool lastHit = false;
    int muzzle = 0;
    int animFrame = 0;

    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        // Sky
        QLinearGradient sky(0, 0, 0, height());
        sky.setColorAt(0.0, QColor(100, 160, 210));
        sky.setColorAt(0.45, QColor(160, 190, 140));
        sky.setColorAt(1.0, QColor(75, 110, 50));
        p.fillRect(rect(), sky);

        // Ground
        p.setBrush(QColor(55, 85, 35));
        p.setPen(Qt::NoPen);
        p.drawRect(0, height() * 0.66, width(), height());

        // Hills
        p.setBrush(QColor(45, 75, 30, 160));
        QPainterPath hills;
        hills.moveTo(0, height() * 0.62);
        hills.quadTo(width() * 0.25, height() * 0.48, width() * 0.5, height() * 0.58);
        hills.quadTo(width() * 0.78, height() * 0.66, width(), height() * 0.50);
        hills.lineTo(width(), height());
        hills.lineTo(0, height());
        p.drawPath(hills);

        // Trees
        drawTree(p, 70, height() * 0.60, 1.15);
        drawTree(p, 200, height() * 0.62, 0.85);
        drawTree(p, width() - 120, height() * 0.59, 1.2);
        drawTree(p, width() - 260, height() * 0.63, 0.7);
        drawTree(p, 360, height() * 0.64, 0.55);

        // Bushes
        p.setBrush(QColor(30, 60, 22));
        for (int i = 0; i < 7; ++i) {
            int bx = 40 + i * (width() / 7);
            p.drawEllipse(bx, height() * 0.62, 48, 24);
        }

        if (flash > 0) {
            QColor c = lastHit ? QColor(255, 240, 140, 60) : QColor(160, 30, 30, 40);
            p.fillRect(rect(), c);
        }

        if (animalVisible) {
            int x = width() * animalX / 100 - 40;
            int y = height() * 0.56;
            drawAnimal(p, x, y, animalType, animFrame);
        }

        // Muzzle flash
        if (muzzle > 0) {
            int cx = width() / 2;
            int cy = height() - 20;
            p.setPen(Qt::NoPen);
            QRadialGradient g(cx, cy - 25, 50);
            g.setColorAt(0, QColor(255, 255, 200, 230));
            g.setColorAt(0.35, QColor(255, 190, 70, 180));
            g.setColorAt(1, QColor(255, 80, 0, 0));
            p.setBrush(g);
            p.drawEllipse(cx - 35, cy - 55, 70, 45);
        }

        // Crosshair
        int cx = width() / 2;
        int cy = height() / 2 + 6;
        p.setPen(QPen(QColor(255, 80, 80), 2.5));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(cx - 30, cy - 30, 60, 60);
        p.drawLine(cx - 46, cy, cx - 12, cy);
        p.drawLine(cx + 12, cy, cx + 46, cy);
        p.drawLine(cx, cy - 46, cx, cy - 12);
        p.drawLine(cx, cy + 12, cx, cy + 46);
        p.setBrush(QColor(255, 80, 80));
        p.drawEllipse(cx - 3, cy - 3, 6, 6);

        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 13, QFont::Bold));
        p.drawText(12, 26, "SHOOT when the animal is under the crosshair");
    }

private:
    void drawTree(QPainter &p, int x, int baseY, qreal scale) {
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(85, 50, 25));
        p.drawRoundedRect(x - 5 * scale, baseY, 10 * scale, 38 * scale, 2, 2);
        p.setBrush(QColor(35, 75, 28));
        p.drawEllipse(x - 26 * scale, baseY - 42 * scale, 52 * scale, 36 * scale);
        p.setBrush(QColor(45, 90, 32));
        p.drawEllipse(x - 20 * scale, baseY - 58 * scale, 40 * scale, 28 * scale);
        p.setBrush(QColor(55, 100, 38));
        p.drawEllipse(x - 14 * scale, baseY - 70 * scale, 28 * scale, 20 * scale);
    }

    void drawAnimal(QPainter &p, int x, int y, int type, int frame) {
        int leg = (frame % 6 < 3) ? 5 : -5;

        if (type == 0) { // Deer
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(165, 105, 50));
            p.drawEllipse(x + 5, y + 4, 58, 32);
            p.drawEllipse(x + 48, y - 16, 26, 26);
            p.setBrush(QColor(120, 70, 30));
            p.drawEllipse(x + 58, y - 24, 8, 14);
            // Antlers
            p.setPen(QPen(QColor(100, 55, 20), 2.5));
            p.drawLine(x + 58, y - 14, x + 50, y - 32);
            p.drawLine(x + 58, y - 14, x + 68, y - 30);
            p.drawLine(x + 50, y - 32, x + 46, y - 40);
            p.drawLine(x + 68, y - 30, x + 74, y - 38);
            // Legs
            p.setPen(QPen(QColor(120, 70, 30), 3.5));
            p.drawLine(x + 16, y + 34, x + 14 + leg, y + 52);
            p.drawLine(x + 28, y + 34, x + 26 - leg, y + 52);
            p.drawLine(x + 40, y + 34, x + 42 + leg, y + 52);
            p.drawLine(x + 50, y + 34, x + 52 - leg, y + 52);
        } else if (type == 1) { // Buffalo
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(70, 42, 25));
            p.drawEllipse(x - 5, y + 6, 90, 44);
            p.drawEllipse(x + 58, y - 8, 34, 32);
            p.setBrush(QColor(50, 30, 15));
            p.drawEllipse(x + 74, y - 4, 12, 12);
            p.setPen(QPen(QColor(50, 30, 15), 4.5));
            p.drawLine(x + 10, y + 48, x + 6 + leg, y + 66);
            p.drawLine(x + 26, y + 48, x + 24 - leg, y + 66);
            p.drawLine(x + 52, y + 48, x + 56 + leg, y + 66);
            p.drawLine(x + 68, y + 48, x + 72 - leg, y + 66);
        } else { // Rabbit
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(200, 175, 140));
            p.drawEllipse(x + 14, y + 10, 34, 24);
            p.drawEllipse(x + 34, y - 2, 20, 20);
            p.setBrush(QColor(220, 200, 170));
            p.drawEllipse(x + 36, y - 16, 6, 16);
            p.drawEllipse(x + 44, y - 16, 6, 16);
            p.setPen(QPen(QColor(170, 140, 100), 2.5));
            p.drawLine(x + 20, y + 32, x + 18 + leg, y + 44);
            p.drawLine(x + 32, y + 32, x + 34 - leg, y + 44);
        }
    }
};

HuntingMinigame::HuntingMinigame(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Hunting");
    resize(960, 560);
    setStyleSheet("background: #111;");

    auto *layout = new QVBoxLayout(this);
    m_canvas = new HuntCanvas;
    m_canvas->setMinimumHeight(380);

    m_status = new QLabel("Time: 25s   Food: 0 lb   Bullets: 0");
    m_status->setFont(QFont("Consolas", 16));
    m_status->setStyleSheet("color: #eee; padding: 8px;");

    m_shoot = new QPushButton("SHOOT");
    m_shoot->setMinimumHeight(76);
    m_shoot->setFont(QFont("Arial", 22, QFont::Bold));
    m_shoot->setStyleSheet(
        "QPushButton { background: #7A3E12; color: white; border-radius: 14px; }"
        "QPushButton:hover { background: #9A5E22; }"
        "QPushButton:pressed { background: #5A2E0A; }"
    );

    layout->addWidget(m_canvas);
    layout->addWidget(m_status);
    layout->addWidget(m_shoot);

    connect(&m_timer, &QTimer::timeout, this, &HuntingMinigame::onTick);
    connect(m_shoot, &QPushButton::clicked, this, &HuntingMinigame::onShoot);

    m_timer.start(120);
}

void HuntingMinigame::onTick()
{
    if (m_flash > 0) m_flash--;
    if (m_muzzle > 0) m_muzzle--;
    m_animFrame++;

    static int slow = 0;
    if (++slow >= 8) {
        slow = 0;
        m_timeLeft--;
        if (m_timeLeft <= 0) {
            m_timer.stop();
            accept();
            return;
        }
    }

    if (m_animalVisible) {
        m_animalX += m_animalDir * (1 + (m_animalType == 2 ? 2 : 0));
        if (m_animalX < 10 || m_animalX > 90) {
            m_animalDir *= -1;
            m_animalX = qBound(10, m_animalX, 90);
        }
        if (QRandomGenerator::global()->bounded(100) < 3)
            m_animalVisible = false;
    } else if (QRandomGenerator::global()->bounded(100) < 12) {
        m_animalX = QRandomGenerator::global()->bounded(20, 80);
        m_animalDir = QRandomGenerator::global()->bounded(2) ? 1 : -1;
        int r = QRandomGenerator::global()->bounded(100);
        m_animalType = r < 55 ? 0 : (r < 80 ? 1 : 2);
        m_animalVisible = true;
    }

    m_canvas->animalX = m_animalX;
    m_canvas->animalType = m_animalType;
    m_canvas->animalVisible = m_animalVisible;
    m_canvas->flash = m_flash;
    m_canvas->lastHit = m_lastHit;
    m_canvas->muzzle = m_muzzle;
    m_canvas->animFrame = m_animFrame;
    m_canvas->update();

    m_status->setText(QString("Time: %1s   Food: %2 lb   Bullets used: %3")
                          .arg(m_timeLeft).arg(m_food).arg(m_bulletsUsed));
}

void HuntingMinigame::onShoot()
{
    m_bulletsUsed += 1;
    m_muzzle = 5;
    m_flash = 5;

    bool hit = m_animalVisible && qAbs(m_animalX - 50) < 13;
    m_lastHit = hit;

    if (hit) {
        int base = (m_animalType == 1) ? 75 : (m_animalType == 0 ? 48 : 22);
        int gain = base + QRandomGenerator::global()->bounded(35);
        m_food += gain;
        m_status->setText(QString("HIT! +%1 lb   Time: %2s   Food: %3")
                              .arg(gain).arg(m_timeLeft).arg(m_food));
        m_animalVisible = false;
    } else {
        m_status->setText(QString("Miss!   Time: %1s   Food: %2 lb")
                              .arg(m_timeLeft).arg(m_food));
    }

    m_canvas->muzzle = m_muzzle;
    m_canvas->flash = m_flash;
    m_canvas->lastHit = m_lastHit;
    m_canvas->animalVisible = m_animalVisible;
    m_canvas->update();
}
