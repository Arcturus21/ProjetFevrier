#include "GameManager.h"
#include "Dame.h"

using namespace std;

GameManager::GameManager(): _joueurActuel(1), _etatTour(selectionPion),_peutManger(false),_pionSelectionne(NULL),
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

void GameManager::Play(sf::RenderWindow& window)    ///d�roulement de la partie
{
    _etatTour=selectionPion;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)            ///Evenement de fermeture du jeu
                window.close();
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            else if(event.type == sf::Event::MouseButtonReleased)   ///Clic sur une case
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    int indX=(int)(event.mouseButton.x/TAILLE_CASE), indY=(int)(event.mouseButton.y/TAILLE_CASE);   ///Indice de la case sur laquelle on a cliqu�
                    if(_etatTour==selectionPion)    ///Si l'on doit s�lectionner un pion
                    {
                        if(SelectionnerPion(indX, indY))
                        {
                            if(SetCaseAtteignable(_pionSelectionne->GetCaseDeplacement()))
                                SetEtat(selectionCase);
                        }
                    }
                    else if(_etatTour==selectionCase)
                    {
                        if(SelectionnerCase(indX, indY))
                        {
                            SetEtat(deplacementPion);

                            if(_casePionOrigine==NULL)
                                continue;
                            int casePionMangerX=0, casePionMangerY=0;
                            if(TrouverPionManger(_casePionOrigine->GetIndX(),_casePionOrigine->GetIndY(),indX,indY,casePionMangerX,casePionMangerY))
                            {
                                Manger(casePionMangerX,casePionMangerY);
                                _peutManger=SetCaseAtteignable(_pionSelectionne->GetCaseDeplacement(true));
                                _casePionOrigine=_pionSelectionne->getCase();
                                MasqueCasePion();
                            }

                            if(_peutManger)
                            {
                                SetEtat(selectionCase);
                            }
                            else
                            {
                                if(BoutDePlateau(_pionSelectionne))
                                {
                                    Dame* d=new Dame(*_pionSelectionne);
                                    if(d->GetColor()==sf::Color::White)
                                    {
                                        for(unsigned int i=0;i<_listPionsBlancs.size();i++)
                                        {
                                            if(_listPionsBlancs[i]==_pionSelectionne)
                                            {
                                                _listPionsBlancs[i]=d;
                                                break;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        for(unsigned int i=0;i<_listPionsNoirs.size();i++)
                                        {
                                            if(_listPionsNoirs[i]==_pionSelectionne)
                                            {
                                                _listPionsNoirs[i]=d;
                                                break;
                                            }
                                        }
                                    }
                                    _pionSelectionne->getCase()->SetPion(d);
                                    _pionSelectionne=d;
                                }
                                SetEtat(finTour);
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
        if(_listPionsNoirs[i]==NULL /*|| !_listPionsNoirs[i]->IsAlive()*/)
            continue;
        target.draw(*(_listPionsNoirs[i]));
    }
    if(_masqueCaseSelectionnable!=NULL)
        target.draw(*_masqueCaseSelectionnable);
}

EtatTour GameManager::GetActionActivated(int indX, int indY)
{
    if(indX<0||indY<0||indX>=NB_CASE||indY>=NB_CASE)    ///clique en dehors du plateau -> on quitte
        return none;

    Case* c=_plateau.GetCase(indX,indY);
    if(c==NULL)         ///La case s�lectionn� est NULL = probl�me -> on quitte
        return none;

    if(c->GetPion()==NULL)      ///s'il n'y a aucun pion sur la case, c'est potentiellement une case � s�lectionn�
        return selectionCase;
    else if((c->GetPion()->GetColor()==sf::Color::White && _joueurActuel==1)       ///Si le pion sur la case est un des pions du joueur
            || (c->GetPion()->GetColor()==sf::Color::Black && _joueurActuel==2))   ///Il veut peut �tre s�lectionn� un pion
        return selectionPion;
    else
        return none;
}

bool GameManager::TrouverPionManger(int caseOrigineX, int caseOrigineY, int caseArriveX, int caseArriveY, int& casePionMangeX, int& casePionMangeY)
{
    int sensX=0,sensY=0;
    if(caseOrigineX<caseArriveX)
        sensX=1;
    else if(caseOrigineX>caseArriveX)
        sensX=-1;
    if(caseOrigineY<caseArriveY)
        sensY=1;
    else if(caseOrigineY>caseArriveY)
        sensY=-1;

    casePionMangeX=caseOrigineX+sensX;
    casePionMangeY=caseOrigineY+sensY;
    Case* cTemp=_plateau.GetCase(casePionMangeX,casePionMangeY);
    while(casePionMangeX!=caseArriveX && cTemp!=NULL)       ///Tant qu'on a pas parcouru toutes les cases et qu'on est sur le plateau
    {
        if(cTemp->GetEtatCase()!=vide)   ///si la case sur laquelle on est contient un pion
            return true;

        casePionMangeX+=sensX;
        casePionMangeY+=sensY;
        cTemp=_plateau.GetCase(casePionMangeX,casePionMangeY);
    }
    return false;
}

void GameManager::NextTurn()
{
    _pionSelectionne=NULL;

    if(_listCaseAtteignable!=NULL)
        delete _listCaseAtteignable;
    _listCaseAtteignable=NULL;

    if(_masqueCaseActuel!=NULL)
        delete _masqueCaseActuel;
    _masqueCaseActuel=NULL;

    if(_masqueCaseSelectionnable!=NULL)
        delete _masqueCaseSelectionnable;
    _masqueCaseSelectionnable=NULL;

    if(GameOver())
    {
        _etatTour=gameOver;
        return;
    }

    _joueurActuel = (_joueurActuel)%2+1;
    SetEtat(selectionPion);
    _peutManger=false;

    for(unsigned int i=0;i<_listPionsBlancs.size();i++)
    {
        if(_listPionsBlancs[i]==NULL || !_listPionsBlancs[i]->IsAlive())
        {
            delete _listPionsBlancs[i];
            _listPionsBlancs.erase(_listPionsBlancs.begin()+i);
            i--;
        }
    }
    for(unsigned int i=0;i<_listPionsNoirs.size();i++)
    {
        if(_listPionsNoirs[i]==NULL || !_listPionsNoirs[i]->IsAlive())
        {
            delete _listPionsNoirs[i];
            _listPionsNoirs.erase(_listPionsNoirs.begin()+i);
            i--;
        }
    }
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
        return false;

    _pionSelectionne=caseCliquee->GetPion();
    if(_pionSelectionne==NULL)
        return false;

    _casePionOrigine=caseCliquee;

    return MasqueCasePion();
}

bool GameManager::MasqueCasePion()
{
    if(_pionSelectionne==NULL)
        return false;
    if(_masqueCaseActuel!=NULL)
        delete _masqueCaseActuel;
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

bool GameManager::InitDeplacementPion()
{
    if(_pionSelectionne==NULL)
        return false;
    _casePionOrigine=_pionSelectionne->getCase();
    cout << _pionSelectionne->getCase()->GetIndX() << " " << _pionSelectionne->getCase()->GetIndY() << endl;
    if(_casePionOrigine==NULL)
        return false;
    if(!MasqueCasePion())
    {
        cout << "masquecasePionFail" << endl;
        return false;
    }
    if(!SetCaseAtteignable(_pionSelectionne->GetCaseDeplacement()))
    {
        cout << "caseatteignablefail" << endl;
        return false;
    }

    return true;
}

bool GameManager::SetCaseAtteignable(std::vector<Case*>* listCaseAtteignable)
{
    if(_listCaseAtteignable!=NULL)
    {
        delete _listCaseAtteignable;
        _listCaseAtteignable=NULL;
    }

    if(_masqueCaseSelectionnable!=NULL)
    {
       delete _masqueCaseSelectionnable;
        _masqueCaseSelectionnable=NULL;
    }

    if(listCaseAtteignable==NULL || listCaseAtteignable->size()<=0)
        return false;
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
    return true;
}

bool GameManager::SelectionnerCase(int indX, int indY)
{
    if(indX<0||indY<0||indX>=NB_CASE||indY>=NB_CASE)
        return false;

    for(unsigned int i=0; i<_listCaseAtteignable->size(); i++)
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

bool GameManager::BoutDePlateau(Pion* p)
{
    if(p==NULL)
        return false;
    return (p->GetColor()==sf::Color::White && p->getCase()->GetIndY()==0)
           || (p->GetColor()==sf::Color::Black && p->getCase()->GetIndY()==NB_CASE-1);
}

bool GameManager::SetEtat(EtatTour etat)
{
    if(_etatTour==etat)
        return true;

    if(etat==gameOver)
    {
        _etatTour=etat;
        return true;
    }

    if(etat==selectionCase && _listCaseAtteignable!=NULL && _listCaseAtteignable->size()>0)
    {
        _etatTour=etat;
        return true;
    }

    if(etat==deplacementPion && _etatTour==selectionCase)
    {
        _etatTour=etat;
        return true;
    }

    if(etat==selectionCase && _etatTour==deplacementPion && _listCaseAtteignable!=NULL && _listCaseAtteignable->size()>0)
    {
        _etatTour=etat;
        return true;
    }

    if(etat==finTour && _etatTour==deplacementPion)
    {
        _etatTour=etat;
        return true;
    }

    if(etat==selectionPion && _etatTour==finTour)
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
