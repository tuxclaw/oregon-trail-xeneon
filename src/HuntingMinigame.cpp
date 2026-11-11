#include "HuntingMinigame.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QRandomGenerator>

class HuntCanvas : public QWidget {
public:
    int target = 50;
    bool visible = false;
    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.fillRect(rect(), QColor(34, 80, 34));
        p.setPen(Qt::white);
        p.drawText(10, 30, "Click SHOOT when the deer is centered!");
        if (visible) {
            int x = width() * target / 100 - 20;
            p.setBrush(QColor(139, 90, 43));
            p.drawEllipse(x, height()/2 - 20, 40, 40);
            p.drawText(x, height()/2 + 40, "Deer");
        }
        // Crosshair
        p.setPen(QPen(Qt::red, 2));
        int cx = width()/2;
        p.drawLine(cx-15, height()/2, cx+15, height()/2);
        p.drawLine(cx, height()/2-15, cx, height()/2+15);
    }
};

HuntingMinigame::HuntingMinigame(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Hunting");
    resize(800, 400);

    auto *layout = new QVBoxLayout(this);
    auto *canvas = new HuntCanvas;
    canvas->setMinimumHeight(250);
    m_status = new QLabel("Time: 20s   Food: 0");
    m_status->setFont(QFont("Arial", 16));
    m_shoot = new QPushButton("SHOOT");
    m_shoot->setMinimumHeight(70);
    m_shoot->setFont(QFont("Arial", 20, QFont::Bold));

    layout->addWidget(canvas);
    layout->addWidget(m_status);
    layout->addWidget(m_shoot);

    connect(&m_timer, &QTimer::timeout, this, &HuntingMinigame::onTick);
    connect(m_shoot, &QPushButton::clicked, this, &HuntingMinigame::onShoot);

    m_timer.start(500);
    // Store canvas pointer for updates
    setProperty("canvas", QVariant::fromValue((void*)canvas));
}

void HuntingMinigame::onTick()
{
    m_timeLeft--;
    if (m_timeLeft <= 0) {
        m_timer.stop();
        accept();
        return;
    }

    // Random deer appearance
    if (QRandomGenerator::global()->bounded(100) < 30) {
        m_target = QRandomGenerator::global()->bounded(20, 80);
        m_visible = true;
    } else {
        m_visible = false;
    }

    auto *canvas = (HuntCanvas*)property("canvas").value<void*>();
    if (canvas) {
        canvas->target = m_target;
        canvas->visible = m_visible;
        canvas->update();
    }

    m_status->setText(QString("Time: %1s   Food: %2 lb").arg(m_timeLeft).arg(m_food));
}

void HuntingMinigame::onShoot()
{
    m_bulletsUsed += 2;
    if (m_visible && qAbs(m_target - 50) < 15) {
        int gain = 40 + QRandomGenerator::global()->bounded(60);
        m_food += gain;
        m_status->setText(QString("HIT! +%1 lb   Time: %2s").arg(gain).arg(m_timeLeft));
    } else {
        m_status->setText(QString("Miss!   Time: %1s   Food: %2").arg(m_timeLeft).arg(m_food));
    }
}
