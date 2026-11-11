#pragma once
#include <QString>
#include <QStringList>
#include <QVector>
#include <random>

enum class Weather { Clear, Rain, HeavyRain, Storm, Snow, Blizzard };

struct Person {
    QString name;
    int health = 100;
    bool alive = true;
};

struct River {
    int mile;
    QString name;
    bool crossed = false;
};

class GameState {
public:
    GameState();

    void newGame(const QStringList &names, int occupation);
    void advanceDay();
    void hunt(int foodGained, int bulletsUsed);
    void rest();
    void trade();
    bool attemptRiverCrossing(int choice); // 0=ford, 1=caulk, 2=ferry
    bool isGameOver() const;
    bool reachedOregon() const { return m_miles >= TOTAL_MILES; }
    QString statusText() const;
    QString eventLog() const;
    int calculateScore() const;
    Weather weather() const { return m_weather; }
    QString weatherName() const;
    bool pendingRiver() const { return m_pendingRiverIndex >= 0; }
    QString pendingRiverName() const;

    int miles() const { return m_miles; }
    int day() const { return m_day; }
    int money() const { return m_money; }
    int food() const { return m_food; }
    int bullets() const { return m_bullets; }
    int clothing() const { return m_clothing; }
    int oxen() const { return m_oxen; }
    const QVector<Person> &party() const { return m_party; }
    int survivors() const;

private:
    void randomEvent();
    void consumeFood();
    void checkHealth();
    void updateWeather();
    void checkRivers();
    void applyWeatherEffects();

    int m_miles = 0;
    int m_day = 1;
    int m_money = 0;
    int m_food = 0;
    int m_bullets = 0;
    int m_clothing = 0;
    int m_oxen = 0;
    QVector<Person> m_party;
    QStringList m_log;
    std::mt19937 m_rng;
    Weather m_weather = Weather::Clear;
    QVector<River> m_rivers;
    int m_pendingRiverIndex = -1;

    static constexpr int TOTAL_MILES = 2040;
};
