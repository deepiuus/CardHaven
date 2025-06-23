/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** LevelManager
*/

#ifndef LEVELMANAGER_HPP_
#define LEVELMANAGER_HPP_

#include <fstream>
#include "../IGames.hpp"
#include "Error.hpp"

namespace triad
{
    enum class TLevel {
        LEVEL1,
        LEVEL2,
        LEVEL3,
        LEVEL4,
        LEVEL5,
    };
    class LevelManager {
        public:
            LevelManager();
            ~LevelManager();
            void LoadLevel();
            void NextLevel();
            void SetLevel(TLevel level);
            TLevel GetLevel() const;
            const std::vector<std::vector<char>> &GetMap() const;
            void ResetLevel();
            void StartDialogue();
            bool IsDialogueFinished() const;
            const std::string &GetDialogue() const;
            void NextDialogue();
            size_t GetDialogueIndex() const;
        private:
            TLevel _currentLevel;
            std::vector<std::vector<char>> _map;
            std::vector<std::string> _dialogue;
            size_t _dialogueIndex;
    };
}

#endif /* !LEVELMANAGER_HPP_ */
