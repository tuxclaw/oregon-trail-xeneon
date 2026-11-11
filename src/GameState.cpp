#include "GameState.h"
#include <QDateTime>

GameState::GameState()
    : m_rng(QDateTime::currentMSecsSinceEpoch())
{
}

void GameState::newGame(const QString &leaderName, int occupation)
{
    m_miles = 0;
    m_day = 1;
    m_party.clear();
    m_log.clear();

    // Starting resources by occupation (classic values)
    switch (occupation) {
    case 0: // Banker
        m_money = 1600; m_food = 200; m_bullets = 50; m_clothing = 10; m_oxen = 2;
        break;
    case 1: // Carpenter
        m_money = 800; m_food = 200; m_bullets = 50; m_clothing = 10; m_oxen = 2;
        break;
    default: // Farmer
        m_money = 400; m_food = 200; m_bullets = 50; m_clothing = 10; m_oxen = 2;
        break;
    }

    m_party.append({leaderName, 100, true});
    m_party.append({"Partner", 100, true});
    m_party.append({"Child 1", 100, true});
    m_party.append({"Child 2", 100, true});

    m_log << "You have decided to go to Oregon!";
    m_log << QString("Starting as %1 with $%2.").arg(occupation == 0 ? "Banker" : occupation == 1 ? "Carpenter" : "Farmer").arg(m_money);
}

void GameState::advanceDay()
{
    if (isGameOver()) return;

    m_day++;
    int pace = 15 + (m_rng() % 10); // 15-24 miles/day
    m_miles += pace;

    consumeFood();
    randomEvent();
    checkHealth();

    m_log << QString("Day %1: Traveled %2 miles. Total: %3/%4.").arg(m_day).arg(pace).arg(m_miles).arg(TOTAL_MILES);

    if (m_miles >= TOTAL_MILES) {
        m_log << "You have reached Oregon! Congratulations!";
    }
}

void GameState::hunt()
{
    if (m_bullets < 10) {
        m_log << "Not enough bullets to hunt.";
        return;
    }
    m_bullets -= 10;
    int found = 50 + (m_rng() % 100);
    m_food += found;
    m_log << QString("Hunted successfully! Gained %1 pounds of food.").arg(found);
}

void GameState::rest()
{
    for (auto &p : m_party) {
        if (p.alive) p.health = qMin(100, p.health + 15);
    }
    m_log << "You rested for a day. Party health improved.";
    consumeFood();
    m_day++;
}

void GameState::trade()
{
    if (m_money < 50) {
        m_log << "Not enough money to trade.";
        return;
    }
    m_money -= 40;
    m_food += 80;
    m_log << "Traded $40 for extra food.";
}

bool GameState::isGameOver() const
{
    if (m_miles >= TOTAL_MILES) return true;
    bool anyAlive = false;
    for (const auto &p : m_party) if (p.alive) anyAlive = true;
    return !anyAlive;
}

QString GameState::statusText() const
{
    QString s;
    s += QString("Day: %1   Miles: %2 / %3\n").arg(m_day).arg(m_miles).arg(TOTAL_MILES);
    s += QString("Money: $%1   Food: %2 lb   Bullets: %3\n").arg(m_money).arg(m_food).arg(m_bullets);
    s += QString("Clothing: %1   Oxen: %2\n\n").arg(m_clothing).arg(m_oxen);
    s += "Party:\n";
    for (const auto &p : m_party) {
        s += QString("  %1: %2 HP %3\n").arg(p.name).arg(p.health).arg(p.alive ? "" : "(DEAD)");
    }
    return s;
}

QString GameState::eventLog() const
{
    return m_log.join("\n");
}

void GameState::randomEvent()
{
    int roll = m_rng() % 100;
    if (roll < 15) {
        int lost = 20 + (m_rng() % 40);
        m_food = qMax(0, m_food - lost);
        m_log << QString("A wagon wheel broke! Lost %1 food.").arg(lost);
    } else if (roll < 25) {
        m_log << "You met friendly travelers who shared supplies.";
        m_food += 30;
    } else if (roll < 35) {
        m_log << "Heavy rain slowed your progress.";
        m_miles -= 5;
    }
}

void GameState::consumeFood()
{
    int eaters = 0;
    for (const auto &p : m_party) if (p.alive) eaters++;
    int consumed = eaters * 5;
    m_food -= consumed;
    if (m_food < 0) {
        m_food = 0;
        m_log << "You are out of food! Party is starving.";
        for (auto &p : m_party) if (p.alive) p.health -= 20;
    }
}

void GameState::checkHealth()
{
    for (auto &p : m_party) {
        if (!p.alive) continue;
        if (p.health <= 0) {
            p.alive = false;
            p.health = 0;
            m_log << QString("%1 has died.").arg(p.name);
        }
    }
}
