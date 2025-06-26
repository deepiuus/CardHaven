/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** MusicManager
*/

#ifndef MUSICMANAGER_HPP_
#define MUSICMANAGER_HPP_

#include <SFML/Audio.hpp>
#include <string>
#include "Error.hpp"

namespace triad
{
    class MusicManager {
        public:
            static MusicManager &GetInstance();
            void Play(const std::string &filePath);
            void Stop();
            void Pause();
            void Resume();
            bool IsPlaying() const;
            bool IsPlayingFile(const std::string &filePath) const;
            void SetVolume(float volume);
            float GetVolume() const;

        private:
            MusicManager() = default;
            sf::Music _music;
            float _volume;
            std::string _currentFile;
    };
}

#endif /* !MUSICMANAGER_HPP_ */
