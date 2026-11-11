#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QStackedWidget>
#include "GameState.h"
#include "TrailWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onTravel();
    void onHunt();
    void onRest();
    void onTrade();
    void onNewGame();
    void onHighScores();
    void onQuit();
    void updateUI();
    void showMenu();
    void resumeGame();

private:
    void setupUI();
    void endGame();
    void updateMenuButtons();

    GameState m_game;
    QStackedWidget *m_stack = nullptr;
    QWidget *m_menuPage = nullptr;
    QWidget *m_gamePage = nullptr;
    TrailWidget *m_trail = nullptr;
    QLabel *m_status = nullptr;
    QTextEdit *m_log = nullptr;
    QPushButton *m_btnTravel = nullptr;
    QPushButton *m_btnHunt = nullptr;
    QPushButton *m_btnRest = nullptr;
    QPushButton *m_btnTrade = nullptr;
    QPushButton *m_btnNew = nullptr;
    QPushButton *m_btnScores = nullptr;
    QPushButton *m_btnResume = nullptr;
    QString m_leaderName;
    bool m_gameInProgress = false;
};
