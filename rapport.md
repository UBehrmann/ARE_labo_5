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
  - [Code](#code)
    - [Accès mémoire](#accès-mémoire)
- [Tests](#tests)
  - [Partie 1](#partie-1)
    - [Simulation](#simulation)
    - [Tests sur la carte](#tests-sur-la-carte)
  - [Partie 2](#partie-2)
    - [Simulation](#simulation-1)
    - [Tests sur la carte](#tests-sur-la-carte-1)

# Introduction

Dans ce laboratoire, nous allons concevoir une interface permettant d'effectuer la lecture de
différentes chaînes de caractères générées par la board DE1-SoC. La génération de ces dernières
peuvent être effecutées à différentes fréquences (1Hz, 1KHz, 100KHz, 1MHz), et de façon manuelle ou
automatique.

Lors de la première partie, les lectures seront effectuées de façon non-fiable, ce qui
nous permettra de remarquer l'accès aux différents blocs de la chaîne alors que l'interface est en
train de remplacer les valeurs des blocks. La deuxième partie nous permettra d'implémenter un
méchanisme permettant d'éviter les lectures éronnées.

Afin de vérifier si une lecture correcte a eu lieu, un byte de checksum est utilisé. La lecture est
considérée comme correcte lorsque la somme des valeurs ASCII de chacun des caractères lus plus le
byte de checksum, le tout modulo 256, vaut 0.

Si une lecture n'a pas été éffectuée correctement, un message à l'écran s'affiche, et le nombre de
fois qu'une lecture erronée a été faite depuis le lancement du programme est également affiché.


# Analyse

## Plan d’adressage


| Address (offset) | Read                                | Write                             |
| ---------------- | ----------------------------------- | --------------------------------- |
| 0x00             | [31..0] Interface user ID           | reserved                          |
| 0x04             | [31..4] "0..0"; [3..0] buttons      | reserved                          |
| 0x08             | [31..10] "0..0"; [9..0] switches    | reserved                          |
| 0x0C             | [31..10] "0..0"; [9..0] leds        | [31..10] reserved; [9..0] leds    |
| 0x10             | [31..2] "0..0"; [1..0] status       | [31..5] reserved; [4] new_char;   |
|                  |                                     | [3..1] reserved; [0] init_char    |
| 0x14             | [31..5] "0..0"; [4] mode_gen;       | [31..5] reserved; [4] mode_gen;   |
|                  | [3..2] "0..0"; [1..0] delay_gen     | [3..2] reserved; [1..0] delay_gen |
| 0x18             | [31..0] "0..0";                     | [31..1] reserved; [0] save_char   |
| 0x1C             | available for new functionality     | available for new functionality   |
| 0x20             | [31..24] char_2; [23..16] char_3;   | reserved                          |
|                  | [15..8] char_4; [7..0] char_4       |                                   |
| 0x24             | [31..24] char_5; [23..16] char_6;   | reserved                          |
|                  | [15..8] char_7; [7..0] char_8       |                                   |
| 0x28             | [31..24] char_9; [23..16] char_10;  | reserved                          |
|                  | [15..8] char_11; [7..0] char_12     |                                   |
| 0x2C             | [31..24] char_13; [23..16] char_14; | reserved                          |
|                  | [15..8] char_15; [7..0] char_16     |                                   |
| 0x30             | [31..8] "0..0"; [7..0] checksum     | reserved                          |
| 0x34             | reserved                            | reserved                          |
| 0x40..0xFFC      | not used                            | not used      

## Schéma bloc de l’interface


## Code

La base de notre code est très semblable au code des laboratoires précédents. Toutefois, elle a été
adapté avec les nouvelles adresses et masques à utiliser.

L'interface utilisant le bus Avalon, les adresses qui nous intéressent commencent à `0xFF20 0000`,
soit l'adresse de base pour l'AXI lightweight HPS-to-FPGA. La zone mémoire pour notre interface
se trouve à l'offset `0x0001 0000`, et nous disposons de 64KiB de mémoire. De ce fait, notre
interface intéragit avec l'espace mémoire, offset, `0x0001 0000` à `0x0001 FFFF`.

Le code va lire l'ID du design standard à la première adresse de cet espace mémoire (offset `0x0`).

### Accès mémoire


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

Ici, un screenshot de l'interface de simulation:

![Simulation](/imgs/p1_test1_0.png)

Et les chronogrammes:

![Chronogramme 1](/imgs/p1_test1_1.png)

![Chronogramme 2](/imgs/p1_test1_2.png)

![Chronogramme 3](/imgs/p1_test1_3.png)


### Tests sur la carte

> **NOTE: ** Les tests ont été effectués de façon séquentielle. Du coup, si nous effectuons le
test "Set du switch 7 à 1 active le mode automatique", alors le mode automatique reste actif
jusqu'à-ce qu'on le désactive avec "Set du switch 7 à 0 active le mode automatique".

Pour la partie 1, nous avons effectué les tests ci-dessous sur la carte. Tous les tests mentionnés
ont été effectués avec succès.

| Test effectué                                                                                   |
|:------------------------------------------------------------------------------------------------|
| Le programme affiche les 2 IDs attendus                                                         |
| Pression de la Key0 réinitialise le générateur réinitialise le générateur à "Hello world!"      |
| Pression sur Key2 affiche une lecture                                                           |
| La lecture affiche les caractères dans le bon ordre ("Hello world!" comparé à "lleHow o!dlr")   |
| Calcul d'intégrité est correct (somme des caractères + valeur de checksum)                      |
| Maintient de la Key2 affiche la même lecture continuellement                                    |
| Maintient de la Key2 + pression de la Key0 réinitialise le générateur à "Hello world!"          |
| Maintient de la Key2 + pression de la Key1 affiche une lecture avec une string différente       |
| Maintient de la Key2 + maintient de la Key1 affiche une seule lecture différente                |
| Set du switch 7 à 1 active le mode automatique                                                  |
| Affichage des lectures affiche les bonnes informations en cas de succès                         |
| Affichage des lectures affiche les bonnes informations en cas d'erreur                          |
| Le compteur des erreurs incrémente à chaque erreur                                              |
| Incrém. de la fréq. est effectuée, et le nombre de strings différentes par seconde augmente     |
| Changement de fréquence reflet le nombre d'erreur perçues                                       |
| Décr. de la fréq., à partir de la valeur maximale jusqu'à minimale, est effectuée correctement  |
| Set du switch 7 à 0 désactive le mode automatique                                               |
| Pression de la Key0 lorsque le mode automatique est actif réinitialise le générateur            |

## Partie 2

### Simulation


### Tests sur la carte

Pour la partie 2, nous avons re-effectué les tests de la partie 1, ainsi que les tests suivants:

| Test effectué                                                                                   |
|:------------------------------------------------------------------------------------------------|
| Set du switch 7 à 1 active le mode automatique                                                  |
| Set du switch 0 à 1 active le mode fiable                                                       |
| Fréquence plus basse (1Hz) génère des mots dans la bonne fréquence                              |
| Pression sur Key2 affiche une lecture sans erreurs                                              |
| Maintient sur Key2 affiche une lecture sans erreurs                                             |
| Incrém. de la fréq. est effectuée, et le nombre de lectures incorrectes reste à 0               |
| Set du switch 0 à 0 désactive le mode fiable                                                    |
| Maintient de la Key2 affiche la même lecture continuellement                                    |
