#ifndef WIPEOUT_H
#define WIPEOUT_H

#include "actions_action.h"

class Wipeout : public Action
{
public:
    Wipeout(bool me);
    void simulate(GameSimulator&) override;
    void undo(GameSimulator&) override;
    void execute() override;
private:
    bool _me;
    BoardSimulator _backupBoard;
};

#endif // WIPEOUT_H
