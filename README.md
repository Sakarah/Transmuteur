Transmuteur est une IA codée en moins de 36 heures lors de l'édition Prologin de 2017.

Pour la tester il faut suivre les instructions de https://bitbucket.org/prologin/stechec2 pour lancer des parties sur le jeu prologin2017.

Elle se base sur une heuristique que toute simulation d'action en tant que l'IA elle-même tente de maximiser et que toute simulation d'action en tant que l'adversaire tente de minimiser.
A chaque tour on commence par simuler son propre tour puis on choisit selon le même algorithme des actions pour le tour de l'adversaire.
On essaye ensuite de lui donner le pire échantillon s'il jouait comme nous.
De plus, on regarde ce qu'il aurait fait pour pouvoir anticiper certaines destructions et récupérer les points avant.
L'IA possède de plus une détection de cycles pour éviter de rester bloquée à toujours faire la même chose.

Au classement de Prologin, cette IA est arrivée 16ème.

L'ensemble de son code à l'exception des fichiers prologin.hh, prologin.cc et Makefile (sous GPLv2) est sous license Apache 2.0.
