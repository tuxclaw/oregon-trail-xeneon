#include "TrailWidget.h"
#include <QPainter>
#include <QPaintEvent>

TrailWidget::TrailWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(200);
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

    // Background — prairie sky and ground
    QLinearGradient sky(0, 0, 0, height());
    sky.setColorAt(0, QColor(135, 206, 235));
    sky.setColorAt(0.6, QColor(255, 228, 181));
    sky.setColorAt(1, QColor(34, 139, 34));
    p.fillRect(rect(), sky);

    if (!m_state) return;

    // Trail progress bar
    int progress = qMin(100, m_state->miles() * 100 / 2040);
    int barW = width() - 40;
    int barH = 30;
    int barX = 20;
    int barY = height() / 2 - 15;

    p.setPen(Qt::black);
    p.setBrush(QColor(101, 67, 33));
    p.drawRoundedRect(barX, barY, barW, barH, 8, 8);

    p.setBrush(QColor(210, 180, 140));
    p.drawRoundedRect(barX, barY, barW * progress / 100, barH, 8, 8);

    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 14, QFont::Bold));
    p.drawText(barX, barY - 10, QString("Independence → Oregon City   (%1 miles)").arg(m_state->miles()));

    // Simple wagon icon at progress position
    int wagonX = barX + (barW * progress / 100) - 30;
    p.setBrush(QColor(139, 69, 19));
    p.drawRect(wagonX, barY - 40, 60, 30);
    p.setBrush(Qt::black);
    p.drawEllipse(wagonX + 5, barY - 15, 12, 12);
    p.drawEllipse(wagonX + 43, barY - 15, 12, 12);

    p.setPen(Qt::white);
    p.drawText(wagonX, barY - 50, "Wagon");
}
