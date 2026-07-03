#pragma once
#include <string>
#include <vector>

struct ScoreEntry {
    std::string name;
    int score;
    bool isWin; // <-- Aqui está a variável que faltava!
};

class Leaderboard {
private:
    std::vector<ScoreEntry> entries;
    std::string filePath;
    static const size_t MAX_ENTRIES = 10;

    void load();

public:
    Leaderboard(const std::string& path);
    void save() const;
    bool qualifies(int score, bool isWin) const; // Modificado
    void addScore(const std::string& name, int score, bool isWin); // Modificado
    const std::vector<ScoreEntry>& getEntries() const;
};