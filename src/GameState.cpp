#include "GameState.h"
#include <QDateTime>

GameState::GameState()
    : m_rng(QDateTime::currentMSecsSinceEpoch())
{
}

void GameState::newGame(const QStringList &names, int occupation)
{
    m_miles = 0;
    m_day = 1;
    m_party.clear();
    m_log.clear();
    m_nearRiver = false;

    switch (occupation) {
    case 0: m_money = 1600; break;
    case 1: m_money = 800; break;
    default: m_money = 400; break;
    }
    m_food = 200;
    m_bullets = 50;
    m_clothing = 10;
    m_oxen = 2;

    for (const auto &n : names) {
        if (!n.isEmpty())
            m_party.append({n, 100, true});
    }
    while (m_party.size() < 4)
        m_party.append({QString("Member %1").arg(m_party.size()+1), 100, true});

    m_log << "You have decided to go to Oregon!";
    QString occ = occupation == 0 ? "Banker" : occupation == 1 ? "Carpenter" : "Farmer";
    m_log << QString("Starting as %1 with $%2.").arg(occ).arg(m_money);
}

void GameState::advanceDay()
{
    if (isGameOver()) return;

    m_day++;
    int pace = 12 + (m_rng() % 14);
    m_miles += pace;

    consumeFood();
    checkLandmark();
    randomEvent();
    checkHealth();

    m_log << QString("Day %1: Traveled %2 miles. Total: %3/%4.")
                .arg(m_day).arg(pace).arg(m_miles).arg(TOTAL_MILES);

    if (m_miles >= TOTAL_MILES)
        m_log << "You have reached Oregon City! Congratulations!";
}

void GameState::hunt(int foodGained, int bulletsUsed)
{
    m_bullets = qMax(0, m_bullets - bulletsUsed);
    m_food += foodGained;
    m_log << QString("Hunting results: +%1 lb food, -%2 bullets.").arg(foodGained).arg(bulletsUsed);
}

void GameState::rest()
{
    for (auto &p : m_party)
        if (p.alive) p.health = qMin(100, p.health + 18);
    m_log << "You rested. Party health improved.";
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
    m_food += 90;
    m_log << "Traded $40 for extra food.";
}

bool GameState::attemptRiverCrossing(int choice)
{
    m_nearRiver = false;
    int roll = m_rng() % 100;
    if (choice == 2) { // ferry
        if (m_money >= 5) {
            m_money -= 5;
            m_log << "You took the ferry safely across.";
            m_miles += 5;
            return true;
        } else {
            m_log << "Cannot afford ferry. Forced to ford.";
            choice = 0;
        }
    }
    if (choice == 1) { // caulk
        if (roll < 70) {
            m_log << "Caulked wagon crossed safely.";
            m_miles += 3;
            return true;
        } else {
            int lost = 30 + (m_rng() % 50);
            m_food = qMax(0, m_food - lost);
            m_log << QString("Wagon leaked! Lost %1 food.").arg(lost);
            return false;
        }
    }
    // ford
    if (roll < 50) {
        m_log << "Fording succeeded with some difficulty.";
        m_miles += 2;
        return true;
    } else {
        int lost = 40 + (m_rng() % 60);
        m_food = qMax(0, m_food - lost);
        for (auto &p : m_party) if (p.alive) p.health -= 10;
        m_log << QString("Rough ford! Lost %1 food and some health.").arg(lost);
        return false;
    }
}

bool GameState::isGameOver() const
{
    if (m_miles >= TOTAL_MILES) return true;
    return survivors() == 0;
}

int GameState::survivors() const
{
    int n = 0;
    for (const auto &p : m_party) if (p.alive) n++;
    return n;
}

int GameState::calculateScore() const
{
    int score = survivors() * 200;
    score += m_money / 2;
    score += m_food / 5;
    score += qMax(0, 100 - m_day) * 5; // faster is better
    if (reachedOregon()) score += 500;
    return score;
}

QString GameState::statusText() const
{
    QString s;
    s += QString("Day: %1    Miles: %2 / %3\n").arg(m_day).arg(m_miles).arg(TOTAL_MILES);
    s += QString("Money: $%1    Food: %2 lb    Bullets: %3\n").arg(m_money).arg(m_food).arg(m_bullets);
    s += QString("Clothing: %1    Oxen: %2\n\n").arg(m_clothing).arg(m_oxen);
    s += "Party:\n";
    for (const auto &p : m_party) {
        s += QString("  %1: %2 HP %3\n").arg(p.name, -12).arg(p.health).arg(p.alive ? "" : "(DEAD)");
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
    if (roll < 12) {
        int lost = 25 + (m_rng() % 40);
        m_food = qMax(0, m_food - lost);
        m_log << QString("Wagon wheel broke! Lost %1 food.").arg(lost);
    } else if (roll < 20) {
        m_log << "Friendly travelers shared supplies.";
        m_food += 40;
    } else if (roll < 28) {
        m_log << "Heavy rain slowed progress.";
        m_miles = qMax(0, m_miles - 8);
    } else if (roll < 34) {
        m_log << "You found wild berries.";
        m_food += 20;
    } else if (roll < 38) {
        m_log << "Oxen are tired. Progress reduced.";
        m_miles = qMax(0, m_miles - 5);
    }
}

void GameState::checkLandmark()
{
    // Simple landmark triggers near certain miles
    if (m_miles > 400 && m_miles < 420 && !m_nearRiver) {
        m_nearRiver = true;
        m_log << "You have reached a wide river. Choose how to cross!";
    }
    if (m_miles > 900 && m_miles < 920)
        m_log << "You pass Chimney Rock — a famous landmark.";
    if (m_miles > 1400 && m_miles < 1420)
        m_log << "You reach Fort Laramie. Supplies available.";
}

void GameState::consumeFood()
{
    int eaters = survivors();
    int consumed = eaters * 5;
    m_food -= consumed;
    if (m_food < 0) {
        m_food = 0;
        m_log << "Out of food! Party is starving.";
        for (auto &p : m_party) if (p.alive) p.health -= 18;
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
