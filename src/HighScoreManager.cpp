#include "HighScoreManager.h"
#include <QSettings>
#include <algorithm>

HighScoreManager &HighScoreManager::instance()
{
    static HighScoreManager inst;
    return inst;
}

void HighScoreManager::addScore(const QString &name, int score, int days, int miles, int survivors)
{
    m_scores.append({name, score, days, miles, survivors});
    std::sort(m_scores.begin(), m_scores.end(), [](const HighScore &a, const HighScore &b) {
        return a.score > b.score;
    });
    if (m_scores.size() > 20) m_scores.resize(20);
    save();
}

QList<HighScore> HighScoreManager::topScores(int n) const
{
    return m_scores.mid(0, n);
}

void HighScoreManager::load()
{
    QSettings s("tuxclaw", "OregonTrailXeneon");
    int count = s.beginReadArray("highscores");
    m_scores.clear();
    for (int i = 0; i < count; ++i) {
        s.setArrayIndex(i);
        HighScore hs;
        hs.name = s.value("name").toString();
        hs.score = s.value("score").toInt();
        hs.days = s.value("days").toInt();
        hs.miles = s.value("miles").toInt();
        hs.survivors = s.value("survivors").toInt();
        m_scores.append(hs);
    }
    s.endArray();
}

void HighScoreManager::save()
{
    QSettings s("tuxclaw", "OregonTrailXeneon");
    s.beginWriteArray("highscores");
    for (int i = 0; i < m_scores.size(); ++i) {
        s.setArrayIndex(i);
        s.setValue("name", m_scores[i].name);
        s.setValue("score", m_scores[i].score);
        s.setValue("days", m_scores[i].days);
        s.setValue("miles", m_scores[i].miles);
        s.setValue("survivors", m_scores[i].survivors);
    }
    s.endArray();
}
