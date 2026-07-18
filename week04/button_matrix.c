#include "stm32f1xx_hal.h"

#define LED_PORT    GPIOA
uint16_t LED_PINS[4] = {GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4};

#define KEYPAD_PORT GPIOB
uint16_t ROW_PINS[2] = {GPIO_PIN_5, GPIO_PIN_6};
uint16_t COL_PINS[2] = {GPIO_PIN_7, GPIO_PIN_8};

uint8_t KEYPAD_2x2_Scan(){
    for (uint8_t r = 0; r < 2; r++){
        for (uint8_t i = 0; i < 2; i++){
            HAL_GPIO_WritePin(KEYPAD_PORT, ROW_PINS[i], (i == r)?GPIO_PIN_RESET:GPIO_PIN_SET);
        }
        HAL_Delay(1);

        for (uint8_t c = 0; c < 2; c++){
            if (HAL_GPIO_ReadPin(KEYPAD_PORT, COL_PINS[c]) == GPIO_PIN_RESET){
                HAL_Delay(20);

                if (HAL_GPIO_ReadPin(KEYPAD_PORT, COL_PINS[c]) == GPIO_PIN_RESET) {
                    while(HAL_GPIO_ReadPin(KEYPAD_PORT, COL_PINS[c]) == GPIO_PIN_RESET);
                    return (r * 2 + c) + 1;
                }
            }
        }
    }
    return 0;
}

static void GPIO_Init(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    HAL_GPIO_WritePin(LED_PORT, LED_PINS[0] | LED_PINS[1] | LED_PINS[2] | LED_PINS[3], GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = LED_PINS[0] | LED_PINS[1] | LED_PINS[2] | LED_PINS[3];
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(KEYPAD_PORT, ROW_PINS[0] | ROW_PINS[1], GPIO_PIN_SET);
    GPIO_InitStruct.Pin = ROW_PINS[0] | ROW_PINS[1];
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(KEYPAD_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = COL_PINS[0] | COL_PINS[1];
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEYPAD_PORT, &GPIO_InitStruct);
}

void SysTick_Handler(void) {
    HAL_IncTick();
}

int main(void){
    HAL_Init();

    GPIO_Init();

    while(1){
        uint8_t key_pressed = KEYPAD_2x2_Scan();

        if (key_pressed != 0) {
            HAL_GPIO_TogglePin(LED_PORT, LED_PINS[key_pressed - 1]);
        }
    }
}
