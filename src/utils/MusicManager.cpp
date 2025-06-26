/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** MusicManager
*/

#include "MusicManager.hpp"

namespace triad
{
    MusicManager &MusicManager::GetInstance()
    {
        static MusicManager instance;
        return instance;
    }

    void MusicManager::Play(const std::string &filePath)
    {
        _music.stop();
        if (!_music.openFromFile(filePath)) {
            throw Error("Failed to load music file: " + filePath);
        }
        _currentFile = filePath;
        _music.setLoop(true);
        _music.play();
    }

    void MusicManager::Stop()
    {
        _music.stop();
    }

    void MusicManager::Pause()
    {
        _music.pause();
    }

    void MusicManager::Resume()
    {
        _music.play();
    }

    bool MusicManager::IsPlaying() const
    {
        return _music.getStatus() == sf::Music::Playing;
    }

    bool MusicManager::IsPlayingFile(const std::string &filePath) const
    {
        return IsPlaying() && _currentFile == filePath;
    }

    void MusicManager::SetVolume(float volume)
    {
        _volume = volume;
        _music.setVolume(volume);
    }

    float MusicManager::GetVolume() const
    {
        return _volume;
    }
}
