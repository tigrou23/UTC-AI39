#define RCC_AHB4ENR    (*(volatile uint32_t*)0x580244E0)
#define GPIOI_MODER    (*(volatile uint32_t*)0x58022000)
#define GPIOI_IDR      (*(volatile uint32_t*)0x58022010)
#define GPIOI_ODR      (*(volatile uint32_t*)0x58022014)

// Adresse de base du Timer 2
#define TIM2_BASE       0x40000000
#define TIM2_CR1        (*(volatile uint32_t *)(TIM2_BASE + 0x00))  // Registre de contrôle
#define TIM2_SR         (*(volatile uint32_t *)(TIM2_BASE + 0x10))  // Registre de statut
#define TIM2_PSC        (*(volatile uint32_t *)(TIM2_BASE + 0x28))  // Prédiviseur
#define TIM2_ARR        (*(volatile uint32_t *)(TIM2_BASE + 0x2C))  // Registre d'auto-rechargement

/* Ajout pour les interruptions */
#define TIM2_DIER       (*(volatile uint32_t *)(TIM2_BASE + 0x0C))
#define irq_enable() asm volatile("cpsie i" :::)
#define irq_disable() asm volatile("cpsid i" :::)

// RCC (Enable Clock for Timer 2)
#define RCC_BASE        0x58024400
#define RCC_APB1LENR    (*(volatile uint32_t *)(RCC_BASE + 0x0E8))

// Fréquence d'horloge système
#define SYS_CLOCK_HZ    64000000  // 64 MHz

  /* Initialiser un port GPIO
     Active le périphérique GPIO associé au port donné en paramètre
     au moyen du RCC.
     Le port est identifié par une lettre de 'A' à 'K'.
  */
  void gpio_init(char port);

/* Choisir le mode d'un bit d'un port GPIO
  Le bit est initialisé en entrée ou sortie en fonction de dir :
	- 0 : sortie
	- 1 : entrée
  Le port est identifié par une lettre de 'A' à 'K'
  Le bit par un numéro d'ordre de 0 à 15.
*/
void gpio_mode(char port, int bit, int dir);

/* Affecter un bit à un port configuré en sortie */
void gpio_set(char port, int n, int value);
//mettre la valeur au bit de numéro n

// Initialiser le Timer 2 pour générer une interruption toutes les 500ms
void timer_init();

// Attendre que le Timer 2 atteigne 500ms
void timer_wait();

// Fonction appelée automatiquement lors de l'interruption du Timer 2
void tim2_handler(void);
