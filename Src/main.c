#include "../Drivers/CMSIS/Device/ST/STM32F4xx/Include/stm32f407xx.h"

#define LEDS_NUM 2

uint8_t leds[LEDS_NUM][3];

uint16_t DMA_data[LEDS_NUM*24+41] = {0};

volatile uint32_t time = 0;

void fillDMAData(){
    uint8_t bit = 0;
    for(int i = 0; i < 1; i++){ // i = led
        for(int j = 0; j < 3; j++){ // j = color
            for(int b = 0; b < 8; b++){ // b = color's bit
                DMA_data[bit++] = ( (leds[i][j] >> (7 - b)) ? 2 : 1 ) * TIM3->ARR / 3 ;
            }
        }
    }
    DMA_data[LEDS_NUM*24] = 0;
}

void timerInit(){
    
    RCC->APB1RSTR |= RCC_APB1RSTR_TIM3RST_Msk;  // Reset 3-th timer
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM3RST_Msk; // 

    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN_Msk; // Enable 3-th timer 

    TIM3->CR1 = 0;
    TIM3->CR1 |= TIM_CR1_ARPE_Msk; // Enable bufferization

    TIM3->CR2 = 0;
    TIM3->PSC = 0;

    TIM3->ARR = 18 -1; // 18 
    TIM3->CCMR1 = TIM_CCMR1_OC1PE
                | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
                // | TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;

    TIM3->CNT = 0;
    TIM3->CCR1 = 0;
    TIM3->DIER = TIM_DIER_CC1DE;
    TIM3->CCER = TIM_CCER_CC1E;

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER |= GPIO_MODER_MODE6_1;

    GPIOA->AFR[0] |= 1 << ((6 * 4) + 1);

    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10_1 | GPIO_OSPEEDER_OSPEEDR10_0;

    TIM3->CR1 |= TIM_CR1_CEN;
}

void dmaInit(){
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN_Msk;

    DMA1_Stream4->CR = 0; // Disabling stream, and clearing settings of it
    DMA1_Stream4->M0AR = (uint32_t)&DMA_data;// Data source 
    DMA1_Stream4->PAR = (uint32_t)&(TIM3->CCR1);// Data destination 
    DMA1_Stream4->NDTR = LEDS_NUM*24+41-1; // Data size

    DMA1_Stream4->CR = DMA_SxCR_CHSEL_0 | DMA_SxCR_CHSEL_2
                     | DMA_SxCR_PL_0 | DMA_SxCR_PL_1
                     | DMA_SxCR_MSIZE_0
                     | DMA_SxCR_PSIZE_0
                     | DMA_SxCR_MINC
                     | DMA_SxCR_CIRC
                     | DMA_SxCR_DIR_0;

    DMA1_Stream4->FCR = DMA_SxFCR_DMDIS
                      | DMA_SxFCR_FTH_0;

    DMA1_Stream4->CR |= DMA_SxCR_EN;
}

int main(void){

    leds[0][0] = (uint8_t)8;
    leds[0][1] = (uint8_t)0;
    leds[0][2] = (uint8_t)0;
    // leds[1][0] = (uint8_t)0;
    // leds[1][1] = (uint8_t)8;
    // leds[1][2] = (uint8_t)0;

    // for(int i=0; i < 8; i++){
    //     DMA_data[i] = 70;
    // }
    // for(int i=8; i < 24; i++){
    //     DMA_data[i] = 24;
    // }
    // for(int i=24; i < 65; i++){
    //     DMA_data[i] = 0;
    // }

    // for(int i=0; i < 24; i++){
    //     // DMA_data[i] = 30;
    //     DMA_data[i] = 65;
    // }
    // for(int i=24; i < 65; i++){
    //     DMA_data[i] = 0;
    // }

    for(int i=24; i < 65; i++){
        DMA_data[i] = 0;
    }

    // DMA_data[0] = DMA_data[8] = DMA_data[16] = 65;
    // DMA_data[0] = 65;

    timerInit();

    fillDMAData();

    TIM3->CCR1 = 1;

    dmaInit();

    // while(1){
    //     TIM3->CCR1++;
    //     if(TIM3->CCR1 == 89){
    //         TIM3->CCR1 = 0;
    //     }
    //     while( time < 10000) time++;
    //     time = 0;
    // }

    // while(1){

        
    //     for(int i=0; i < 48; i++){
    //         DMA_data[i] = TIM3->ARR / 3;
    //     }
    //     for(int i=48; i < 89; i++){
    //         DMA_data[i] = 0;
    //     }

    //     while( time < 1000000) time++;
    //     time = 0;

    //     for(int i=0; i < 48; i++){
    //         DMA_data[i] = 2 * TIM3->ARR / 3;
    //     }
    //     for(int i=48; i < 89; i++){
    //         DMA_data[i] = 0;
    //     }
        
    //     while( time < 1000000) time++;
    //     time = 0;
    // }
}