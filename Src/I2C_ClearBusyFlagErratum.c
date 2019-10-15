#include "I2C_ClearBusyFlagErratum.h"

#define I2C1_SCL_Pin GPIO_PIN_6
#define I2C1_SCL_GPIO_Port GPIOB
#define I2C1_SDA_Pin GPIO_PIN_7
#define I2C1_SDA_GPIO_Port GPIOB
 enum 
{
	FALSE,
	TRUE
	
};
uint8_t wait_for_gpio_state_timeout(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState, uint32_t timeout){
    uint32_t Tickstart = HAL_GetTick();
    uint8_t ret = TRUE;
	 /* Wait until flag is set */
    for(;(PinState != HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)) && (TRUE == ret);)
    {
        /* Check for the timeout */
        if (timeout != HAL_MAX_DELAY)
        {
            if ((timeout == 0U) || ((HAL_GetTick() - Tickstart) > timeout))
            {
                ret = FALSE;
            }
        }
        
    }
    return ret;

}

myError I2C_ClearBusyFlagErratum(I2C_HandleTypeDef* handle, uint32_t timeout)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. Clear PE bit.
    CLEAR_BIT(handle->Instance->CR1, I2C_CR1_PE);

    //  2. Configure the SCL and SDA I/Os as General Purpose Output Open-Drain, High level (Write 1 to GPIOx_ODR).
    HAL_I2C_DeInit(handle);

    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull = GPIO_NOPULL;

    GPIO_InitStructure.Pin = I2C1_SCL_Pin;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = I2C1_SDA_Pin;
    HAL_GPIO_Init(I2C1_SDA_GPIO_Port, &GPIO_InitStructure);

    // 3. Check SCL and SDA High level in GPIOx_IDR.
    HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_SET);		
    HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET);

		if(!wait_for_gpio_state_timeout(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_SET, timeout)){
				return NewError(HAL_TIMEOUT, "SDA pin cant be setted\r");
		}
    if(!wait_for_gpio_state_timeout(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET, timeout)){
				return NewError(HAL_TIMEOUT, "SCL pin cant be setted\r");
		}
   

    // 4. Configure the SDA I/O as General Purpose Output Open-Drain, Low level (Write 0 to GPIOx_ODR).
    HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_RESET);

    // 5. Check SDA Low level in GPIOx_IDR.
		if(!wait_for_gpio_state_timeout(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_RESET, timeout) ){
				return NewError(HAL_TIMEOUT, "SDA pin cant be resetted\r");
		}

    // 6. Configure the SCL I/O as General Purpose Output Open-Drain, Low level (Write 0 to GPIOx_ODR).
    HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_RESET);

    // 7. Check SCL Low level in GPIOx_IDR.
    if(!wait_for_gpio_state_timeout(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_RESET, timeout)){
				return NewError(HAL_TIMEOUT, "SCL pin cant be resetted\r");
		}

    // 8. Configure the SCL I/O as General Purpose Output Open-Drain, High level (Write 1 to GPIOx_ODR).
    HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET);

    // 9. Check SCL High level in GPIOx_IDR.
    if(!wait_for_gpio_state_timeout(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET, timeout)){
				return NewError(HAL_TIMEOUT, "SCL pin cant be setted\r");
		}	
    // 10. Configure the SDA I/O as General Purpose Output Open-Drain , High level (Write 1 to GPIOx_ODR).
    HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_SET);

    // 11. Check SDA High level in GPIOx_IDR.
    if(!wait_for_gpio_state_timeout(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_SET, timeout)){
				return NewError(HAL_TIMEOUT, "SDA pin cant be setted\r");
		}
    // 12. Configure the SCL and SDA I/Os as Alternate function Open-Drain.
    GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStructure.Alternate = GPIO_AF4_I2C1;

    GPIO_InitStructure.Pin = I2C1_SCL_Pin;
    HAL_GPIO_Init(I2C1_SCL_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = I2C1_SDA_Pin;
    HAL_GPIO_Init(I2C1_SDA_GPIO_Port, &GPIO_InitStructure);

    // 13. Set SWRST bit in I2Cx_CR1 register.
    SET_BIT(handle->Instance->CR1, I2C_CR1_SWRST);
 

    /* 14. Clear SWRST bit in I2Cx_CR1 register. */
    CLEAR_BIT(handle->Instance->CR1, I2C_CR1_SWRST);


    /* 15. Enable the I2C peripheral by setting the PE bit in I2Cx_CR1 register */
    SET_BIT(handle->Instance->CR1, I2C_CR1_PE);


    // Call initialization function.
    HAL_I2C_Init(handle);
		
		return NewError(HAL_OK, "");
}


myError _I2cFailRecover(){
    GPIO_InitTypeDef GPIO_InitStruct;
    int i, nRetry=0;


    // We can't assume bus state based on SDA and SCL state (we may be in a data or NAK bit so SCL=SDA=1)
    // by setting SDA high and toggling SCL at least 10 time we ensure whatever agent and state
    // all agent should end up seeing a "stop" and bus get back to an known idle i2c  bus state

    // Enable I/O
    __GPIOB_CLK_ENABLE();
    HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = I2C1_SCL_Pin|I2C1_SDA_Pin ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(I2C1_SDA_GPIO_Port, &GPIO_InitStruct);
    //TODO we could do this faster by not using HAL delay 1ms for clk timing
    do{
        for( i=0; i<10; i++){
            HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_RESET);
            HAL_Delay(1);
            HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET);
            HAL_Delay(1);
        }
//        if( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == 0 ){
//            static int RetryRecover;
//            RetryRecover++;
//        }
    }while(HAL_GPIO_ReadPin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin) == 0 && nRetry++<7);
		if(HAL_GPIO_ReadPin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin) == 0){
			return NewError(HAL_TIMEOUT, "SDA pin cant be setted\r");
		}
		return NewError(HAL_OK, ""); 
}