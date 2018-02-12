#include "GameManager.h"

GameManager::GameManager()
{
    //ctor
    for(int i=0;i<10;i++)       ///INVERSER
    {
        for(int j=0+i%2;j<4;j+=2)
        {
            _listPionsBlancs.push_back(new Pion(i,j,TAILLE_CASE,sf::Color::White));
        }
        for(int j=NB_CASE-1-((i+1)%2);j>=NB_CASE-4;j-=2)
        {
            _listPionsNoirs.push_back(new Pion(i,j,TAILLE_CASE,sf::Color::Black));
        }
    }
}

GameManager::~GameManager()
{
    //dtor
    for(int i=0;i<_listPionsBlancs.size();i++)       ///INVERSER
    {
        delete _listPionsBlancs[i];
    }
    for(int i=0;i<_listPionsNoirs.size();i++)       ///INVERSER
    {
        delete _listPionsNoirs[i];
    }
}

void GameManager::AfficherPions(sf::RenderTarget& target)
{
    for(int i=0;i<_listPionsBlancs.size();i++)
    {
        if(_listPionsBlancs[i]==NULL)
            continue;
        target.draw(*(_listPionsBlancs[i]));
    }
    for(int i=0;i<_listPionsNoirs.size();i++)
    {
        if(_listPionsNoirs[i]==NULL)
            continue;
        target.draw(*(_listPionsNoirs[i]));
    }
}
