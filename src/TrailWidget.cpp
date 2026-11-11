#include "TrailWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QLinearGradient>
#include <QRandomGenerator>
#include <QPainterPath>

TrailWidget::TrailWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(240);
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

    // Sky
    QLinearGradient sky(0, 0, 0, height());
    if (w == Weather::Clear) {
        sky.setColorAt(0.0, QColor(60, 120, 180));
        sky.setColorAt(0.45, QColor(140, 190, 220));
        sky.setColorAt(1.0, QColor(255, 210, 140));
    } else if (w == Weather::Rain || w == Weather::HeavyRain) {
        sky.setColorAt(0.0, QColor(70, 85, 100));
        sky.setColorAt(0.5, QColor(110, 120, 130));
        sky.setColorAt(1.0, QColor(150, 155, 145));
    } else if (w == Weather::Storm) {
        sky.setColorAt(0.0, QColor(30, 35, 45));
        sky.setColorAt(0.5, QColor(55, 60, 70));
        sky.setColorAt(1.0, QColor(75, 75, 75));
    } else {
        sky.setColorAt(0.0, QColor(170, 185, 200));
        sky.setColorAt(0.5, QColor(200, 210, 220));
        sky.setColorAt(1.0, QColor(225, 230, 235));
    }
    p.fillRect(rect(), sky);

    // Ground
    QColor ground = (w == Weather::Snow || w == Weather::Blizzard) ? QColor(235, 240, 245) : QColor(75, 100, 40);
    p.setBrush(ground);
    p.setPen(Qt::NoPen);
    p.drawRect(0, height() * 0.68, width(), height());

    // Distant hills
    p.setBrush(QColor(50, 80, 35, 140));
    QPainterPath hills;
    hills.moveTo(0, height() * 0.62);
    hills.quadTo(width() * 0.2, height() * 0.50, width() * 0.45, height() * 0.58);
    hills.quadTo(width() * 0.7, height() * 0.66, width(), height() * 0.52);
    hills.lineTo(width(), height());
    hills.lineTo(0, height());
    p.drawPath(hills);

    // Weather particles
    if (w == Weather::Rain || w == Weather::HeavyRain || w == Weather::Storm) {
        p.setPen(QPen(QColor(190, 205, 220, 150), 1));
        int drops = (w == Weather::Storm) ? 70 : (w == Weather::HeavyRain ? 45 : 22);
        for (int i = 0; i < drops; ++i) {
            int x = QRandomGenerator::global()->bounded(width());
            int y = QRandomGenerator::global()->bounded(height());
            p.drawLine(x, y, x + 1, y + 7);
        }
    }
    if (w == Weather::Snow || w == Weather::Blizzard) {
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(255, 255, 255, 190));
        int flakes = (w == Weather::Blizzard) ? 55 : 28;
        for (int i = 0; i < flakes; ++i) {
            int x = QRandomGenerator::global()->bounded(width());
            int y = QRandomGenerator::global()->bounded(height());
            int s = 2 + QRandomGenerator::global()->bounded(3);
            p.drawEllipse(x, y, s, s);
        }
    }

    if (!m_state) return;

    int progress = qMin(100, m_state->miles() * 100 / 2040);
    int barW = width() - 80;
    int barH = 28;
    int barX = 40;
    int barY = height() * 0.58;

    // Trail
    p.setPen(QPen(QColor(60, 40, 20), 2));
    p.setBrush(QColor(110, 75, 40));
    p.drawRoundedRect(barX, barY, barW, barH, 8, 8);
    p.setBrush(QColor(200, 170, 120));
    p.drawRoundedRect(barX, barY, barW * progress / 100.0, barH, 8, 8);

    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 14, QFont::Bold));
    p.drawText(barX, barY - 12, QString("Independence  →  Oregon City    %1 / 2040 miles   |   %2")
               .arg(m_state->miles()).arg(m_state->weatherName()));

    // Improved wagon
    int wagonX = barX + (barW * progress / 100.0) - 50;
    int wagonY = barY - 48;

    // Body
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(140, 80, 40));
    p.drawRoundedRect(wagonX, wagonY + 8, 90, 32, 4, 4);
    // Cover
    p.setBrush(QColor(90, 50, 25));
    QPainterPath cover;
    cover.moveTo(wagonX + 8, wagonY + 10);
    cover.quadTo(wagonX + 45, wagonY - 18, wagonX + 82, wagonY + 10);
    cover.lineTo(wagonX + 78, wagonY + 14);
    cover.quadTo(wagonX + 45, wagonY - 10, wagonX + 12, wagonY + 14);
    p.drawPath(cover);
    // Wheels
    p.setBrush(QColor(40, 25, 15));
    p.drawEllipse(wagonX + 8, wagonY + 32, 18, 18);
    p.drawEllipse(wagonX + 64, wagonY + 32, 18, 18);
    p.setBrush(QColor(80, 50, 30));
    p.drawEllipse(wagonX + 11, wagonY + 35, 12, 12);
    p.drawEllipse(wagonX + 67, wagonY + 35, 12, 12);
    // Spokes hint
    p.setPen(QPen(QColor(60, 35, 20), 1));
    p.drawLine(wagonX + 17, wagonY + 34, wagonX + 17, wagonY + 48);
    p.drawLine(wagonX + 73, wagonY + 34, wagonX + 73, wagonY + 48);
}
