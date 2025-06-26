/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** LevelManager
*/

#include "LevelManager.hpp"

namespace triad
{
    LevelManager::LevelManager()
        : _currentLevel(TLevel::LEVEL1), _currentDifficulty(TDifficulty::EASY), _map()
    {
    }

    LevelManager::~LevelManager()
    {
    }

    void LevelManager::LoadLevel()
    {
        std::ifstream mapFile;
        std::string line;
        std::string fileName;

        switch (_currentLevel) {
            case TLevel::LEVEL1: fileName = "maps/level_1.txt"; break;
            case TLevel::LEVEL2: fileName = "maps/level_2.txt"; break;
            case TLevel::LEVEL3: fileName = "maps/level_3.txt"; break;
            case TLevel::LEVEL4: fileName = "maps/level_4.txt"; break;
            case TLevel::LEVEL5: fileName = "maps/level_5.txt"; break;
            default: throw Error("Unknown level");
        }
        _map.clear();
        mapFile.open(fileName);
        if (!mapFile.is_open()) {
            throw Error("Failed to open level file: " + fileName);
        }
        while (std::getline(mapFile, line)) {
            std::vector<char> row(line.begin(), line.end());
            _map.push_back(row);
        }
        mapFile.close();
    }

    void LevelManager::NextLevel()
    {
        switch (_currentLevel) {
            case TLevel::LEVEL1:
                _currentLevel = TLevel::LEVEL2;
                break;
            case TLevel::LEVEL2:
                _currentLevel = TLevel::LEVEL3;
                break;
            case TLevel::LEVEL3:
                _currentLevel = TLevel::LEVEL4;
                break;
            case TLevel::LEVEL4:
                _currentLevel = TLevel::LEVEL5;
                break;
            case TLevel::LEVEL5:
                break;
            default: break;
        }
        LoadLevel();
    }

    TLevel LevelManager::GetLevel() const
    {
        return _currentLevel;
    }

    const std::vector<std::vector<char>> &LevelManager::GetMap() const
    {
        return _map;
    }

    void LevelManager::ResetLevel()
    {
        LoadLevel();
    }

    void LevelManager::StartDialogue()
    {
        _dialogueIndex = 0;
        _dialogue.clear();
        switch (_currentLevel) {
            case TLevel::LEVEL1:
                _dialogue = {"Welcome to the first level!", "Prepare for your adventure!"};
                break;
            case TLevel::LEVEL2:
                _dialogue = {"You have reached the second level!", "Keep going!"};
                break;
            case TLevel::LEVEL3:
                _dialogue = {"Third level, you're doing great!", "Stay focused!"};
                break;
            case TLevel::LEVEL4:
                _dialogue = {"Almost there, fourth level!", "Don't give up!"};
                break;
            case TLevel::LEVEL5:
                _dialogue = {"Final level, make it count!", "Victory is within reach!"};
                break;
            default: break;
        }
    }

    bool LevelManager::IsDialogueFinished() const
    {
        return _dialogueIndex >= _dialogue.size();
    }

    const std::string &LevelManager::GetDialogue() const
    {
        static const std::string empty = "";
        if (_dialogue.empty() || _dialogueIndex >= _dialogue.size()) {
            return empty;
        }
        return _dialogue[_dialogueIndex];
    }

    void LevelManager::NextDialogue()
    {
        if (!IsDialogueFinished()) {
            _dialogueIndex++;
        }
    }

    size_t LevelManager::GetDialogueIndex() const
    {
        return _dialogueIndex;
    }

    void LevelManager::SetLevel(TLevel level)
    {
        _currentLevel = level;
        LoadLevel();
    }

    void LevelManager::SetDifficulty(TDifficulty difficulty)
    {
        _currentDifficulty = difficulty;
    }

    TDifficulty LevelManager::GetDifficulty() const
    {
        return _currentDifficulty;
    }
}
