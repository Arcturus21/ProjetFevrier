#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>

#include "Pion.h"
#include "Case.h"
#include "Plateau.h"

class GameManager
{
    public:
        GameManager();
        virtual ~GameManager();

        void NextTurn();
        void SelectionnerPion();
        void SelectionnerCase();

        void AfficherPions(sf::RenderTarget& target);

    protected:

    private:
        int _joueurActuel;

        Plateau _plateau;

        std::vector<Pion*> _listPionsBlancs;
        std::vector<Pion*> _listPionsNoirs;

        Pion* _pionSelectionne;
        Case* _casePionOrigine;
        std::vector<Case*>* _listCasesDisponibles;
        sf::VertexArray* _masqueCaseSelectionnable;
};

#endif // GAMEMANAGER_H
