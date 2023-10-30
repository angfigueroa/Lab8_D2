#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"


//prototipo de funciones
void Timer0IntHandler(void);

void UARTIntHandler(void);

//Variables
int state = 0;

int rgb;

int uart_f = 0;

int data;

//Main Loop
int main(void)
{
    //Reloj y puertos 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ |
    SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOF) ;//HAbilitar periféricos de la Tiva C

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);//Puerto f

    //TIMER0
    SysCtlPeripheralEnable (SYSCTL_PERIPH_TIMER0) ;

    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);//periodico de 32 bits

    TimerLoadSet(TIMER0_BASE,TIMER_A,20000000);//Especificado el TimerA

    TimerIntRegister(TIMER0_BASE,TIMER_A,Timer0IntHandler);

    IntEnable(INT_TIMER0A);

    TimerEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);

    IntMasterEnable();

    TimerEnable(TIMER0_BASE, TIMER_A);

    //UART
    SysCtlPeripheralEnable (SYSCTL_PERIPH_UART0);//Habilitar perifericos del UART0

    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOA);//Perifericos A

    GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0|GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE,SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    //HAbilita el handler y comunicacion
    UARTIntEnable(UART0_BASE,UART_INT_RT|UART_INT_RX);

    UARTIntRegister(UART0_BASE, UARTIntHandler);

    while(1){
        if(state && uart_f == 1){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, rgb);//Cuando ambas banderas están habilitadas, impimen el color
        }
        else{//Se apagan
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);

        }
    }

}

//Funciones
void Timer0IntHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_A);//se borra el timer
    state = !state;//Toggle

}
void UARTIntHandler(void){
    UARTIntClear(UART0_BASE, UART_INT_RT | UART_INT_RX);
    data = UARTCharGet(UART0_BASE);//Lee el puerto y lo almacena en data
    if (data == 'r'){
        uart_f = !uart_f;
        rgb = 8;//verde
    }
    else if (data == 'g'){
        uart_f = !uart_f;
        rgb = 3;///amarillo
    }
    else if (data == 'b'){
        uart_f = !uart_f;
        rgb = 2;//rojo
    }gbbrgbb

}
