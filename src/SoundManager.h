#pragma once
#include <QObject>
#include <QSoundEffect>
#include <QMap>

class SoundManager : public QObject {
    Q_OBJECT
public:
    static SoundManager &instance();

    void play(const QString &name);
    void setEnabled(bool on) { m_enabled = on; }
    bool enabled() const { return m_enabled; }

private:
    SoundManager();
    QMap<QString, QSoundEffect*> m_sounds;
    bool m_enabled = true;
};
