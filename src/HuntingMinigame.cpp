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

        // Sky + ground
        QLinearGradient sky(0, 0, 0, height());
        sky.setColorAt(0.0, QColor(120, 170, 210));
        sky.setColorAt(0.5, QColor(170, 195, 150));
        sky.setColorAt(1.0, QColor(85, 120, 55));
        p.fillRect(rect(), sky);

        p.setBrush(QColor(65, 95, 40));
        p.setPen(Qt::NoPen);
        p.drawRect(0, height() * 0.64, width(), height());

        // Hills
        p.setBrush(QColor(55, 85, 35, 170));
        QPainterPath hills;
        hills.moveTo(0, height() * 0.60);
        hills.quadTo(width() * 0.3, height() * 0.46, width() * 0.55, height() * 0.56);
        hills.quadTo(width() * 0.8, height() * 0.64, width(), height() * 0.48);
        hills.lineTo(width(), height());
        hills.lineTo(0, height());
        p.drawPath(hills);

        // Trees
        drawTree(p, 80, height() * 0.58, 1.0);
        drawTree(p, 220, height() * 0.60, 0.8);
        drawTree(p, width() - 140, height() * 0.57, 1.1);
        drawTree(p, width() - 280, height() * 0.61, 0.75);
        drawTree(p, 380, height() * 0.62, 0.6);

        // Bushes
        p.setBrush(QColor(35, 65, 25));
        for (int i = 0; i < 6; ++i) {
            int bx = 60 + i * (width() / 6);
            p.drawEllipse(bx, height() * 0.60, 55, 28);
        }

        // Hit/miss flash
        if (flash > 0) {
            QColor c = lastHit ? QColor(255, 230, 120, 70) : QColor(180, 40, 40, 50);
            p.fillRect(rect(), c);
        }

        // Animal
        if (animalVisible) {
            int x = width() * animalX / 100 - 35;
            int y = height() * 0.54;
            drawAnimal(p, x, y, animalType, animFrame);
        }

        // Muzzle flash (from bottom center, as if the player is holding the gun)
        if (muzzle > 0) {
            int cx = width() / 2;
            int cy = height() - 30;
            p.setPen(Qt::NoPen);
            QRadialGradient flashGrad(cx, cy - 20, 40);
            flashGrad.setColorAt(0, QColor(255, 255, 180, 220));
            flashGrad.setColorAt(0.4, QColor(255, 180, 60, 160));
            flashGrad.setColorAt(1, QColor(255, 100, 0, 0));
            p.setBrush(flashGrad);
            p.drawEllipse(cx - 30, cy - 50, 60, 40);
            // Sparks
            p.setPen(QPen(QColor(255, 220, 100), 2));
            for (int i = 0; i < 6; ++i) {
                int dx = (i - 3) * 8;
                p.drawLine(cx + dx, cy - 20, cx + dx * 2, cy - 40 - i * 3);
            }
        }

        // Crosshair
        int cx = width() / 2;
        int cy = height() / 2 + 8;
        p.setPen(QPen(QColor(255, 70, 70), 2));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(cx - 32, cy - 32, 64, 64);
        p.drawLine(cx - 48, cy, cx - 14, cy);
        p.drawLine(cx + 14, cy, cx + 48, cy);
        p.drawLine(cx, cy - 48, cx, cy - 14);
        p.drawLine(cx, cy + 14, cx, cy + 48);
        p.setBrush(QColor(255, 70, 70));
        p.drawEllipse(cx - 3, cy - 3, 6, 6);

        // Instructions
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 13, QFont::Bold));
        p.drawText(10, 24, "SHOOT when the animal is under the crosshair!");
    }

private:
    void drawTree(QPainter &p, int x, int baseY, qreal scale) {
        p.setPen(Qt::NoPen);
        // Trunk
        p.setBrush(QColor(90, 55, 30));
        p.drawRect(x - 6 * scale, baseY, 12 * scale, 40 * scale);
        // Foliage
        p.setBrush(QColor(40, 80, 30));
        p.drawEllipse(x - 28 * scale, baseY - 45 * scale, 56 * scale, 40 * scale);
        p.setBrush(QColor(50, 95, 35));
        p.drawEllipse(x - 22 * scale, baseY - 60 * scale, 44 * scale, 32 * scale);
    }

    void drawAnimal(QPainter &p, int x, int y, int type, int frame) {
        int legOffset = (frame % 4 < 2) ? 4 : -4;

        if (type == 0) { // Deer
            p.setBrush(QColor(150, 95, 45));
            p.setPen(Qt::NoPen);
            p.drawEllipse(x, y, 55, 30);
            p.drawEllipse(x + 40, y - 20, 24, 24);
            p.setBrush(QColor(100, 55, 25));
            p.drawEllipse(x + 48, y - 26, 7, 12);
            // Antlers
            p.setPen(QPen(QColor(90, 50, 20), 2));
            p.drawLine(x + 50, y - 18, x + 46, y - 32);
            p.drawLine(x + 50, y - 18, x + 58, y - 30);
            // Legs (animated)
            p.setPen(QPen(QColor(110, 65, 30), 3));
            p.drawLine(x + 12, y + 28, x + 10 + legOffset, y + 46);
            p.drawLine(x + 24, y + 28, x + 22 - legOffset, y + 46);
            p.drawLine(x + 36, y + 28, x + 38 + legOffset, y + 46);
            p.drawLine(x + 46, y + 28, x + 48 - legOffset, y + 46);
        } else if (type == 1) { // Buffalo
            p.setBrush(QColor(75, 48, 28));
            p.setPen(Qt::NoPen);
            p.drawEllipse(x - 8, y + 2, 85, 42);
            p.drawEllipse(x + 52, y - 10, 32, 30);
            p.setBrush(QColor(55, 35, 18));
            p.drawEllipse(x + 68, y - 6, 10, 10);
            p.setPen(QPen(QColor(55, 35, 18), 4));
            p.drawLine(x + 8, y + 42, x + 4 + legOffset, y + 60);
            p.drawLine(x + 22, y + 42, x + 20 - legOffset, y + 60);
            p.drawLine(x + 48, y + 42, x + 52 + legOffset, y + 60);
            p.drawLine(x + 64, y + 42, x + 68 - legOffset, y + 60);
        } else { // Rabbit
            p.setBrush(QColor(190, 165, 130));
            p.setPen(Qt::NoPen);
            p.drawEllipse(x + 12, y + 6, 32, 22);
            p.drawEllipse(x + 30, y - 6, 18, 18);
            p.setBrush(QColor(210, 190, 160));
            p.drawEllipse(x + 32, y - 16, 5, 14);
            p.drawEllipse(x + 38, y - 16, 5, 14);
            p.setPen(QPen(QColor(160, 130, 95), 2));
            p.drawLine(x + 18, y + 26, x + 16 + legOffset, y + 38);
            p.drawLine(x + 28, y + 26, x + 30 - legOffset, y + 38);
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

    m_timer.start(120); // smoother animation
}

void HuntingMinigame::onTick()
{
    if (m_flash > 0) m_flash--;
    if (m_muzzle > 0) m_muzzle--;
    m_animFrame++;

    // Time counts down more slowly relative to animation ticks
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

    // Move animal
    if (m_animalVisible) {
        m_animalX += m_animalDir * (1 + (m_animalType == 2 ? 2 : 0));
        if (m_animalX < 10 || m_animalX > 90) {
            m_animalDir *= -1;
            m_animalX = qBound(10, m_animalX, 90);
        }
        // Occasional disappear
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
    m_bulletsUsed += 2;
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
