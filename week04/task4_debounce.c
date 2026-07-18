#include "stm32f1xx_hal.h"

#define LED_PIN     GPIO_PIN_13
#define LED_PORT    GPIOC
#define BUTTON_PIN  GPIO_PIN_0
#define BUTTON_PORT GPIOA

void process_button(void){
    static uint32_t last_debounce_time = 0;
    static GPIO_PinState last_button_state = GPIO_PIN_SET;
    static uint8_t is_pressed = 0;

    GPIO_PinState current_reading = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);

    if (current_reading != last_button_state){
        last_debounce_time = HAL_GetTick();
        last_button_state = current_reading;
    }

    if ((HAL_GetTick() - last_debounce_time) > 20){
        if (current_reading == GPIO_PIN_RESET) {
            if (is_pressed == 0){
                HAL_GPIO_TogglePin(LED_PORT, LED_PIN);

                is_pressed = 1;
            }
        } else {
            is_pressed = 0;
        }
    }
}

static void GPIO_Init(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);
}

void SysTick_Handler(void) {
    HAL_IncTick();
}

int main(void){
    HAL_Init();

    GPIO_Init();

    while(1){
        process_button();
    }
}