#include "GameManager.h"

GameManager::GameManager(): _joueurActuel(1), _etatTour(selectionPion),_pionSelectionne(NULL),
    _casePionOrigine(NULL),_masqueCaseActuel(NULL),_listCaseAtteignable(NULL),_masqueCaseSelectionnable(NULL)
{
    //ctor
    Pion* pTemp=NULL;
    Case* cTemp=NULL;
    for(int i=0; i<10; i++) ///Initialise les colonnes du tableau (abcisses)
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
    delete _masqueCaseSelectionnable;
    delete _masqueCaseActuel;
    delete _listCaseAtteignable;
    for(unsigned int i=0; i<_listPionsBlancs.size(); i++)
    {
        delete _listPionsBlancs[i];
    }
    for(unsigned int i=0; i<_listPionsNoirs.size(); i++)
    {
        delete _listPionsNoirs[i];
    }
}

void GameManager::Play(sf::RenderWindow& window)
{
    _etatTour=selectionPion;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::MouseButtonReleased)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    int indX=(int)(event.mouseButton.x/TAILLE_CASE), indY=(int)(event.mouseButton.y/TAILLE_CASE);
                    /*if(Selection(event.mouseButton.x, event.mouseButton.y))
                    {
                        if(_etatTour==selectionPion)
                        {
                            SetCaseAtteignable(_pionSelectionne->GetCaseDeplacement());
                            SetEtat(selectionCase);
                        }
                        else if(_etatTour==selectionCase)
                        {
                            SetEtat(deplacementPion);
                        }
                    }*/
                    if(_etatTour==selectionPion)
                    {
                        if(SelectionnerPion(indX, indY))
                        {
                            SetCaseAtteignable(_pionSelectionne->GetCaseDeplacement());
                            SetEtat(selectionCase);
                        }
                    }
                    else if(_etatTour==selectionCase)
                    {
                        if(SelectionnerCase(indX, indY))
                        {
                            SetEtat(deplacementPion);

                            int distanceCaseX = indX-_casePionOrigine->GetIndX();
                            int distanceCaseY = indY-_casePionOrigine->GetIndY();
                            if(indX>=2 || indX<=-2)
                            {
                                Manger(indX-distanceCaseX/2,indY-distanceCaseY/2);
                            }

                            NextTurn();
                        }
                    }
                }
            }
        }

        window.clear();

        window.draw(_plateau);
        AfficherPions(window);

        window.display();
    }
}

void GameManager::AfficherPions(sf::RenderTarget& target)
{
    if(_masqueCaseActuel!=NULL)
        target.draw(*_masqueCaseActuel);
    for(unsigned int i=0; i<_listPionsBlancs.size(); i++)
    {
        if(_listPionsBlancs[i]==NULL || !_listPionsBlancs[i]->IsAlive())
            continue;
        target.draw(*(_listPionsBlancs[i]));
    }
    for(unsigned int i=0; i<_listPionsNoirs.size(); i++)
    {
        if(_listPionsNoirs[i]==NULL || !_listPionsNoirs[i]->IsAlive())
            continue;
        target.draw(*(_listPionsNoirs[i]));
    }
    if(_masqueCaseSelectionnable!=NULL)
        target.draw(*_masqueCaseSelectionnable);
}

void GameManager::NextTurn()
{
    _pionSelectionne=NULL;
    delete _listCaseAtteignable;
    _listCaseAtteignable=NULL;
    delete _masqueCaseActuel;
    _masqueCaseActuel=NULL;
    delete _masqueCaseSelectionnable;
    _masqueCaseSelectionnable=NULL;

    if(GameOver())
    {
        _etatTour=gameOver;
        return;
    }

    _joueurActuel = (_joueurActuel)%2+1;
    _etatTour=selectionPion;
}

bool GameManager::Selection(int mousePosX, int mousePosY)
{
    int indX=(int)(mousePosX/TAILLE_CASE), indY=(int)(mousePosY/TAILLE_CASE);

    if(indX<0||indY<0||indX>=NB_CASE||indY>=NB_CASE)
        return false;

    EtatCase etatCase = _plateau.GetCase(indX,indY)->GetEtatCase();
    if(_etatTour==selectionPion && etatCase!=vide)
        return SelectionnerPion(indX, indY);
    else if(_etatTour==selectionCase && etatCase==vide)
        return SelectionnerCase(indX, indY);
    else if(_etatTour==selectionCase && ((etatCase==pionBlanc && _pionSelectionne->GetColor()==sf::Color::White)
                                         || (etatCase==pionNoir && _pionSelectionne->GetColor()==sf::Color::Black)))
    {
        SetEtat(selectionPion);
        return SelectionnerCase(indX, indY);
    }

    return false;
}

