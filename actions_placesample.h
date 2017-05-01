#ifndef PLACESAMPLE_H
#define PLACESAMPLE_H

#include "actions_action.h"

/// Implémente une action de pose d'échantillon
class PlaceSample : public Action
{
public:
    PlaceSample(bool me, position pos1, position pos2, echantillon ech);
    void simulate(GameSimulator&) override;
    void undo(GameSimulator&) override;
    void execute() override;
    void debugPrint() override;
private:
    bool _me;
    position _pos1;
    position _pos2;
    echantillon _ech;
};

#endif // PLACESAMPLE_H
