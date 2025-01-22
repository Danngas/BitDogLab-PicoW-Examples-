# Projeto: Tocando Melodias na BitDogLab

Este repositório apresenta um projeto para tocar melodias utilizando a placa **BitDogLab**, que possui um buzzer embutido. O código implementa a execução da música "Pantera Cor de Rosa", utilizando o controle de PWM (Pulse Width Modulation) para reproduzir as notas musicais.

## Funcionalidade do Código

O programa reproduz uma melodia através do buzzer embutido na BitDogLab, utilizando as seguintes etapas:

1. **Definição das notas musicais:**
   - As frequências das notas são definidas na biblioteca `notas.h`.
   - Cada nota é representada por um valor em Hertz (Hz).

2. **Configuração do PWM:**
   - O pino GPIO 21 é configurado para gerar um sinal PWM.
   - A função `play_note()` configura o PWM para a frequência desejada e define a duração da nota.

3. **Execução da melodia:**
   - O programa percorre um array de notas musicais e tempos correspondentes para reproduzir a música.
   - Pequenas pausas são inseridas entre as notas para melhor distinção.

4. **Finalização:**
   - O PWM é desativado após a execução da melodia.

## Estrutura do Código

O projeto é composto pelos seguintes arquivos:

- `main.c` - Arquivo principal que executa a melodia.
- `notas.h` - Biblioteca contendo as frequências das notas musicais.

### Código Principal (`main.c`)

```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "notas.h"

#define PWM_PIN 21
float divisor_frequency = 125;

void play_note(uint frequency, float duration, float clkdiv) {
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clkdiv);
    uint wrap = 125000000 / (frequency * clkdiv) - 1;
    pwm_config_set_wrap(&config, wrap);

    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(PWM_PIN, wrap / 2);
    sleep_ms(duration);

    pwm_set_gpio_level(PWM_PIN, 0);
    pwm_set_enabled(slice_num, false);
}

int main() {
    stdio_init_all();
    for (int i = 0; i < 82; i++) {
        int tempo = 1300 / tempoNotas[i];
        play_note(melodia[i], tempo, divisor_frequency);
        sleep_ms(tempo * 1.2);
    }
    printf("Reprodução concluída.\n");
    return 0;
}
```

### Biblioteca de Notas (`notas.h`)

Este arquivo define as frequências das notas musicais em Hertz:

```c
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define PAUSA    0
```

## Notas Musicais
As notas musicais são representadas por frequências em Hertz (Hz). Abaixo está uma tabela de referência que mostra as frequências das notas em diferentes oitavas. Clique em qualquer uma das frequências para ouvir o tom correspondente, mas lembre-se de ajustar o volume, especialmente para notas agudas.


## Notas Musicais

As notas musicais são representadas por frequências em Hertz (Hz). Abaixo está uma tabela de referência que mostra as frequências das notas em diferentes oitavas. Clique em qualquer uma das frequências para ouvir o tom correspondente, mas lembre-se de ajustar o volume, especialmente para notas agudas.

| Nota  | Frequência (Hz) | Oitava 0 | Oitava 1 | Oitava 2 | Oitava 3 | Oitava 4 | Oitava 5 | Oitava 6 | Oitava 7 | Oitava 8 |
|-------|-----------------|----------|----------|----------|----------|----------|----------|----------|----------|----------|
| C     | 16.35           | 32.7     | 65.41    | 130.81   | 261.63   | 523.25   | 1046.5   | 2093     | 4186     |
| C#    | 17.32           | 34.65    | 69.3     | 138.59   | 277.18   | 554.37   | 1108.73  | 2217.46  | 4434.92  |
| D     | 18.35           | 36.71    | 73.42    | 146.83   | 293.66   | 587.33   | 1174.66  | 2349.32  | 4698.63  |
| D#    | 19.45           | 38.89    | 77.78    | 155.56   | 311.13   | 622.25   | 1244.51  | 2489     | 4978     |
| E     | 20.6            | 41.2     | 82.41    | 164.81   | 329.63   | 659.25   | 1318.51  | 2637     | 5274     |
| F     | 21.83           | 43.65    | 87.31    | 174.61   | 349.23   | 698.46   | 1396.91  | 2793.83  | 5587.65  |
| F#    | 23.12           | 46.25    | 92.5     | 185      | 369.99   | 739.99   | 1479.98  | 2959.96  | 5919.91  |
| G     | 24.5            | 49       | 98       | 196      | 392      | 783.99   | 1567.98  | 3135.96  | 6271.93  |
| G#    | 25.96           | 51.91    | 103.83   | 207.65   | 415.3    | 830.61   | 1661.22  | 3322.44  | 6644.88  |
| A     | 27.5            | 55       | 110      | 220      | 440      | 880      | 1760     | 3520     | 7040     |
| A#    | 29.14           | 58.27    | 116.54   | 233.08   | 466.16   | 932.33   | 1864.66  | 3729.31  | 7458.62  |
| B     | 30.87           | 61.74    | 123.47   | 246.94   | 493.88   | 987.77   | 1975.53  | 3951     | 7902.13  |

**Referência:** Os valores estão em Hertz (Hz), com a linha superior representando as oitavas (de 0 a 8).



## Como Usar

1. Clone este repositório para sua máquina:
   ```sh
   git clone https://github.com/seu-usuario/bitdoglab-melody.git
   ```

2. Compile o código utilizando o ambiente de desenvolvimento para Raspberry Pi Pico (como o SDK oficial da Pico).

3. Envie o binário para a placa BitDogLab.

4. A música será reproduzida automaticamente após a inicialização da placa.

## Personalização

Para tocar diferentes melodias, altere os valores nos arrays `melodia[]` e `tempoNotas[]` no arquivo `main.c`.

## Simulação

Você pode testar a simulação do projeto no Wokwi através do seguinte link:

[Simulação no Wokwi](https://wokwi.com/projects/420790635743727617)

