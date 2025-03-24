# 💡 STM32 – LED Blinker avec Timer 2 et Interruption

_Projet réalisé en C par deux étudiants de l'UTC ( [Hugo Pereira](https://github.com/tigrou23) & Maher Zizouni), avec une attention particulière portée à la compréhension bas niveau, la rigueur mémoire et l’optimisation du contrôle matériel 💻⚙️🔩_


---

Ce projet en C pour le microcontrôleur **STM32** met en œuvre une LED connectée à la broche qui clignote automatiquement, grâce à l’utilisation du **Timer 2** et d’un système d’interruption NVIC.

Le programme est exécuté **directement depuis la mémoire Flash** au démarrage du microcontrôleur, sans système d’exploitation, pour une maîtrise complète du matériel.

---

## 📚 Sommaire

- [🚀 Objectifs du projet](#-objectifs-du-projet)
- [📦 Contenu du projet](#-contenu-du-projet)
- [⚙️ Fonctionnement détaillé](#️-fonctionnement-détaillé)
- [🧱 Implémentation](#-implémentation)
  - [1. Initialisation mémoire (`startup.S`)](#1-initialisation-mémoire-startups)
  - [2. Gestion de la LED (`gpio.c`)](#2-gestion-de-la-led-gpioc)
  - [3. Timer 2 + interruption (`timer_init` + `tim2_handler`)](#3-timer-2--interruption-timer_init--tim2_handler)
  - [4. Table des vecteurs](#4-table-des-vecteurs)
  - [5. Linker script (`stm32.x`)](#5-linker-script-stm32x)
- [📌 Points d’apprentissage](#-points-dapprentissage)
- [📜 Licence](#-licence)
- [❤️ Remerciements](#-remerciements)

---

## 🚀 Objectifs du projet

- ✅ Piloter un **GPIO** pour allumer et éteindre une LED
- ✅ Utiliser un **timer matériel** pour créer un signal périodique
- ✅ Gérer une **interruption matérielle** déclenchée automatiquement par le Timer
- ✅ Associer la routine d’interruption au bon **vecteur (28 pour TIM2)**
- ✅ Activer l’interruption via le **NVIC** (Nested Vectored Interrupt Controller)
- ✅ Placer le programme en **Flash** pour qu’il démarre automatiquement
- ✅ Tout est fait via accès registres directs
- ✅ Maîtriser le **linker script** et l’allocation mémoire
- ✅ Réduire la consommation avec l’instruction **WFI** (Wait For Interrupt)

---

## 📦 Contenu du projet

| Fichier / Dossier     | Rôle principal |
|------------------------|----------------|
| `startup.S`            | Point d’entrée (`_start`), table des vecteurs, init `.bss` et `.data` |
| `stm32.x`              | Script de lien (`linker script`) : Flash, RAM, sections |
| `copy_data.c`          | Fonction `_copy_data()` pour transférer `.data` de la Flash à la RAM |
| `gpio.h` / `gpio.c`    | Fonctions bas-niveau pour initialiser et manipuler le GPIO |
| `main.c`               | Fonction principale : init timer, IRQ, boucle `wfi()` |
| `tim2_handler()`       | Routine d’interruption appelée toutes les 500ms |

---

## ⚙️ Fonctionnement détaillé

À la mise sous tension :
1. Le microcontrôleur lit la **table des vecteurs** à l'adresse 0x08000000 (Flash)
2. Le **vecteur de reset** pointe vers `_start` dans `startup.S`
3. `_start` :
   - Initialise la pile (`SP`)
   - Copie `.data` de la Flash vers la RAM
   - Initialise `.bss` à 0
   - Appelle `main()`
4. `main()` :
   - Configure GPIOI.13 en sortie (LED)
   - Configure le **Timer 2** avec une base de temps de 500 ms
   - Active le bit `UIE` dans `TIM2_DIER` pour générer des interruptions
   - Active l’interruption n°28 dans **NVIC**
   - Autorise globalement les interruptions (`cpsie i`)
   - Entre dans une boucle infinie avec `wfi`, pour **attendre les interruptions**
5. À chaque interruption du Timer :
   - Le `tim2_handler()` est exécuté
   - Il efface le flag `UIF` et toggle l’état de la LED

---

## 🧱 Implémentation

### 1. Initialisation mémoire (`startup.S`)
- Contient la **table des vecteurs** au début de la Flash
- Initialise `.bss` à 0 (`_init_bss`)
- Appelle `_copy_data()` pour transférer `.data`
- Démarre `main()`
- Contient aussi un **vecteur d’interruption TIM2** pointant vers `tim2_handler`

### 2. Gestion de la LED (`gpio.c`)
- `gpio_init()` : active l'horloge RCC pour GPIOI
- `gpio_mode()` : configure la broche 13 en sortie
- `gpio_set()`, `gpio_toggle()`, `gpio_get()` : contrôle du GPIO via les registres

### 3. Timer 2 + interruption (`timer_init` + `tim2_handler`)
- Configure le Timer 2 pour tick toutes les 500ms
- Active l’interruption `UIE`
- Dans `tim2_handler()` :
  - Le flag `UIF` est remis à 0
  - La LED est togglée

### 4. Table des vecteurs
- Section `.vectors` dans `startup.S`
- Positionnée en Flash (`.vectors > FLASH`)
- Contient :
  - SP initial
  - `_start` (reset)
  - Vecteur (`tim2_handler`) à la bonne position

### 5. Linker script (`stm32.x`)
- Définit deux zones mémoire :
  - `FLASH` : 0x08000000, 1MB
  - `RAM` : 0x20000000, 512KB
- Place les sections `.vectors`, `.text`, `.rodata` en Flash
- Place `.data`, `.bss` en RAM
- Utilise `AT > FLASH` pour charger `.data` en Flash mais l’exécuter en RAM
- Définit les symboles `_sdata`, `_edata`, `_sidata`, `_bss_start`, `_bss_end` pour l’initialisation manuelle

---

## 📌 Points d’apprentissage

| Compétence                             | Acquise |
|----------------------------------------|---------|
| Manipulation de registres matériels    | ✅      |
| Écriture d’un linker script complet    | ✅      |
| Gestion de mémoire Flash / RAM         | ✅      |
| Table des vecteurs d’interruption      | ✅      |
| Configuration du NVIC                  | ✅      |
| Routine d’interruption C propre        | ✅      |
| Mise en veille du cœur (`wfi`)         | ✅      |

Ce projet couvre les bases essentielles du développement embarqué bas niveau, en interaction directe avec les ressources matérielles du STM32, sans surcouche logicielle.

---

## 📜 Licence

Ce projet est open-source et fourni à des fins d’apprentissage et d’expérimentation.

Tu peux :
- 📥 Le cloner
- 🛠️ Le modifier
- 📢 Le partager
- 💡 T’en inspirer

---

## ❤️ Remerciements

Nous tenons à exprimer nos sincères remerciements à **Stéphane Bonnet** pour son accompagnement tout au long de ce projet.

Son expertise, sa pédagogie et sa disponibilité ont été des atouts précieux dans la compréhension des concepts avancés liés au développement sur microcontrôleur.
