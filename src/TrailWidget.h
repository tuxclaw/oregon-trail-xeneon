#pragma once
#include <QWidget>
#include <QPixmap>
#include "GameState.h"

class TrailWidget : public QWidget {
    Q_OBJECT
public:
    explicit TrailWidget(QWidget *parent = nullptr);
    void setGameState(const GameState *state);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_wagon;
    QPixmap m_oxen;
    QPixmap m_tree;
    QPixmap m_hills;
    QPixmap m_ground;
    QPixmap m_groundSnow;
    const GameState *m_state = nullptr;
};
