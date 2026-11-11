#include "MainWindow.h"
#include "StartDialog.h"
#include "HuntingMinigame.h"
#include "HighScoreManager.h"
#include "SoundManager.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QFont>
#include <QMessageBox>
#include <QInputDialog>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Oregon Trail — Xeneon Edge Edition");
    resize(2560, 720);
    setMinimumSize(1280, 480);

    HighScoreManager::instance().load();
    setupUI();
    showMenu();
}

void MainWindow::setupUI()
{
    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    // Menu page
    m_menuPage = new QWidget;
    auto *menuLayout = new QVBoxLayout(m_menuPage);
    menuLayout->setAlignment(Qt::AlignCenter);

    auto *title = new QLabel("OREGON TRAIL");
    title->setFont(QFont("Arial", 48, QFont::Bold));
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #f0e6d2;");

    auto *sub = new QLabel("Xeneon Edge Edition");
    sub->setFont(QFont("Arial", 22));
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("color: #c0b090;");

    auto makeMenuBtn = [](const QString &text) {
        auto *b = new QPushButton(text);
        b->setMinimumSize(420, 78);
        b->setFont(QFont("Arial", 20, QFont::Bold));
        b->setStyleSheet(
            "QPushButton { background: #4a6741; color: white; border-radius: 16px; }"
            "QPushButton:hover { background: #5a8751; }"
            "QPushButton:disabled { background: #2a3a2a; color: #666; }"
        );
        return b;
    };

    m_btnResume = makeMenuBtn("Resume Journey");
    auto *btnNew = makeMenuBtn("New Journey");
    auto *btnScores = makeMenuBtn("High Scores");
    auto *btnQuit = makeMenuBtn("Quit");

    menuLayout->addStretch();
    menuLayout->addWidget(title);
    menuLayout->addWidget(sub);
    menuLayout->addSpacing(36);
    menuLayout->addWidget(m_btnResume);
    menuLayout->addWidget(btnNew);
    menuLayout->addWidget(btnScores);
    menuLayout->addWidget(btnQuit);
    menuLayout->addStretch();

    m_menuPage->setStyleSheet("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #1a2a1a, stop:1 #0d1a0d);");

    connect(m_btnResume, &QPushButton::clicked, this, &MainWindow::resumeGame);
    connect(btnNew, &QPushButton::clicked, this, &MainWindow::onNewGame);
    connect(btnScores, &QPushButton::clicked, this, &MainWindow::onHighScores);
    connect(btnQuit, &QPushButton::clicked, this, &MainWindow::onQuit);

    // Game page
    m_gamePage = new QWidget;
    auto *mainLayout = new QHBoxLayout(m_gamePage);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    m_status = new QLabel;
    m_status->setFont(QFont("Consolas", 15));
    m_status->setStyleSheet("QLabel { background: #121212; color: #e8e8e8; padding: 14px; border-radius: 10px; }");
    m_status->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_status->setMinimumWidth(400);
    m_status->setWordWrap(true);

    m_trail = new TrailWidget;
    m_trail->setStyleSheet("border-radius: 10px;");

    auto *right = new QVBoxLayout;
    m_log = new QTextEdit;
    m_log->setReadOnly(true);
    m_log->setFont(QFont("Consolas", 13));
    m_log->setStyleSheet("QTextEdit { background: #121212; color: #d0d0d0; border-radius: 10px; }");

    auto *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(6);

    auto makeBtn = [](const QString &text) {
        auto *b = new QPushButton(text);
        b->setMinimumHeight(68);
        b->setFont(QFont("Arial", 15, QFont::Bold));
        b->setStyleSheet(
            "QPushButton { background: #3d6b4f; color: white; border-radius: 12px; padding: 6px; }"
            "QPushButton:hover { background: #4d8b5f; }"
            "QPushButton:pressed { background: #2d5b3f; }"
        );
        return b;
    };

    m_btnTravel = makeBtn("Travel");
    m_btnHunt   = makeBtn("Hunt");
    m_btnRest   = makeBtn("Rest");
    m_btnTrade  = makeBtn("Trade");
    m_btnNew    = makeBtn("Menu");
    m_btnScores = makeBtn("Scores");

    btnLayout->addWidget(m_btnTravel);
    btnLayout->addWidget(m_btnHunt);
    btnLayout->addWidget(m_btnRest);
    btnLayout->addWidget(m_btnTrade);
    btnLayout->addWidget(m_btnNew);
    btnLayout->addWidget(m_btnScores);

    right->addWidget(m_log, 1);
    right->addLayout(btnLayout);

    mainLayout->addWidget(m_status, 2);
    mainLayout->addWidget(m_trail, 5);
    mainLayout->addLayout(right, 3);

    connect(m_btnTravel, &QPushButton::clicked, this, &MainWindow::onTravel);
    connect(m_btnHunt,   &QPushButton::clicked, this, &MainWindow::onHunt);
    connect(m_btnRest,   &QPushButton::clicked, this, &MainWindow::onRest);
    connect(m_btnTrade,  &QPushButton::clicked, this, &MainWindow::onTrade);
    connect(m_btnNew,    &QPushButton::clicked, this, &MainWindow::showMenu);
    connect(m_btnScores, &QPushButton::clicked, this, &MainWindow::onHighScores);

    m_stack->addWidget(m_menuPage);
    m_stack->addWidget(m_gamePage);
}

