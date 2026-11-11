#include "MainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QFont>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Oregon Trail — Xeneon Edge Edition");
    // Perfect size for Corsair Xeneon Edge
    resize(2560, 720);
    setMinimumSize(1280, 480);

    setupUI();
    onNewGame();
}

void MainWindow::setupUI()
{
    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(12);

    // Left: Status
    m_status = new QLabel;
    m_status->setFont(QFont("Consolas", 16));
    m_status->setStyleSheet("QLabel { background: #1a1a1a; color: #e0e0e0; padding: 16px; border-radius: 8px; }");
    m_status->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_status->setMinimumWidth(380);
    m_status->setWordWrap(true);

    // Center: Trail
    m_trail = new TrailWidget;
    m_trail->setStyleSheet("background: #2d5016; border-radius: 8px;");

    // Right: Log + buttons
    auto *right = new QVBoxLayout;
    m_log = new QTextEdit;
    m_log->setReadOnly(true);
    m_log->setFont(QFont("Consolas", 14));
    m_log->setStyleSheet("QTextEdit { background: #1a1a1a; color: #d4d4d4; border-radius: 8px; }");

    auto *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(8);

    auto makeBtn = [](const QString &text) {
        auto *b = new QPushButton(text);
        b->setMinimumHeight(70);
        b->setFont(QFont("Arial", 16, QFont::Bold));
        b->setStyleSheet(
            "QPushButton { background: #4a7c59; color: white; border-radius: 10px; padding: 8px; }"
            "QPushButton:hover { background: #5a9c69; }"
            "QPushButton:pressed { background: #3a6c49; }"
        );
        return b;
    };

    m_btnTravel = makeBtn("Travel");
    m_btnHunt   = makeBtn("Hunt");
    m_btnRest   = makeBtn("Rest");
    m_btnTrade  = makeBtn("Trade");
    m_btnNew    = makeBtn("New Game");

    btnLayout->addWidget(m_btnTravel);
    btnLayout->addWidget(m_btnHunt);
    btnLayout->addWidget(m_btnRest);
    btnLayout->addWidget(m_btnTrade);
    btnLayout->addWidget(m_btnNew);

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
}

void MainWindow::onNewGame()
{
    m_game.newGame("Leader", 1); // default Carpenter
    updateUI();
}

void MainWindow::onTravel()
{
    m_game.advanceDay();
    updateUI();
}

void MainWindow::onHunt()
{
    m_game.hunt();
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

void MainWindow::updateUI()
{
    m_status->setText(m_game.statusText());
    m_log->setPlainText(m_game.eventLog());
    m_trail->setGameState(&m_game);

    if (m_game.isGameOver()) {
        QString msg = m_game.miles() >= 2040 ? "You made it to Oregon!" : "The party has perished.";
        QMessageBox::information(this, "Game Over", msg);
    }
}
