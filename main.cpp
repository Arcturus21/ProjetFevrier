#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>

#include "Plateau.h"
#include "GameManager.h"

using namespace std;

void connexionClient()
{
    int age=15, jourNaissance=30, moisNaissance=4, anneeNaissance=1997;
    string recu;
    sf::Packet packet;

    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect("127.0.0.1", 53000, sf::seconds(3));
    if (status != sf::Socket::Done)
    {
        // erreur...
        cout << "ERREUR CONNEXION CLIENT" << endl;
    }
    else
    {
        cout << "CONNEXION CLIENT" << endl;
    }

    packet << age << jourNaissance << moisNaissance << anneeNaissance;
    socket.send(packet);
    socket.receive(packet);
    if(packet >> age)
    {
        cout << age << endl;
    }
    else
        cout << " erreur " << endl;
}

void connexionServeur()
{
    ///SERVEUR
    sf::TcpListener listener;

    // lie l'écouteur à un port
    cout << "listening" << endl;
    if (listener.listen(53000) != sf::Socket::Done)
    {
        // erreur...
        cout << "ERREUR LECTURE PORT" << endl;
    }

    // accepte une nouvelle connexion
    sf::TcpSocket client;
    sf::Packet packet;
    int age, jour, mois, annee;
    string r="recu";
    if (listener.accept(client) != sf::Socket::Done)
    {
        // erreur...
        cout << "ERREUR CONNEXION SERVEUR" << endl;
        return;
    }
    else
    {
        cout << "CONNEXION SERVEUR" << endl;
    }

    // utilisez la socket "client" pour communiquer avec le client connecté,
    // et continuez à attendre de nouvelles connexions avec l'écouteur
    client.receive(packet);
    packet >> age >> jour >> mois >> annee;
    cout << "Client agé de " << age << " ans, né le " << jour << '/' << mois << '/' << annee << endl;
    packet << age;
    client.send(packet);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(TAILLE_CASE*NB_CASE, TAILLE_CASE*NB_CASE), "EXTRA DAME 2018 PLUS ULTRA");

    GameManager gm;

    gm.Play(window);

    //cout << sf::IpAddress::getLocalAddress() << endl << sf::IpAddress::getPublicAddress() << endl;
    return 0;

    ///CLIENT
    sf::Thread threadClient(&connexionClient);
    sf::Thread threadServeur(&connexionServeur);

    threadServeur.launch();
    threadClient.launch();

    threadClient.wait();
    threadServeur.terminate();

    return 0;
}
