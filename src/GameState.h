#pragma once
#include <QString>
#include <QStringList>
#include <QVector>
#include <random>

struct Person {
    QString name;
    int health = 100; // 0-100
    bool alive = true;
};

class GameState {
public:
    GameState();

    void newGame(const QString &leaderName, int occupation);
    void advanceDay();
    void hunt();
    void rest();
    void trade();
    bool isGameOver() const;
    QString statusText() const;
    QString eventLog() const;

    // Getters
    int miles() const { return m_miles; }
    int day() const { return m_day; }
    int money() const { return m_money; }
    int food() const { return m_food; }
    int bullets() const { return m_bullets; }
    int clothing() const { return m_clothing; }
    int oxen() const { return m_oxen; }
    const QVector<Person> &party() const { return m_party; }

private:
    void randomEvent();
    void consumeFood();
    void checkHealth();

    int m_miles = 0;
    int m_day = 1;
    int m_money = 0;
    int m_food = 0;
    int m_bullets = 0;
    int m_clothing = 0;
    int m_oxen = 0;
    int m_misc = 0;
    QVector<Person> m_party;
    QStringList m_log;
    std::mt19937 m_rng;

    static constexpr int TOTAL_MILES = 2040;
};
