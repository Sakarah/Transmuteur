#include "prototypes.h"

bool isValid(position pos)
{
    if(pos.ligne < 0 || pos.ligne >= TAILLE_ETABLI) return false;
    if(pos.colonne < 0 || pos.colonne >= TAILLE_ETABLI) return false;
    return true;
}

int regionGoldValue(int size, case_type type)
{
    if(propriete_case_type(type) == TRANSMUTABLE_OR)
    {
        return quantite_transmutation_or(size);
    }
    else if(propriete_case_type(type) == TRANSMUTABLE_CATALYSEUR)
    {
        return quantite_transmutation_catalyseur_or(size);
    }
    else return 0;
}

int regionCatalyserValue(int size, case_type type)
{
    if(propriete_case_type(type) == TRANSMUTABLE_CATALYSEUR) return quantite_transmutation_catalyseur(size);
    else return 0;
}

position operator+(position p1, position p2)
{
    return position{p1.ligne+p2.ligne, p1.colonne+p2.colonne};
}
position operator-(position p1, position p2)
{
    return position{p1.ligne-p2.ligne, p1.colonne-p2.colonne};
}
