#include "boardsimulator.h"
#include "prologin.hh"
#include "prototypes.h"

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

int BoardSimulator::regionExtension(std::vector<position>& region) const
{
    int extension = 0;
    for(position pos : region)
    {
        for(position diffPos : DIFF_POS)
        {
            position testPos = pos + diffPos;
            if(!isValid(testPos)) continue;
            if(typeCase(testPos) == VIDE) extension++;
        }
    }
    return ((extension+1) / 2) + 1; // On estime qu'on pourra occuper la moitié (arrondi sup) des espaces autour
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

    return result;
}

int BoardSimulator::boardPotential() const
{
    int potential = 0;
    for(std::vector<position> region : getRegions())
    {
        // On s'attend à ce qu'une case sur le plateau puisse rapporter par la suite.
        potential += regionGoldValue(region.size() + 2, typeCase(region[0]));
        potential += regionCatalyserValue(region.size() + 2, typeCase(region[0]));
    }
    //potential -= countHoles();
    return potential;
}

int BoardSimulator::countHoles() const
{
    int holes = 0;

    for(int x = 0 ; x < TAILLE_ETABLI ; x++)
    {
        for(int y = 0 ; y < TAILLE_ETABLI ; y++)
        {
            position pos = position{x,y};
            if(typeCase(pos) != VIDE) continue;

            bool hole = true;
            for(position diffPos : DIFF_POS)
            {
                position testPos = pos + diffPos;
                if(!isValid(testPos)) continue;
                if(typeCase(testPos) == VIDE) hole = false;
            }

            if(hole) holes++;
        }
    }

    return holes;
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
