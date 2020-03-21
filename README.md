# Codenames

Petit projet fait sous confinement pour pouvoir jouer au jeu Codenames via ordinateur (et en particulier à distance). Les programmes ne fait pas respecter les règles du jeu (elles sont disponibles en ligne si jamais), mais sert juste de plateau virtuel.

## Utilisation

 - Télecharger les 2 programmes Codenames-client et Codenames-serveur (ou les compiler)
 - Un espion lance le programme Codenames-serveur pour créer la partie. Si les autres espions ne sont pas au même endroit, je recommande un parteg d'écran (je réflechis à un moyen de changer ça)
 - Il récupère son adresse IP locale (si les autres joueurs sont sur le même réseau : même box, même routeur, même hotspot ...) ou externe dans le menu pour la donner aux agents
 - Chaque agent/groupe d'agents lance le programme Codenames-client, entre l'adresse IP fournie par le serveur et se connecte
 - Il suffit ensuite un agent de cliquer sur une carte pour tenter de deviner un nom
 
Chaque carte qui aura été devinée sera masquée par sa couleur, il suffit de passer la souris dessus pour voir le mot<br>
Par défaut, le serveur utilise le port 8081 pour les connexions. Si il n'est pas possible de se connecter, vérifie que le routeur ne bloque pas ce port<br> 
Il est possible de rejoindre une partie en cours, le plateau contiendra déjà les mots devinés (pratique en cas d'internet capricieux)<br>
Dans le menu du serveur, l'option Nouvelle Partie permet de recréer un plateau de jeu et l'envoie automatiquement à tous les personnes connectées<br>
En cas de déconnexions, les clients tentent de se reconnecter toutes les 10s. Il peut donc être bien d'attendre quelques dizaines de secondes avant de relancer le programme en cas de problème

## Compilation

Ce projet dépend de Qt5, il faut donc que au moins `qt5-base` soit présent

### Linux 

```
git clone https://github.com/Itrimel/Codenames.git  
mkdir build  
cd build  
qmake ../Codenames  
make 
```
  
 ### Windows
 
 J'ai pas essayé, mais j'imagine qu'avec la suite Qt Creator il doit être possible de faire quelquechose

## To-do liste pour des améliorations futures

 - [ ] Faire en sorte que le serveur et les clients puissent faire espion ou agent indifférement
 - [ ] Meilleure gestion des erreurs de connexion
 - [ ] Ajouter un système de mot de passe pour limiter le nombre de personnes pouvant se connecter en tant qu'agent et/ou espion
 - [ ] Créer un client tournant sur Nintendo Switch
