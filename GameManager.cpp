#include "GameManager.h"

GameManager::GameManager()
{
    //ctor
    Pion* pTemp=NULL;
    Case* cTemp=NULL;
    for(int i=0;i<10;i++)   ///Initialise les colonnes du tableau (abcisses)
    {
        for(int j=(i+1)%2; j<4; j+=2)   ///Initialise les lignes du tableau (ordonnees)
        {
            pTemp=new Pion(i,j,TAILLE_CASE,sf::Color::Black);
            pTemp->SetPlateau(_plateau);
            cTemp=_plateau.GetCase(i,j);
            if(cTemp!=NULL)
                cTemp->SetPion(pTemp);
            _listPionsNoirs.push_back(pTemp);
        }
        for(int j=NB_CASE-1-((i)%2); j>=NB_CASE-4; j-=2)
        {
            pTemp=new Pion(i,j,TAILLE_CASE,sf::Color::White);
            pTemp->SetPlateau(_plateau);
            cTemp=_plateau.GetCase(i,j);
            if(cTemp!=NULL)
                cTemp->SetPion(pTemp);
            _listPionsBlancs.push_back(pTemp);
        }
    }
}

GameManager::~GameManager()
{
    //dtor
    for(unsigned int i=0;i<_listPionsBlancs.size();i++)
    {
        delete _listPionsBlancs[i];
    }
    for(unsigned int i=0;i<_listPionsNoirs.size();i++)
    {
        delete _listPionsNoirs[i];
    }
}

void GameManager::AfficherPions(sf::RenderTarget& target)
{
    for(unsigned int i=0;i<_listPionsBlancs.size();i++)
    {
        if(_listPionsBlancs[i]==NULL)
            continue;
        target.draw(*(_listPionsBlancs[i]));
    }
    for(unsigned int i=0;i<_listPionsNoirs.size();i++)
    {
        if(_listPionsNoirs[i]==NULL)
            continue;
        target.draw(*(_listPionsNoirs[i]));
    }
}
