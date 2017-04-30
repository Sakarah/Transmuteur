#include "boardsimulator.h"
#include "prologin.hh"
#include "prototypes.h"
#include <algorithm>

BoardSimulator::BoardSimulator(int idPlayer)
{
    _playerId = idPlayer;

    for(int i = 0 ; i < NB_TYPE_CASES ; i++) _typeCount[i] = 0;

    for(int l = 0 ; l < TAILLE_ETABLI ; l++)
    {
        for(int c = 0 ; c < TAILLE_ETABLI ; c++)
        {
            case_type t = type_case(position{l,c}, idPlayer);
            _typeCount[t]++;
            _board[l][c] = t;
        }
    }
}

BoardSimulator BoardSimulator::copy()
{
    BoardSimulator newSim;
    newSim._playerId = _playerId;

    for(int i = 0 ; i < NB_TYPE_CASES ; i++) newSim._typeCount[i] = _typeCount[i];

    for(int l = 0 ; l < TAILLE_ETABLI ; l++)
    {
        for(int c = 0 ; c < TAILLE_ETABLI ; c++) newSim._board[l][c] = _board[l][c];
    }

    return newSim;
}

void BoardSimulator::swap(BoardSimulator& other)
{
    std::swap(_playerId, other._playerId);
    std::swap(_board, other._board);
    std::swap(_typeCount, other._typeCount);
}

int BoardSimulator::hash() const
{
    int hash = 0;
    for(int l = 0 ; l < TAILLE_ETABLI ; l++)
    {
        for(int c = 0 ; c < TAILLE_ETABLI ; c++)
        {
            hash <<= 1;
            hash ^= _board[l][c];
        }
    }
    return hash;
}

case_type BoardSimulator::typeCase(position pos) const { return _board[pos.ligne][pos.colonne]; }

void BoardSimulator::putSample(position pos1, position pos2, echantillon ech)
{
    // Precond: (pos1,pos2,ech) est une position d'échantillon valide
    setCase(pos1, ech.element1);
    setCase(pos2, ech.element2);
}

void BoardSimulator::setCase(position pos, case_type type)
{
    _typeCount[typeCase(pos)]--;
    _board[pos.ligne][pos.colonne] = type;
    _typeCount[typeCase(pos)]++;
}

void BoardSimulator::wipeout()
{
    for(int l = 0 ; l < TAILLE_ETABLI ; l++)
    {
        for(int c = 0 ; c < TAILLE_ETABLI ; c++)
        {
            setCase(position{l,c}, VIDE);
        }
    }
}

void BoardSimulator::fillRegion(std::vector<position>& reg, case_type type)
{
    for(position regP : reg) setCase(regP, type);
}

std::vector<std::vector<position>> BoardSimulator::getRegions() const
{
    bool visitedPos[TAILLE_ETABLI][TAILLE_ETABLI];
    for(int x = 0 ; x < TAILLE_ETABLI ; x++)
    {
        for(int y = 0 ; y < TAILLE_ETABLI ; y++)
            visitedPos[x][y] = false;
    }

    std::vector<std::vector<position>> regions;
    for(int x = 0 ; x < TAILLE_ETABLI ; x++)
    {
        for(int y = 0 ; y < TAILLE_ETABLI ; y++)
        {
            if(visitedPos[x][y]) continue;
            visitedPos[x][y] = true;
            if(_board[x][y] == VIDE) continue;

            std::vector<position> region;
            region.push_back(position{x,y});
            case_type regionType = _board[x][y];
            unsigned posLast = 0;

            while(posLast < region.size())
            {
                position curPos = region[posLast];
                for(position diffPos : DIFF_POS)
                {
                    position newPos = curPos + diffPos;
                    if(!isValid(newPos)) continue;
                    if(typeCase(newPos) != regionType) continue;
                    if(visitedPos[newPos.ligne][newPos.colonne]) continue;
                    visitedPos[newPos.ligne][newPos.colonne] = true;
                    region.push_back(newPos);
                }
                posLast++;
            }

            regions.push_back(region);
        }
    }

    return regions;
}

