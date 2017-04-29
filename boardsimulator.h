#ifndef BOARDSIMULATOR_H
#define BOARDSIMULATOR_H

#include "prologin.hh"

class BoardSimulator
{
public:
    BoardSimulator() = default;
    BoardSimulator(int idPlayer);
    void swap(BoardSimulator&);

    case_type typeCase(position) const;
    void putSample(position, position, echantillon);
    void setCase(position, case_type);
    void wipeout();

    void fillRegion(std::vector<position>&, case_type);
    std::vector<std::vector<position>> getRegions() const;

    bool isValidSamplePos(position, position, echantillon) const;
    std::vector<position_echantillon> possibleSamplePos(echantillon) const;

    int boardPotential() const;
    int typeCount(case_type) const;

private:
    int _playerId;
    case_type _board[TAILLE_ETABLI][TAILLE_ETABLI];
    int _typeCount[NB_TYPE_CASES];
};

#endif // BOARDSIMULATOR_H
