#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>

#include "Pion.h"
#include "Case.h"
#include "Plateau.h"

typedef enum
{
    none,
    selectionPion,
    selectionCase,
    deplacementPion,
    finTour,
    gameOver
}EtatTour;

class GameManager
{
    public:
        GameManager();
        virtual ~GameManager();

        void Play(sf::RenderWindow& window);

    protected:

    private:

        void NextTurn();
        bool Selection(int mousePosX, int mousePosY);
        bool SelectionnerPion(int indX, int indY);
        bool SelectionnerCase(int indX, int indY);
        bool BoutDePlateau(Pion* p);

        bool Manger(int indX, int indY);

        bool SetCaseAtteignable(std::vector<Case*>* listCaseAtteignable);

        void AfficherPions(sf::RenderTarget& target);

        bool SetEtat(EtatTour etat);

        int GameOver();

        EtatTour GetActionActivated(int indX, int indY);

        bool MasqueCasePion();
        bool InitDeplacementPion();

    private:
        int _joueurActuel;
        EtatTour _etatTour;
        bool _aManger;

        Plateau _plateau;

        std::vector<Pion*> _listPionsBlancs;
        std::vector<Pion*> _listPionsNoirs;

        Pion* _pionSelectionne;
        Case* _casePionOrigine;
        sf::VertexArray* _masqueCaseActuel;
        std::vector<Case*>* _listCaseAtteignable;
        sf::VertexArray* _masqueCaseSelectionnable;
};

#endif // GAMEMANAGER_H