std::vector<position> BoardSimulator::regionOf(position initPos) const
{
    bool visitedPos[TAILLE_ETABLI][TAILLE_ETABLI];
    for(int x = 0 ; x < TAILLE_ETABLI ; x++)
    {
        for(int y = 0 ; y < TAILLE_ETABLI ; y++)
            visitedPos[x][y] = false;
    }

    std::vector<position> region;
    region.push_back(initPos);
    visitedPos[initPos.ligne][initPos.colonne] = true;
    case_type regionType = typeCase(initPos);
    unsigned posLast = 0;

    while(posLast < region.size())
    {
        position curPos = region[posLast];
        for(position diffPos : DIFF_POS)
        {
            position newPos = curPos + diffPos;
            if(!isValid(newPos)) continue;
            if(typeCase(newPos) != regionType) continue;
            if(visitedPos[newPos.ligne][newPos.colonne]) continue;
            visitedPos[newPos.ligne][newPos.colonne] = true;
            region.push_back(newPos);
        }
        posLast++;
    }

    return region;
}

bool BoardSimulator::isValidSamplePos(position pos1, position pos2, echantillon ech) const
{
    // Precond : pos1 et pos2 sont contigues et valides
    if(typeCase(pos1) != VIDE || typeCase(pos2) != VIDE) return false;

    if(typeCount(ech.element1) == 0 && typeCount(ech.element2) == 0) return true;

    for(position diffPos : DIFF_POS)
    {
        position neigh1 = pos1 + diffPos;
        if(isValid(neigh1) && typeCase(neigh1) == ech.element1) return true;

        position neigh2 = pos2 + diffPos;
        if(isValid(neigh2) && typeCase(neigh2) == ech.element2) return true;
    }

    return false;
}

bool nearCenterCmp(const position_echantillon& a, const position_echantillon& b)
{
    const position twoTimesCenter = position{TAILLE_ETABLI, TAILLE_ETABLI};

    position aToCenter = a.pos1 + a.pos2 - twoTimesCenter;
    int aDistSqrCenter = (aToCenter.colonne*aToCenter.colonne)+(aToCenter.ligne*aToCenter.ligne);

    position bToCenter = b.pos1 + b.pos2 - twoTimesCenter;
    int bDistSqrCenter = (bToCenter.colonne*bToCenter.colonne)+(bToCenter.ligne*bToCenter.ligne);

    return aDistSqrCenter < bDistSqrCenter;
}

std::vector<position_echantillon> BoardSimulator::possibleSamplePos(echantillon ech) const
{
    std::vector<position_echantillon> result;
    for(int l = 0 ; l < TAILLE_ETABLI ; l++)
    {
        for(int c = 0 ; c < TAILLE_ETABLI ; c++)
        {
            position pos1 = position{l,c};
            for(position diffPos : DIFF_POS)
            {
                position pos2 = pos1+diffPos;
                if(!isValid(pos2)) continue;
                if(isValidSamplePos(pos1,pos2,ech))
                {
                    result.push_back(position_echantillon{pos1,pos2});
                }
            }
        }
    }

    std::sort(result.begin(), result.end(), nearCenterCmp); // Favorise légèrement les cases du centre

    return result;
}

std::pair<int,int> BoardSimulator::boardPotential() const
{
    int potentialGold = 0;
    int potentialCatalyser = 0;
    for(std::vector<position> region : getRegions())
    {
        // On s'attend à ce qu'une case sur le plateau puisse rapporter par la suite.
        potentialGold += regionGoldValue(region.size() + (isRegionIsolated(region) ? 0 : 1), typeCase(region[0]));
        potentialCatalyser += regionCatalyserValue(region.size(), typeCase(region[0]));
    }
    return std::make_pair(potentialGold, potentialCatalyser);
}

bool BoardSimulator::isRegionIsolated(std::vector<position>& reg) const
{
    for(position pos : reg)
    {
        for(position diffPos : DIFF_POS)
        {
            position testPos = pos + diffPos;
            if(!isValid(testPos)) continue;
            if(typeCase(testPos) == VIDE) return false;
        }
    }

    return true;
}

int BoardSimulator::typeCount(case_type type) const { return _typeCount[type]; }

void BoardSimulator::printBoard() const
{
    for(int x = 0 ; x < TAILLE_ETABLI ; x++)
    {
        for(int y = 0 ; y < TAILLE_ETABLI ; y++)
        {
            printf("%d", _board[x][y]);
        }
        printf("\n");
    }
}
