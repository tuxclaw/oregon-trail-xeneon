#pragma once
#include <QString>
#include <QList>

struct HighScore {
    QString name;
    int score = 0;
    int days = 0;
    int miles = 0;
    int survivors = 0;
};

class HighScoreManager {
public:
    static HighScoreManager &instance();

    void addScore(const QString &name, int score, int days, int miles, int survivors);
    QList<HighScore> topScores(int n = 10) const;
    void load();
    void save();

private:
    HighScoreManager() = default;
    QList<HighScore> m_scores;
};
