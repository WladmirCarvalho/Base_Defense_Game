#pragma once
#include <string>
#include <vector>

// Uma pontuação registrada no ranking
struct ScoreEntry {
    std::string name;
    int score;
};

// Ranking persistido em arquivo, estilo "high score" de arcade
class Leaderboard {
private:
    std::vector<ScoreEntry> entries;
    std::string filePath;
    static const size_t MAX_ENTRIES = 5;

    void load();

public:
    Leaderboard(const std::string& path);
    void save() const;
    bool qualifies(int score) const;
    void addScore(const std::string& name, int score);
    const std::vector<ScoreEntry>& getEntries() const;
};