bool GameManager::SelectionnerPion(int indX, int indY)
{
    if(indX<0||indY<0||indX>=NB_CASE||indY>=NB_CASE)
        return false;

    EtatCase etatCaseCorrect;
    if(_joueurActuel==1)
        etatCaseCorrect=pionBlanc;
    else
        etatCaseCorrect=pionNoir;

    Case* caseCliquee=_plateau.GetCase(indX,indY);
    if(caseCliquee==NULL || caseCliquee->GetEtatCase()!=etatCaseCorrect)
        if(_plateau.GetCase(indX,indY)->GetEtatCase()!=etatCaseCorrect)
            return false;

    _pionSelectionne=caseCliquee->GetPion();
    if(_pionSelectionne==NULL)
        return false;
    _casePionOrigine=caseCliquee;

    _masqueCaseActuel=new sf::VertexArray(sf::Quads,4);
    (*_masqueCaseActuel)[0].position=_casePionOrigine->GetPosition();
    (*_masqueCaseActuel)[0].color=sf::Color(200,0,0,128);
    (*_masqueCaseActuel)[1].position=_casePionOrigine->GetPosition()+sf::Vector2f(TAILLE_CASE,0);
    (*_masqueCaseActuel)[1].color=sf::Color(200,0,0,128);
    (*_masqueCaseActuel)[2].position=_casePionOrigine->GetPosition()+sf::Vector2f(TAILLE_CASE,TAILLE_CASE);
    (*_masqueCaseActuel)[2].color=sf::Color(200,0,0,128);
    (*_masqueCaseActuel)[3].position=_casePionOrigine->GetPosition()+sf::Vector2f(0,TAILLE_CASE);
    (*_masqueCaseActuel)[3].color=sf::Color(200,0,0,128);

    return true;
}

void GameManager::SetCaseAtteignable(std::vector<Case*>* listCaseAtteignable)
{
    if(listCaseAtteignable==NULL)
        return;
    _listCaseAtteignable=listCaseAtteignable;

    _masqueCaseSelectionnable = new sf::VertexArray(sf::Quads,4*_listCaseAtteignable->size());
    for(unsigned int i=0, j=0; i<_masqueCaseSelectionnable->getVertexCount(); i+=4,j++)
    {
        (*_masqueCaseSelectionnable)[i].position=(*_listCaseAtteignable)[j]->GetPosition();
        (*_masqueCaseSelectionnable)[i].color=sf::Color(0,200,0,128);
        (*_masqueCaseSelectionnable)[i+1].position=(*_listCaseAtteignable)[j]->GetPosition()+sf::Vector2f(TAILLE_CASE,0);
        (*_masqueCaseSelectionnable)[i+1].color=sf::Color(0,200,0,128);
        (*_masqueCaseSelectionnable)[i+2].position=(*_listCaseAtteignable)[j]->GetPosition()+sf::Vector2f(TAILLE_CASE,TAILLE_CASE);
        (*_masqueCaseSelectionnable)[i+2].color=sf::Color(0,200,0,128);
        (*_masqueCaseSelectionnable)[i+3].position=(*_listCaseAtteignable)[j]->GetPosition()+sf::Vector2f(0,TAILLE_CASE);
        (*_masqueCaseSelectionnable)[i+3].color=sf::Color(0,200,0,128);
    }
}

bool GameManager::SelectionnerCase(int indX, int indY)
{
    if(indX<0||indY<0||indX>=NB_CASE||indY>=NB_CASE)
        return false;

    for(unsigned int i=0; i<_listCaseAtteignable->size();i++)
    {
        if(_plateau.GetCase(indX,indY)==(*_listCaseAtteignable)[i])
        {
            _pionSelectionne->setCase(indX,indY);

            SetEtat(deplacementPion);

            return true;
        }
    }

    return false;
}

bool GameManager::Manger(int indX, int indY)
{
    if(indX<0||indY<0||indX>=NB_CASE||indY>=NB_CASE)
        return false;

    Case* c=_plateau.GetCase(indX, indY);
    if(c==NULL)
        return false;
    Pion* p=c->GetPion();
    c->SetPion(NULL);
    p->SetAlive(false);

    return true;
}

bool GameManager::SetEtat(EtatTour etat)
{
    if(_etatTour==etat)
        return true;

    if(etat==selectionCase&&_etatTour==selectionPion&&_pionSelectionne!=NULL)
    {
        _etatTour=etat;
        return true;
    }

    if(etat==selectionPion && _etatTour==selectionCase)
    {
        _pionSelectionne=NULL;
        delete _masqueCaseSelectionnable;
        _masqueCaseSelectionnable=NULL;
        delete _masqueCaseActuel;
        _masqueCaseActuel=NULL;
        delete _listCaseAtteignable;
        _listCaseAtteignable=NULL;

        _etatTour=etat;

        return true;
    }

    if(etat==deplacementPion&&_etatTour==selectionCase&&_listCaseAtteignable!=NULL&&_listCaseAtteignable->size()>0)
    {
        _etatTour=etat;
        return true;
    }

    if(etat==gameOver)
    {
        _etatTour=etat;
        return true;
    }
    return false;
}

int GameManager::GameOver()
{
    int retour=1;
    for(unsigned int i=0; i<_listPionsBlancs.size(); i++)
    {
        if(_listPionsBlancs[i]->IsAlive())
            retour=0;
    }
    if(retour==1)
        return retour;
    retour=2;
    for(unsigned int i=0; i<_listPionsNoirs.size(); i++)
    {
        if(_listPionsNoirs[i]->IsAlive())
            retour=0;
    }
    return 0;
}
