#ifndef DAME_H
#define DAME_H

#include "Pion.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Dame : public Pion
{
    public:
        Dame(const Pion& p);
        virtual ~Dame();

        std::vector<Case*>* GetCaseDeplacement(bool manger=false);

    protected:

    private:

};

#endif // DAME_H
