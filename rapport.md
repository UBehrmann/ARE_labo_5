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
    - [Sauvegarde des caractères](#sauvegarde-des-caractères)
    - [Gestion d'écriture](#gestion-décriture)
    - [Synch I/O](#synch-io)
    - [Gestion de lecture](#gestion-de-lecture)
  - [Chronogramme de fonctionnement](#chronogramme-de-fonctionnement)
    - [Fonctionnement manuel](#fonctionnement-manuel)
    - [Fonctionnement automatique non-fiable](#fonctionnement-automatique-non-fiable)
    - [Fonctionnement automatique fiable](#fonctionnement-automatique-fiable)
  - [MSS](#mss)
  - [Code](#code)
    - [Accès mémoire](#accès-mémoire)
- [Tests](#tests)
  - [Partie 1](#partie-1)
    - [Simulation](#simulation)
    - [Tests sur la carte](#tests-sur-la-carte)
  - [Partie 2](#partie-2)
    - [Simulation](#simulation-1)
    - [Tests sur la carte](#tests-sur-la-carte-1)
- [Conclusion](#conclusion)
- [Code](#code-1)

# Introduction

Dans ce laboratoire, nous allons concevoir une interface permettant d'effectuer la lecture de
différentes chaînes de caractères générées par la board DE1-SoC. La génération de ces dernières
peuvent être effectués à différentes fréquences (1Hz, 1KHz, 100KHz, 1MHz), et de façon manuelle ou
automatique.

Lors de la première partie, les lectures seront effectuées de façon non-fiable, ce qui
permettra l'accès aux différents blocs de la chaîne alors que l'interface est en train de remplacer
les valeurs des blocks, et de remarquer que des erreurs de lecture risque d'arriver. La deuxième
partie nous permettra d'implémenter un méchanisme permettant d'éviter ces lectures éronnées.

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
| 0x10             | [31..2] "0..0"; [1..0] status       | [31..5] reserved; [4] new_char    |
|                  |                                     | [3..1] reserved; [0] init_char    |
| 0x14             | [31..5] "0..0"; [4] mode_gen;       | [31..5] reserved; [4] mode_gen    |
|                  | [3..2] "0..0"; [1..0] delay_gen     | [3..2] reserved; [1..0] delay_gen |
| 0x18             | [31..0] "0..0"                      | [31..1] reserved; [0] save_char   |
| 0x1C             | [31..1] "0..0"; [0] reliable        | [31..1] reserved; [0] reliable    |
| 0x20             | [31..24] char_2; [23..16] char_3;   | reserved                          |
|                  | [15..8] char_4; [7..0] char_4       |                                   |
| 0x24             | [31..24] char_5; [23..16] char_6;   | reserved                          |
|                  | [15..8] char_7; [7..0] char_8       |                                   |
| 0x28             | [31..24] char_9; [23..16] char_10;  | reserved                          |
|                  | [15..8] char_11; [7..0] char_12     |                                   |
| 0x2C             | [31..24] char_13; [23..16] char_14; | reserved                          |
|                  | [15..8] char_15; [7..0] char_16     |                                   |
| 0x30             | [31..8] "0..0"; [7..0] checksum     | reserved                          |
| 0x34 ... 0xFFFC  | [31..0] "0..0"                      | reserved                          |

## Schéma bloc de l’interface

![Schéma bloc de l'interface](/imgs/schema_bloc.drawio.svg)


### Sauvegarde des caractères

Ce bloc sert simplement à sauvegarder les caractères automatiquement générés reçus en entrée. La
seule entrée synchrone utilisée dans ce bloc est `save_s`, sortant de la MSS.

![Schéma du bloc "Sauvegarde des caractères"](/imgs/schema_save_chars.drawio.svg)

### Gestion d'écriture

Ce bloc sert à sauvegarder les données à écrire dans des différents registres. L'idée c'est qu'il s'en
charge et du "décodage d'adresses", et de l'extraction des données venant du bus Avalon. De ce fait,
au lieu de répéter le même schéma que pour le bloc "Sauvegarde des caractères", car ce bloc est
également compris de registres, on donne ici les équations pour l'enable de chacun des registres.

Pour rappel, les entrées synchrones dans ce bloc sont: `avl_writedata_i`, `avl_address_i`, et
`avl_write_i`.

- `init_char_s`: `avl_write_i AND avl_writedata_i(0) AND (avl_address_i = 4)`

- `new_char_s`: `avl_write_i AND avl_writedata_i(4) AND (avl_address_i = 4)`

- `save_char_s`: `avl_write_i AND avl_writedata_i(0) AND (avl_address_i = 6)`

- `mode_gen_s`: `avl_write_i AND avl_writedata_i(4) AND (avl_address_i = 5)`

- `led_reg_s`: `avl_writedata_i(9 downto 0)` si `avl_write_i AND (avl_address_i = 3)`

- `delay_gen_s`: `avl_writedata_i(1 downto 0)` si `avl_write_i AND (avl_address_i = 5)`

- `reliable_s`: `avl_write_i AND avl_writedata_i(0) AND (avl_address_i = 7)`


### Synch I/O

Tout comme les 2 précédents blocs, ce bloc permet également simplement de sauvegarder des données
dans des registres. Toutefois les registres dans ce bloc n'ont pas d'enable, et se content de
synchroniser les 2 entrées à chaque coup de clock.


### Gestion de lecture

Ce bloc agit comme un multiplexeur pour savoir quoi enregistrer dans le signal utilisé pour sortie
les données lues par le CPU. On utilise l'adresse reçue comme bits de sélection; 14 au total. Du
coup, dans cette section, nous décrivons ce qu'on écrit dans le signal dans chacun des cas.

Ce bloc aurait pu être amélioré, en utilisant simplement les 4 least-significant bits de l'adresse,
car nous n'avons que 13 adresses différentes à affecter, et on pourrait vérifier qu'on se trouve
bien au tout début de l'espace mémoire de l'interface (`avl_address_i(13 downto 4) = "00...0"`)
avant d'affecter le signal utilisé en sortie.

- `avl_address_i = 0`: ID d'interface (0x12345678)

- `avl_address_i = 1`: état actuel des boutons

- `avl_address_i = 2`: état actuel des switches

- `avl_address_i = 3`: état actuel des LEDs

- `avl_address_i = 5`: status de l'interface

- `avl_address_i = 6`: concaténation du mode de génération (bit 4) avec le délai de génération (bits 1 à 0)

- `avl_address_i = 7`: informe si la lecture est fiable

- `avl_address_i = 8`: premier buffer de 4 caractères

- `avl_address_i = 9`: deuxième buffer de 4 caractères

- `avl_address_i = 10`: troisième buffer de 4 caractères

- `avl_address_i = 11`: quatrième buffer de 4 caractères

- `avl_address_i = 12`: checksum (bits 7 à 0)

- Autre valeur: `"00...0"`

## Chronogramme de fonctionnement

### Fonctionnement manuel

Pour le fonctionnement manuel, le CPU envoie des commandes à l'interface pour reseter l'interface, générer les prochains caractères ou lire les caractères. 

![Chronogramme manuel](/imgs/chrono_manuel.png)

### Fonctionnement automatique non-fiable

Dans le fonctionnement automatique non-fiable, l'interface génère les caractères automatiquement selon la fréquence définie ('delay_gen'). Le CPU peut lire les caractères à tout moment, mais n'a aucune garantie que les prochaines charactères sont de la même séquence que les précédents. Seulement avec vérification du checksum, le CPU peut savoir si les caractères lus sont corrects.

![Chronogramme auto non-fiable](/imgs/chrono_auto_non_fiable.png)

### Fonctionnement automatique fiable

![Chronogramme auto fiable](/imgs/chrono_auto_fiable.png)

## MSS

Pour la partie 2, nous avons ajouté une MSS qui permet de cadencer la sauvegarde et libération de la lecture des caractères. Cette MSS est composée de 3 états: `WAIT_FOR_SAVE`, `SAVE` et `SAVE_DONE`. Lorsque l'interface est en mode automatique, la MSS attend qu'une sauvegarde soit demandée. Lorsque la sauvegarde est demandée, l'interface sauvegarde les caractères dans un registre. Au prochain cycle d'horloge, l'interface informe le CPU via le registre `status(0)` que la sauvegarde est terminée.  

![MSS](/imgs/mss.png)

## Code

La base de notre code est très semblable au code des laboratoires précédents. Toutefois, elle a été
adapté avec les nouvelles adresses et masques à utiliser.

L'interface utilisant le bus Avalon, les adresses qui nous intéressent commencent à `0xFF20 0000`,
soit l'adresse de base pour l'AXI lightweight HPS-to-FPGA. La zone mémoire pour notre interface
se trouve à l'offset `0x0001 0000`, et nous disposons de 64KiB de mémoire. De ce fait, notre
interface interagit avec l'espace mémoire, offset, `0x0001 0000` à `0x0001 FFFF`.

Le code va lire l'ID du design standard à la première adresse de cet espace mémoire (offset `0x0`).

### Accès mémoire

Lorsqu'on lance l'application, on retrouve la mémoire à l'état ci-dessous. L'ID de l'interface y
est visible, la valeur des keys est lu (en active low), et on y retrouve également la première
chaîne de caractères lue. Dans notre cas, "Hello world!".

![État initial](/imgs/initial_state.JPG)

La pression sur le Key0, qui réinitialise le générateur, n'a pas d'impacte sur la mémoire, car la
chaîne de caractères qu'on y retrouve est déjà la chaîne de caractères initiale.

![Pression sur le Key0](/imgs/key0_press.JPG)

Lorsqu'on appuie sur le Key1, on remarque qu'une nouvelle chaîne de caractères est générée.

![Pression sur le Key1](/imgs/key1_press.JPG)

On laisse tourner l'application dans le mode automatique, avec la fréquence maximale. Tant qu'on
n'appuie pas sur le Key2, utilisé pour effectuer la lecture des chaînes de caractères, le buffer
de caractères qu'on retrouve en mémoire est rempli de zéros.

![Mode automatique, fréquence maximale, avec lecture fiable, avant demande de lecture](/imgs/max_freq_auto_trusty_noread.JPG)

Dès qu'on appuie sur le Key2, la demande de lecture fiable est envoyée à l'interface, et la chaîne
peut être retrouvée peu après en mémoire.

![Mode automatique, fréquence maximale, avec lecture fiable, lecture](/imgs/max_freq_auto_trusty_read.JPG)


# Tests

## Partie 1

### Simulation

Pour la partie 1, nous avons effectué en premier des tests avec le test bench fourni. Nous avons créé une série de commandes que le CPU peut envoyer à l'interface. Nous avons testé toutes les lectures et écritures décrites dans la description du laboratoire.

| Commande       | Description                                    |
| -------------- | ---------------------------------------------- |
| `Read 0`       | Lecture de l'interface user ID                 |
| `Read 4`       | Lecture des 4 buttons                          |
| `Read 8`       | Lecture des 10 switches                        |
| `Write 12 682` | Ecriture de 682 sur les 10 LEDs (0b1010101010) |
| `Read 12`      | Lecture des 10 LEDs                            |
| `Write 16 17`  | Ecriture dans new_char [4] et init_char [0]    |
| `Read 16`      | Lecture du status                              |
| `Write 20 16`  | Ecriture dans mode_gen [4] et delay_gen [1-0]  |
| `Read 20`      | Lecture du mode_gen [4] et de delay_gen [1-0]  |
| `Read 32`      | Lecture des char 1 à 4                         |
| `Read 36`      | Lecture des char 5 à 8                         |
| `Read 40`      | Lecture des char 9 à 12                        |
| `Read 44`      | Lecture des char 13 à 16                       |
| `Read 48`      | Lecture du checksum [7-0] 48                   |

Ici, un screenshot de l'interface de simulation:

![Simulation](/imgs/p1_test1_0.png)

Et les chronogrammes:

![Chronogramme 1](/imgs/p1_test1_1.png)

![Chronogramme 2](/imgs/p1_test1_2.png)

![Chronogramme 3](/imgs/p1_test1_3.png)

Cette simulation nous a permis de vérifier que l'interface réagite correctement aux commandes du CPU. 

### Tests sur la carte

> **NOTE:** Les tests ont été effectués de façon séquentielle. Du coup, si nous effectuons le
test "Set du switch 7 à 1 active le mode automatique", alors le mode automatique reste actif
jusqu'à-ce qu'on le désactive avec "Set du switch 7 à 0 active le mode automatique".

Pour la partie 1, nous avons effectué les tests ci-dessous sur la carte. Tous les tests mentionnés
ont été effectués avec succès.

| Test effectué                                                                                  |
| :--------------------------------------------------------------------------------------------- |
| Le programme affiche les 2 IDs attendus                                                        |
| Pression de la Key0 réinitialise le générateur réinitialise le générateur à "Hello world!"     |
| Pression sur Key2 affiche une lecture                                                          |
| La lecture affiche les caractères dans le bon ordre ("Hello world!" comparé à "lleHow o!dlr")  |
| Calcul d'intégrité est correct (somme des caractères + valeur de checksum)                     |
| Maintient de la Key2 affiche la même lecture continuellement                                   |
| Maintient de la Key2 + pression de la Key0 réinitialise le générateur à "Hello world!"         |
| Maintient de la Key2 + pression de la Key1 affiche une lecture avec une string différente      |
| Maintient de la Key2 + maintient de la Key1 affiche une seule lecture différente               |
| Set du switch 7 à 1 active le mode automatique                                                 |
| Affichage des lectures affiche les bonnes informations en cas de succès                        |
| Affichage des lectures affiche les bonnes informations en cas d'erreur                         |
| Le compteur des erreurs incrémente à chaque erreur                                             |
| Incrém. de la fréq. est effectuée, et le nombre de strings différentes par seconde augmente    |
| Changement de fréquence reflet le nombre d'erreur perçues                                      |
| Décr. de la fréq., à partir de la valeur maximale jusqu'à minimale, est effectuée correctement |
| Set du switch 7 à 0 désactive le mode automatique                                              |
| Pression de la Key0 lorsque le mode automatique est actif réinitialise le générateur           |

## Partie 2

### Simulation

Pour la simulation de la partie 2, nous avons effectué les mêmes tests que pour la partie 1, mais en ajoutant les tests pour les nouvelles fonctionnalités. Voici les commandes utilisées:

| Commande       | Description                                    |
| -------------- | ---------------------------------------------- |
| `Read 0`       | Lecture de l'interface user ID                 |
| `Read 4`       | Lecture des 4 buttons                          |
| `Read 8`       | Lecture des 10 switches                        |
| `Write 12 682` | Ecriture de 682 sur les 10 LEDs (0b1010101010) |
| `Read 12`      | Lecture des 10 LEDs                            |
| `Write 16 17`  | Ecriture dans new_char [4] et init_char [0]    |
| `Read 16`      | Lecture du status                              |
| `Write 20 16`  | Ecriture dans mode_gen [4] et delay_gen [1-0]  |
| `Read 20`      | Lecture du mode_gen [4] et de delay_gen [1-0]  |
| `Write 24 1`   | Ecriture dans save_char [0]                    |
| `Write 28 1`   | Ecriture dans reliable [0]                     |
| `Read 28`      | Lecture de reliable [0]                        |
| `Read 32`      | Lecture des char 1 à 4                         |
| `Read 36`      | Lecture des char 5 à 8                         |
| `Read 40`      | Lecture des char 9 à 12                        |
| `Read 44`      | Lecture des char 13 à 16                       |
| `Read 48`      | Lecture du checksum [7-0] 48                   |

Ici, un screenshot de l'interface de simulation:

![Simulation partie 2](/imgs/p2_test1_0.png)

Et les chronogrammes:

![Chronogramme 1 partie 2](/imgs/p2_test1_1.png)

![Chronogramme 2 partie 2](/imgs/p2_test1_2.png)

![Chronogramme 3 partie 2](/imgs/p2_test1_3.png)

### Tests sur la carte

Pour la partie 2, nous avons re-effectué les tests de la partie 1, ainsi que les tests suivants:

| Test effectué                                                                     |
| :-------------------------------------------------------------------------------- |
| Set du switch 7 à 1 active le mode automatique                                    |
| Set du switch 0 à 1 active le mode fiable                                         |
| Fréquence plus basse (1Hz) génère des mots dans la bonne fréquence                |
| Pression sur Key2 affiche une lecture sans erreurs                                |
| Maintient sur Key2 affiche une lecture sans erreurs                               |
| Incrém. de la fréq. est effectuée, et le nombre de lectures incorrectes reste à 0 |
| Set du switch 0 à 0 désactive le mode fiable                                      |
| Maintient de la Key2 affiche la même lecture continuellement                      |

# Conclusion

Dans ce laboratoire, nous avons pu implémenter une interface qui peut lire des chaînes de caractères générées d'une manière fiable ou non-fiable. Nous avons pu tester cette interface en simulation et sur la carte. Les tests ont été effectués avec succès, et nous avons pu vérifier que l'interface fonctionne correctement.

Nous avons aussi fait contrôlé la partie 1 à Anthony Convers le 6. décembre et la partie 2 à M. Messerli le 13. décembre.

# Code