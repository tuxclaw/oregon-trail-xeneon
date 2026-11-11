#include "GameState.h"
#include <QDateTime>

GameState::GameState()
    : m_rng(QDateTime::currentMSecsSinceEpoch())
{
    m_rivers = {
        {350, "Kansas River"},
        {650, "Big Blue River"},
        {950, "Platte River"},
        {1350, "North Platte"},
        {1700, "Snake River"}
    };
}

void GameState::newGame(const QStringList &names, int occupation)
{
    m_miles = 0;
    m_day = 1;
    m_party.clear();
    m_log.clear();
    m_pendingRiverIndex = -1;
    m_weather = Weather::Clear;
    for (auto &r : m_rivers) r.crossed = false;

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
    if (isGameOver() || m_pendingRiverIndex >= 0) return;

    m_day++;
    updateWeather();

    int pace = 14 + (m_rng() % 12);
    if (m_weather == Weather::Rain) pace = qMax(8, pace - 4);
    if (m_weather == Weather::HeavyRain || m_weather == Weather::Storm) pace = qMax(5, pace - 8);
    if (m_weather == Weather::Snow) pace = qMax(6, pace - 6);
    if (m_weather == Weather::Blizzard) pace = qMax(2, pace - 12);

    m_miles += pace;
    consumeFood();
    applyWeatherEffects();
    checkRivers();
    randomEvent();
    checkHealth();

    m_log << QString("Day %1: %2. Traveled %3 miles. Total: %4/%5.")
                .arg(m_day).arg(weatherName()).arg(pace).arg(m_miles).arg(TOTAL_MILES);

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
        if (p.alive) p.health = qMin(100, p.health + 20);
    m_log << "You rested. Party health improved.";
    consumeFood();
    m_day++;
    updateWeather();
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
    if (m_pendingRiverIndex < 0) return false;
    auto &river = m_rivers[m_pendingRiverIndex];
    m_pendingRiverIndex = -1;
    river.crossed = true;

    int roll = m_rng() % 100;
    int difficulty = 40 + (river.mile / 40); // harder later rivers

    if (choice == 2) { // ferry
        int cost = 5 + river.mile / 200;
        if (m_money >= cost) {
            m_money -= cost;
            m_log << QString("Took the ferry across the %1 ($%2).").arg(river.name).arg(cost);
            m_miles += 4;
            return true;
        } else {
            m_log << "Cannot afford ferry. Forced to ford.";
            choice = 0;
        }
    }
    if (choice == 1) { // caulk
        if (roll < 75 - difficulty / 5) {
            m_log << QString("Caulked wagon crossed the %1 safely.").arg(river.name);
            m_miles += 3;
            return true;
        } else {
            int lost = 35 + (m_rng() % 60);
            m_food = qMax(0, m_food - lost);
            m_log << QString("Wagon leaked in the %1! Lost %2 food.").arg(river.name).arg(lost);
            return false;
        }
    }
    // ford
    if (roll < 45 - difficulty / 8) {
        m_log << QString("Fording the %1 succeeded.").arg(river.name);
        m_miles += 2;
        return true;
    } else {
        int lost = 50 + (m_rng() % 70);
        m_food = qMax(0, m_food - lost);
        for (auto &p : m_party) if (p.alive) p.health -= 12;
        m_log << QString("Rough ford on the %1! Lost %2 food and health.").arg(river.name).arg(lost);
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
    score += qMax(0, 120 - m_day) * 4;
    if (reachedOregon()) score += 500;
    return score;
}

QString GameState::weatherName() const
{
    switch (m_weather) {
    case Weather::Clear: return "Clear skies";
    case Weather::Rain: return "Light rain";
    case Weather::HeavyRain: return "Heavy rain";
    case Weather::Storm: return "Thunderstorm";
    case Weather::Snow: return "Snow";
    case Weather::Blizzard: return "Blizzard";
    }
    return "Clear";
}

QString GameState::pendingRiverName() const
{
    if (m_pendingRiverIndex < 0) return {};
    return m_rivers[m_pendingRiverIndex].name;
}

QString GameState::statusText() const
{
    QString s;
    s += QString("Day: %1    Miles: %2 / %3\n").arg(m_day).arg(m_miles).arg(TOTAL_MILES);
    s += QString("Weather: %1\n").arg(weatherName());
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

void GameState::updateWeather()
{
    int roll = m_rng() % 100;
    int monthish = m_day / 30; // rough season
    if (monthish >= 4 || m_miles > 1200) { // later / colder
        if (roll < 25) m_weather = Weather::Clear;
        else if (roll < 45) m_weather = Weather::Rain;
        else if (roll < 60) m_weather = Weather::Snow;
        else if (roll < 75) m_weather = Weather::HeavyRain;
        else m_weather = Weather::Blizzard;
    } else {
        if (roll < 40) m_weather = Weather::Clear;
        else if (roll < 65) m_weather = Weather::Rain;
        else if (roll < 80) m_weather = Weather::HeavyRain;
        else m_weather = Weather::Storm;
    }
}

void GameState::applyWeatherEffects()
{
    if (m_weather == Weather::Storm || m_weather == Weather::Blizzard) {
        for (auto &p : m_party) if (p.alive) p.health -= 5;
        m_log << "Severe weather is taking a toll on the party.";
    }
    if (m_weather == Weather::Snow && m_clothing < 5) {
        for (auto &p : m_party) if (p.alive) p.health -= 4;
        m_log << "Not enough clothing for the cold.";
    }
}

void GameState::checkRivers()
{
    for (int i = 0; i < m_rivers.size(); ++i) {
        auto &r = m_rivers[i];
        if (!r.crossed && m_miles >= r.mile && m_miles < r.mile + 40) {
            m_pendingRiverIndex = i;
            m_log << QString("You have reached the %1. Choose how to cross!").arg(r.name);
            return;
        }
    }
}

void GameState::randomEvent()
{
    int roll = m_rng() % 100;
    if (roll < 10) {
        int lost = 20 + (m_rng() % 35);
        m_food = qMax(0, m_food - lost);
        m_log << QString("Wagon wheel broke! Lost %1 food.").arg(lost);
    } else if (roll < 16) {
        m_log << "Friendly travelers shared supplies.";
        m_food += 35;
    } else if (roll < 22) {
        m_log << "You found wild berries.";
        m_food += 18;
    }
}

void GameState::consumeFood()
{
    int eaters = survivors();
    int consumed = eaters * 5;
    if (m_weather == Weather::Snow || m_weather == Weather::Blizzard) consumed += eaters * 2;
    m_food -= consumed;
    if (m_food < 0) {
        m_food = 0;
        m_log << "Out of food! Party is starving.";
        for (auto &p : m_party) if (p.alive) p.health -= 16;
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
