#pragma once
#include <QWidget>
#include "GameState.h"

class TrailWidget : public QWidget {
    Q_OBJECT
public:
    explicit TrailWidget(QWidget *parent = nullptr);
    void setGameState(const GameState *state);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    const GameState *m_state = nullptr;
};
