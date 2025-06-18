/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** CardManager
*/

#include "CardManager.hpp"

namespace triad
{
    Card::Card(int id, const std::string &name, int top, int bottom, int left, int right)
        : _id(id), _name(name), _top(top), _bottom(bottom), _left(left), _right(right) {}

    int Card::GetId() const
    {
        return _id;
    }

    const std::string &Card::GetName() const
    {
        return _name;
    }

    const int Card::GetTop() const
    {
        return _top;
    }

    const int Card::GetBottom() const
    {
        return _bottom;
    }

    const int Card::GetLeft() const
    {
        return _left;
    }

    const int Card::GetRight() const
    {
        return _right;
    }

    bool Card::LoadTexture(const std::string &path)
    {
        return _texture.loadFromFile(path);
    }

    const sf::Texture &Card::GetTexture() const
    {
        return _texture;
    }

    CardManager::CardManager() {}

    CardManager &CardManager::GetInstance()
    {
        static CardManager instance;
        return instance;
    }

    const Card &CardManager::GetCard(int id) const
    {
        auto it = _cards.find(id);
        if (it == _cards.end()) {
            throw Error("Card not found");
        }
        return *(it->second);
    }

    void CardManager::LoadCards()
    {
        _cards.emplace(1, std::make_unique<Card>(1, "ahriman", 5, 2, 2, 5));
        _cards.emplace(2, std::make_unique<Card>(2, "amaljaa", 1, 7, 1, 4));
        _cards.emplace(3, std::make_unique<Card>(3, "bombo", 3, 3, 3, 4));
        _cards.emplace(4, std::make_unique<Card>(4, "chocobo", 3, 2, 1, 7));
        _cards.emplace(5, std::make_unique<Card>(5, "coblyn", 3, 3, 4, 3));
        _cards.emplace(6, std::make_unique<Card>(6, "coeurl", 2, 2, 5, 5));
        _cards.emplace(7, std::make_unique<Card>(7, "dodo", 4, 3, 4, 2));
        _cards.emplace(8, std::make_unique<Card>(8, "goobbue", 2, 5, 2, 5));
        _cards.emplace(9, std::make_unique<Card>(9, "ixal", 6, 3, 4, 1));
        _cards.emplace(10, std::make_unique<Card>(10, "kobold", 2, 4, 6, 2));
        _cards.emplace(11, std::make_unique<Card>(11, "mandragore", 4, 5, 3, 2));
        _cards.emplace(12, std::make_unique<Card>(12, "mog", 2, 3, 7, 1));
        _cards.emplace(13, std::make_unique<Card>(13, "morbol", 5, 5, 2, 2));
        _cards.emplace(14, std::make_unique<Card>(14, "pampa", 4, 3, 3, 3));
        _cards.emplace(15, std::make_unique<Card>(15, "pudding", 2, 3, 5, 4));
        _cards.emplace(16, std::make_unique<Card>(16, "sahuagin", 4, 3, 3, 5));
        _cards.emplace(17, std::make_unique<Card>(17, "spriggan", 2, 4, 4, 3));
        _cards.emplace(18, std::make_unique<Card>(18, "sylphe", 2, 5, 4, 4));
        _cards.emplace(19, std::make_unique<Card>(19, "tataru", 7, 3, 2, 2));
        _cards.emplace(20, std::make_unique<Card>(20, "tomberry", 2, 7, 2, 2));

        for (auto &pair : _cards) {
            std::string path = "assets/sprites/" + pair.second->GetName() + ".png";
            if (!pair.second->LoadTexture(path)) {
                throw Error("Failed to load texture for card: " + pair.second->GetName());
            }
        }
    }
}
