#include "TrailWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QLinearGradient>

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

    // Sky gradient
    QLinearGradient sky(0, 0, 0, height());
    sky.setColorAt(0.0, QColor(70, 130, 180));
    sky.setColorAt(0.45, QColor(135, 206, 235));
    sky.setColorAt(0.7, QColor(255, 220, 150));
    sky.setColorAt(1.0, QColor(46, 125, 50));
    p.fillRect(rect(), sky);

    // Simple ground
    p.setBrush(QColor(85, 107, 47));
    p.drawRect(0, height() * 0.65, width(), height() * 0.35);

    if (!m_state) return;

    int progress = qMin(100, m_state->miles() * 100 / 2040);
    int barW = width() - 60;
    int barH = 36;
    int barX = 30;
    int barY = height() * 0.55;

    // Trail bar
    p.setPen(QPen(Qt::black, 2));
    p.setBrush(QColor(120, 80, 40));
    p.drawRoundedRect(barX, barY, barW, barH, 10, 10);

    p.setBrush(QColor(210, 180, 140));
    p.drawRoundedRect(barX, barY, barW * progress / 100.0, barH, 10, 10);

    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 15, QFont::Bold));
    p.drawText(barX, barY - 14, QString("Independence  →  Oregon City    %1 / 2040 miles").arg(m_state->miles()));

    // Landmarks
    auto drawLandmark = [&](int mile, const QString &name) {
        int x = barX + (barW * mile / 2040);
        p.setPen(Qt::yellow);
        p.drawLine(x, barY + barH, x, barY + barH + 20);
        p.drawText(x - 40, barY + barH + 38, name);
    };
    drawLandmark(500, "River");
    drawLandmark(950, "Chimney Rock");
    drawLandmark(1450, "Fort Laramie");

    // Better wagon
    int wagonX = barX + (barW * progress / 100.0) - 40;
    int wagonY = barY - 50;
    p.setBrush(QColor(160, 82, 45));
    p.drawRoundedRect(wagonX, wagonY, 80, 36, 6, 6);
    p.setBrush(QColor(100, 50, 20));
    p.drawRect(wagonX + 10, wagonY - 12, 50, 14); // cover
    p.setBrush(Qt::black);
    p.drawEllipse(wagonX + 8, wagonY + 28, 16, 16);
    p.drawEllipse(wagonX + 56, wagonY + 28, 16, 16);
    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 11, QFont::Bold));
    p.drawText(wagonX + 10, wagonY - 18, "Wagon");
}
