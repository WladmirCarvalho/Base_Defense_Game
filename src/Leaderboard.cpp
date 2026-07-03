#include "../include/Leaderboard.hpp"
#include <fstream>
#include <algorithm>

Leaderboard::Leaderboard(const std::string& path) : filePath(path) { load(); }

void Leaderboard::load() {
    entries.clear();
    std::ifstream file(filePath);
    if (!file.is_open()) return;

    std::string name;
    int score;
    bool isWin;
    while (file >> name >> score >> isWin) {
        entries.push_back({name, score, isWin});
    }
}

void Leaderboard::save() const {
    std::ofstream file(filePath);
    for (const auto& entry : entries) {
        file << entry.name << " " << entry.score << " " << entry.isWin << "\n";
    }
}

bool Leaderboard::qualifies(int score, bool isWin) const {
    if (entries.size() < MAX_ENTRIES) return true;
    
    const auto& last = entries.back();
    if (isWin && !last.isWin) return true; 
    if (isWin && last.isWin) return score < last.score; 
    if (!isWin && !last.isWin) return score > last.score; 
    
    return false; 
}

void Leaderboard::addScore(const std::string& name, int score, bool isWin) {
    entries.push_back({name, score, isWin});
    
    std::sort(entries.begin(), entries.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        if (a.isWin != b.isWin) return a.isWin; 
        if (a.isWin) return a.score < b.score;  
        return a.score > b.score;               
    });
    
    if (entries.size() > MAX_ENTRIES) entries.resize(MAX_ENTRIES);
    save();
}

const std::vector<ScoreEntry>& Leaderboard::getEntries() const { return entries; }