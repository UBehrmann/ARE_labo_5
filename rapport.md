<div align="justify" style="margin-right:25px;margin-left:25px">

# Laboratoire 05 - Conception d’une interface fiable <!-- omit from toc -->

## Etudiants

- Pedro Alves da Silva
- Urs Behrmann

# Table des matières

- [Table des matières](#table-des-matières)
- [Introduction](#introduction)
- [Analyse](#analyse)
  - [Plan d’adressage](#plan-dadressage)
  - [Schéma bloc de l’interface](#schéma-bloc-de-linterface)
- [Tests](#tests)
  - [Partie 1](#partie-1)
    - [Simulation](#simulation)
    - [Tests sur la carte](#tests-sur-la-carte)

# Introduction

# Analyse

## Plan d’adressage



## Schéma bloc de l’interface


# Tests

## Partie 1

### Simulation

Pour la partie 1, nous avons effectué en premier des tests avec le test bench fourni. Nous avons créé une série de commande que le CPU peut envoyer à l'interface. Nous avons testé toutes les lectures et écritures décrites dans la description du laboratoire.

| Commande       | Description                                    |
| -------------- | ---------------------------------------------- |
| `Read 0`       | Lecture de l'interface user ID                 |
| `Read 4`       | Lecture des 4 buttons                          |
| `Read 8`       | Lecture des 10 switches                        |
| `Write 12 682` | Ecriture de 682 sur les 10 LEDs (0b1010101010) |
| `Read 12`      | Lecture des 10 LEDs                            |
| `Write 16 17`  | Ecriture dans new_char [4] et init_char [0]    |
| `Read 16`      | Lecture du status                              |
| `Write 20 16`  | Ecriture dans mode_gen [4]  et delay_gen [1-0] |
| `Read 20`      | Lecture des char 1 à 4                         |
| `Read 32`      | Lecture des char 5 à 8                         |
| `Read 36`      | Lecture des char 9 à 12                        |
| `Read 40`      | Lecture des char 13 à 16                       |
| `Read 44`      | Lecture du checksum [7-0]                      |

Ici, un screenshot de l'interface de simulation:l

![Simulation](/imgs/p1_test1_0.png)

Et les chronogrammes:

![Chronogramme 1](/imgs/p1_test1_1.png)

![Chronogramme 2](/imgs/p1_test1_2.png)

![Chronogramme 3](/imgs/p1_test1_3.png)


### Tests sur la carte

Pour la partie 1, nous avons effectué les tests ci-dessous sur la carte. Tous les tests mentionnés
ont été effectués avec succès.

| Test effectué                                                                                   |
|:------------------------------------------------------------------------------------------------|
| Le programme affiche les 2 IDs attendus                                                         |
| Pression sur Key2 affiche une lecture                                                           |
| Maintient de la Key2 affiche les lectures continuellement                                       |
| Maintient de la Key2 + pression de la Key0 réinitialise le générateur à "Hello world!"          |
| Maintient de la Key2 + pression de la Key1 affiche une lecture avec une string différente       |
| Maintient de la Key2 + maintient de la Key1 affiche une seule lecture différente                |
| Set du switch 7 à 1 active le mode automatique                                                  |
| Affichage des lectures affiche les bonnes informations en cas de succès                         |
| Affichage des lectures affiche les bonnes informations en cas d'erreur                          |
| Le compteur des erreurs incrémente à chaque erreur                                              |
| Changement de fréquence est effectuée, et le nombre de strings différentes par seconde augmente |
| Changement de fréquence reflet le nombre d'erreurs perçues                                      |
| Incrémentation et décrémentation de la fréquence est effectuée correctement                     |
| Set du switch 7 à 0 désactive le mode automatique                                               |
| Pression de la Key0 lorsque le mode automatique est actif réinitialise le générateur            |