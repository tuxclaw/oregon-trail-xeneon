#include "SoundManager.h"
#include <QDir>
#include <QDebug>

SoundManager &SoundManager::instance()
{
    static SoundManager inst;
    return inst;
}

SoundManager::SoundManager()
{
    // Optional: place .wav files in a "sounds" folder next to the binary
    // e.g. travel.wav, hunt.wav, event.wav, win.wav, lose.wav, shot.wav
    QStringList names = {"travel", "hunt", "event", "win", "lose", "shot", "river"};
    QDir dir("sounds");
    for (const auto &n : names) {
        QString path = dir.filePath(n + ".wav");
        if (QFile::exists(path)) {
            auto *s = new QSoundEffect(this);
            s->setSource(QUrl::fromLocalFile(path));
            s->setVolume(0.6);
            m_sounds[n] = s;
        }
    }
}

void SoundManager::play(const QString &name)
{
    if (!m_enabled) return;
    if (auto *s = m_sounds.value(name, nullptr)) {
        s->play();
    }
}
