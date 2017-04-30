#ifndef BOARDSIMULATOR_H
#define BOARDSIMULATOR_H

#include "prologin.hh"

class BoardSimulator
{
public:
    BoardSimulator() = default;
    BoardSimulator(int idPlayer);
    BoardSimulator copy();
    void swap(BoardSimulator&);
    int hash() const;

    case_type typeCase(position) const;
    void putSample(position, position, echantillon);
    void setCase(position, case_type);
    void wipeout();

    void fillRegion(std::vector<position>&, case_type);
    std::vector<std::vector<position>> getRegions() const;
    int regionExtension(std::vector<position>&) const;

    bool isValidSamplePos(position, position, echantillon) const;
    std::vector<position_echantillon> possibleSamplePos(echantillon) const;

    int boardPotential() const;
    int countHoles() const;
    int typeCount(case_type) const;

    void printBoard() const;

private:
    int _playerId;
    case_type _board[TAILLE_ETABLI][TAILLE_ETABLI];
    int _typeCount[NB_TYPE_CASES];
};

#endif // BOARDSIMULATOR_H