void MainWindow::updateMenuButtons()
{
    m_btnResume->setEnabled(m_gameInProgress && !m_game.isGameOver());
    m_btnResume->setVisible(m_gameInProgress && !m_game.isGameOver());
}

void MainWindow::showMenu()
{
    updateMenuButtons();
    m_stack->setCurrentWidget(m_menuPage);
}

void MainWindow::resumeGame()
{
    if (m_gameInProgress && !m_game.isGameOver()) {
        m_stack->setCurrentWidget(m_gamePage);
        updateUI();
    }
}

void MainWindow::onNewGame()
{
    StartDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) return;

    m_leaderName = dlg.leaderName();
    m_game.newGame(dlg.partyNames(), dlg.occupation());
    m_gameInProgress = true;
    SoundManager::instance().play("travel");
    m_stack->setCurrentWidget(m_gamePage);
    updateUI();
}

void MainWindow::onTravel()
{
    if (m_game.pendingRiver()) {
        QStringList options = {"Ford the river (risky)", "Caulk the wagon", "Take the ferry"};
        bool ok;
        QString choice = QInputDialog::getItem(this, "River Crossing",
                                               QString("You have reached the %1.\nHow will you cross?").arg(m_game.pendingRiverName()),
                                               options, 0, false, &ok);
        if (ok) {
            int idx = options.indexOf(choice);
            m_game.attemptRiverCrossing(idx);
            SoundManager::instance().play("river");
            updateUI();
            return;
        }
    }

    m_game.advanceDay();
    SoundManager::instance().play("travel");
    updateUI();
}

void MainWindow::onHunt()
{
    if (m_game.bullets() < 1) {
        QMessageBox::information(this, "Hunt", "Not enough bullets.");
        return;
    }
    HuntingMinigame hunt(this);
    hunt.exec();
    m_game.hunt(hunt.foodGained(), hunt.bulletsUsed());
    SoundManager::instance().play("hunt");
    updateUI();
}

void MainWindow::onRest()
{
    m_game.rest();
    updateUI();
}

void MainWindow::onTrade()
{
    m_game.trade();
    updateUI();
}

void MainWindow::onHighScores()
{
    auto scores = HighScoreManager::instance().topScores(10);
    QString text = "High Scores:\n\n";
    if (scores.isEmpty()) text += "No scores yet.";
    for (int i = 0; i < scores.size(); ++i) {
        const auto &s = scores[i];
        text += QString("%1. %2 — %3 pts (%4 survivors, %5 days)\n")
                    .arg(i+1).arg(s.name).arg(s.score).arg(s.survivors).arg(s.days);
    }
    QMessageBox::information(this, "High Scores", text);
}

void MainWindow::onQuit()
{
    close();
}

void MainWindow::updateUI()
{
    m_status->setText(m_game.statusText());
    m_log->setPlainText(m_game.eventLog());
    m_log->moveCursor(QTextCursor::End);
    m_trail->setGameState(&m_game);

    if (m_game.isGameOver())
        endGame();
}

void MainWindow::endGame()
{
    int score = m_game.calculateScore();
    QString msg;
    if (m_game.reachedOregon()) {
        msg = QString("You reached Oregon!\nScore: %1").arg(score);
        SoundManager::instance().play("win");
    } else {
        msg = QString("The party has perished.\nScore: %1").arg(score);
        SoundManager::instance().play("lose");
    }

    HighScoreManager::instance().addScore(m_leaderName.isEmpty() ? "Anonymous" : m_leaderName,
                                          score, m_game.day(), m_game.miles(), m_game.survivors());

    m_gameInProgress = false;
    QMessageBox::information(this, "Journey Ended", msg);
    showMenu();
}
