#include "../include/Leaderboard.hpp"
#include <fstream>
#include <algorithm>

Leaderboard::Leaderboard(const std::string& path) : filePath(path) {
    load();
}

void Leaderboard::load() {
    entries.clear();
    std::ifstream file(filePath);
    if (!file.is_open()) return;

    std::string name;
    int score;
    while (file >> name >> score) {
        entries.push_back({name, score});
    }
}

void Leaderboard::save() const {
    std::ofstream file(filePath);
    for (const auto& entry : entries) {
        file << entry.name << " " << entry.score << "\n";
    }
}

bool Leaderboard::qualifies(int score) const {
    if (entries.size() < MAX_ENTRIES) return true;
    return score > entries.back().score;
}

void Leaderboard::addScore(const std::string& name, int score) {
    entries.push_back({name, score});
    std::sort(entries.begin(), entries.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return a.score > b.score;
    });
    if (entries.size() > MAX_ENTRIES) entries.resize(MAX_ENTRIES);
    save();
}

const std::vector<ScoreEntry>& Leaderboard::getEntries() const {
    return entries;
}
