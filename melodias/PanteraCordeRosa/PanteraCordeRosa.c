#include <stdio.h>                // Biblioteca para funções de entrada e saída padrão
#include "pico/stdlib.h"           // Biblioteca para funções específicas do Raspberry Pi Pico
#include "hardware/pwm.h"          // Biblioteca para controle de PWM (Pulse Width Modulation)
#include "notas.h"                 // Biblioteca externa com as notas musicais (definidas na matriz)
#include <stdlib.h>

#define NO_SOUND 0                 // Definição para uma nota que significa "sem som" (frequência 0)

#define PWM_PIN 21                 // Definindo o pino GPIO 21 para saída de PWM (som)

float divisor_frequency = 125;    // Divisor de frequência para ajustar o tom do PWM

// Função que configura e toca uma nota com base na frequência e duração fornecida
void play_note(uint frequency, float duration, float clkdiv) {
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN); // Obtém o número do slice (canal PWM) para o pino
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);      // Define a função do pino para PWM

    pwm_config config = pwm_get_default_config();    // Obtém a configuração padrão do PWM
    pwm_config_set_clkdiv(&config, clkdiv);          // Ajusta o divisor de frequência do PWM

    uint wrap = 125000000 / (frequency * clkdiv) - 1; // Calcula o valor de wrap baseado na frequência e divisor
    pwm_config_set_wrap(&config, wrap);              // Define o wrap para o PWM

    pwm_init(slice_num, &config, true);              // Inicializa o PWM no slice e começa a geração de sinal
    pwm_set_gpio_level(PWM_PIN, wrap / 2);           // Define o duty cycle de 50% (meia altura)

    sleep_ms(duration);                             // Aguarda a duração da nota (em milissegundos)

    pwm_set_gpio_level(PWM_PIN, 0);                 // Desliga o sinal PWM (tira o som)
    pwm_set_enabled(slice_num, false);              // Desativa o PWM
}



int melodia[] = {
  PAUSA, PAUSA, PAUSA, NOTE_DS4, 
  NOTE_E4, PAUSA, NOTE_FS4, NOTE_G4, PAUSA, NOTE_DS4,
  NOTE_E4, NOTE_FS4, NOTE_G4, NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_B4,   
  NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_D4, 
  NOTE_E4, PAUSA, PAUSA, NOTE_DS4,

  NOTE_E4, PAUSA, NOTE_FS4, NOTE_G4, PAUSA, NOTE_DS4,
  NOTE_E4, NOTE_FS4, NOTE_G4, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_B4, NOTE_E5,
  NOTE_DS5,   
  NOTE_D5, PAUSA, PAUSA, NOTE_DS4, 
  NOTE_E4, PAUSA, NOTE_FS4, NOTE_G4, PAUSA, NOTE_DS4,
  NOTE_E4, NOTE_FS4, NOTE_G4, NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_B4,   
  
  NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_D4, 
  NOTE_E4, PAUSA,
  PAUSA, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_E4,
  NOTE_AS4, NOTE_A4, NOTE_AS4, NOTE_A4, NOTE_AS4, NOTE_A4, NOTE_AS4, NOTE_A4,   
  NOTE_G4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_E4
};

int tempoNotas[] = {
  2, 4, 8, 8, 
  -4, 8, 8, -4, 8, 8,
  -8, 8, -8, 8, -8, 8, -8, 8,   
  2, -16, -16, -16, -16, 
  2, 4, 8, 4,

  -4, 8, 8, -4, 8, 8,
  -8, 8, -8, 8, -8, 8, -8, 8,
  1,   
  2, 4, 8, 8, 
  -4, 8, 8, -4, 8, 8,
  -8, 8, -8, 8, -8, 8, -8, 8,   
  
  2, -16, -16, -16, -16, 
  -4, 4,
  4, -8, 8, -8, 8, -8, -8,
  16, -8, 16, -8, 16, -8, 16, -8,   
  -16, -16, -16, 16, 16, 2
};


// Compasso da música (determina o tempo da música)
const int compasso = 1000; // Compasso que altera a duração das notas

// Função principal que executa a reprodução das notas
int main() {
    stdio_init_all(); // Inicializa a comunicação com o terminal (para depuração, por exemplo)
    int numeroNotas = sizeof(melodia) / sizeof(melodia[0]);  // Número de notas na melodia
    
    // Laço para tocar todas as notas da melodia
    for (int Nota = 0; Nota < numeroNotas; Nota++) { // notas no array melodia
        int tempo = compasso / tempoNotas[Nota]; // Calcula o tempo da nota com base no compasso

        // Verifica se o tempo da nota é negativo (nota pontuada)
        if (tempoNotas[Nota] < 0) {
            tempo = compasso / abs(tempoNotas[Nota]); // Calcula a duração da nota
            tempo = tempo * 1.2; // Aumenta a duração em 20% para notas pontuadas
        }

        // Toca a nota por um tempo determinado
        play_note(melodia[Nota], tempo, divisor_frequency); 
        sleep_ms(tempo * 1.2); // Dá um intervalo de 20% após a nota para distinguir
    }

    // Desliga o PWM após a execução da melodia
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);
    pwm_set_enabled(slice_num, false);
    gpio_put(PWM_PIN, 0);

    printf("Reprodução concluída.\n"); // Mensagem de fim da execução
    return 0; // Finaliza o programa
}
