#include "gpio.h"
#include <stdint.h>


//que le RCC, on s'en fiche du port pour l'instant . affecter un 1 au bit 8.
void gpio_init(char port) {
    uint32_t *gpio_mode_register = 0;

    if (port == 'I') {
        gpio_mode_register = &GPIOI_MODER;
    }
    RCC_AHB4ENR |=  (1 << 8);

    while (RCC_AHB4ENR & (1 << 8) == 0);
}

void gpio_mode(char port, int bit, int dir) {
    volatile uint32_t *gpio_mode_register = 0;
    if (port == 'I') {
        gpio_mode_register = &GPIOI_MODER;
    }

    // Configurer le GPIO en entrée (1) ou en sortie (0)
    if (dir == 0) {  // Mode sortie
        *gpio_mode_register &= ~(3 << (bit * 2));  // Effacer les bits
        *gpio_mode_register |= (1 << (bit * 2));   // Mettre à 01 pour mode sortie
    } else {  // Mode entrée
        *gpio_mode_register &= ~(3 << (bit * 2));  // Effacer les bits pour configurer en entrée
    }
}

void gpio_set(char port, int bit, int value) {
    volatile uint32_t *gpio_odr_register = 0;

    if (port == 'I') {
    	gpio_odr_register = &GPIOI_ODR;
    }

    if (value) {
        *gpio_odr_register |= (1 << bit);  // Mettre à 1
    } else {
        *gpio_odr_register &= ~(1 << bit); // Mettre à 0
    }
}

/* Lire un bit d'un port configuré en entrée */
int gpio_get(char port, int bit){
	volatile uint32_t *gpio_idr_register = 0;

	    if (port == 'I') {
	        gpio_idr_register = &GPIOI_IDR;  // Adresse GPIOI_IDR
	    } else {
	        return -1;  // Retourner une erreur si le port n'est pas supporté
	    }

	    return (*gpio_idr_register & (1 << bit)) ? 1 : 0;  // Retourne l'état du bit

}

/* Inverser la valeur d'une entrée */
void gpio_toggle(char port, int bit){
	volatile uint32_t *gpio_odr_register = 0;

	    if (port == 'I') {
	        gpio_odr_register = &GPIOI_ODR;  // Adresse GPIOI_ODR
	    } else {
	        return;
	    }

	    *gpio_odr_register ^= (1 << bit);  // XOR pour inverser l'état du bit

}

void timer_init() {
    RCC_APB1LENR |= (1 << 0);  // Activer l'horloge du Timer 2

    TIM2_PSC = (SYS_CLOCK_HZ / 1000) - 1;  // Diviser l'horloge pour obtenir une base de 1ms
    TIM2_ARR = 500;  // Déclencher un événement toutes les 500ms

    TIM2_CR1 |= (1 << 0);  // Activer le Timer 2 (bit CEN)
}

void timer_wait() {
    while (!(TIM2_SR & (1 << 0)));  // Attendre que le bit UIF (Update Flag) soit à 1
    TIM2_SR &= ~(1 << 0);  // Reset du flag pour le prochain cycle
}

int main(){
    gpio_init('I');         // Activer GPIOI
    gpio_mode('I', 13, 0);  // Configurer GPIOI, broche 13 en sortie
    timer_init();

    while (1) {
    	timer_wait();
    	gpio_toggle('I', 13);  // Inverser l'état de la LED
    }

}
