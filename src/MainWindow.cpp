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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Oregon Trail — Xeneon Edge Edition");
    resize(2560, 720);
    setMinimumSize(1280, 480);

    HighScoreManager::instance().load();
    setupUI();
    onNewGame();
}

void MainWindow::setupUI()
{
    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *mainLayout = new QHBoxLayout(central);
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
    m_btnNew    = makeBtn("New Game");
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
    connect(m_btnNew,    &QPushButton::clicked, this, &MainWindow::onNewGame);
    connect(m_btnScores, &QPushButton::clicked, this, &MainWindow::onHighScores);
}

void MainWindow::onNewGame()
{
    StartDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) {
        if (m_game.day() == 1 && m_game.miles() == 0) {
            // first launch — force a start
            m_game.newGame({"Leader", "Partner", "Child", "Child"}, 1);
            m_leaderName = "Leader";
        }
        updateUI();
        return;
    }

    m_leaderName = dlg.leaderName();
    m_game.newGame(dlg.partyNames(), dlg.occupation());
    SoundManager::instance().play("travel");
    updateUI();
}

void MainWindow::onTravel()
{
    // Check for river prompt
    if (m_game.miles() > 400 && m_game.miles() < 450 && m_game.eventLog().contains("wide river")) {
        QStringList options = {"Ford the river (risky)", "Caulk the wagon (safer)", "Take the ferry ($5)"};
        bool ok;
        QString choice = QInputDialog::getItem(this, "River Crossing", "How will you cross?", options, 0, false, &ok);
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
    if (m_game.bullets() < 4) {
        QMessageBox::information(this, "Hunt", "Not enough bullets.");
        return;
    }
    HuntingMinigame hunt(this);
    if (hunt.exec() == QDialog::Accepted || true) {
        m_game.hunt(hunt.foodGained(), hunt.bulletsUsed());
        SoundManager::instance().play("hunt");
    }
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

    QMessageBox::information(this, "Journey Ended", msg);
}
