#include "TrailWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QLinearGradient>
#include <QRandomGenerator>

TrailWidget::TrailWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(220);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void TrailWidget::setGameState(const GameState *state)
{
    m_state = state;
    update();
}

void TrailWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    Weather w = m_state ? m_state->weather() : Weather::Clear;

    // Dynamic sky based on weather
    QLinearGradient sky(0, 0, 0, height());
    if (w == Weather::Clear) {
        sky.setColorAt(0.0, QColor(70, 130, 180));
        sky.setColorAt(0.5, QColor(135, 206, 235));
        sky.setColorAt(1.0, QColor(255, 220, 160));
    } else if (w == Weather::Rain || w == Weather::HeavyRain) {
        sky.setColorAt(0.0, QColor(80, 90, 100));
        sky.setColorAt(0.5, QColor(120, 130, 140));
        sky.setColorAt(1.0, QColor(160, 160, 150));
    } else if (w == Weather::Storm) {
        sky.setColorAt(0.0, QColor(40, 45, 55));
        sky.setColorAt(0.5, QColor(70, 75, 85));
        sky.setColorAt(1.0, QColor(90, 90, 90));
    } else { // snow / blizzard
        sky.setColorAt(0.0, QColor(180, 190, 200));
        sky.setColorAt(0.5, QColor(200, 210, 220));
        sky.setColorAt(1.0, QColor(220, 225, 230));
    }
    p.fillRect(rect(), sky);

    // Ground
    QColor ground = (w == Weather::Snow || w == Weather::Blizzard) ? QColor(230, 235, 240) : QColor(85, 107, 47);
    p.setBrush(ground);
    p.drawRect(0, height() * 0.65, width(), height() * 0.35);

    // Weather particles
    if (w == Weather::Rain || w == Weather::HeavyRain || w == Weather::Storm) {
        p.setPen(QPen(QColor(180, 200, 220, 160), 1));
        int drops = (w == Weather::Storm) ? 80 : (w == Weather::HeavyRain ? 50 : 25);
        for (int i = 0; i < drops; ++i) {
            int x = QRandomGenerator::global()->bounded(width());
            int y = QRandomGenerator::global()->bounded(height());
            p.drawLine(x, y, x + 2, y + 8);
        }
    }
    if (w == Weather::Snow || w == Weather::Blizzard) {
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(255, 255, 255, 200));
        int flakes = (w == Weather::Blizzard) ? 60 : 30;
        for (int i = 0; i < flakes; ++i) {
            int x = QRandomGenerator::global()->bounded(width());
            int y = QRandomGenerator::global()->bounded(height());
            int s = 2 + QRandomGenerator::global()->bounded(3);
            p.drawEllipse(x, y, s, s);
        }
    }

    if (!m_state) return;

    int progress = qMin(100, m_state->miles() * 100 / 2040);
    int barW = width() - 60;
    int barH = 36;
    int barX = 30;
    int barY = height() * 0.55;

    p.setPen(QPen(Qt::black, 2));
    p.setBrush(QColor(120, 80, 40));
    p.drawRoundedRect(barX, barY, barW, barH, 10, 10);
    p.setBrush(QColor(210, 180, 140));
    p.drawRoundedRect(barX, barY, barW * progress / 100.0, barH, 10, 10);

    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 15, QFont::Bold));
    p.drawText(barX, barY - 14, QString("Independence  →  Oregon City    %1 / 2040 miles  |  %2")
               .arg(m_state->miles()).arg(m_state->weatherName()));

    // Wagon
    int wagonX = barX + (barW * progress / 100.0) - 40;
    int wagonY = barY - 50;
    p.setBrush(QColor(160, 82, 45));
    p.drawRoundedRect(wagonX, wagonY, 80, 36, 6, 6);
    p.setBrush(QColor(100, 50, 20));
    p.drawRect(wagonX + 10, wagonY - 12, 50, 14);
    p.setBrush(Qt::black);
    p.drawEllipse(wagonX + 8, wagonY + 28, 16, 16);
    p.drawEllipse(wagonX + 56, wagonY + 28, 16, 16);
}
