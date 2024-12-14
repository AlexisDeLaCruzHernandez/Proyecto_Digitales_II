#include "ALARMA.h"

uint32_t evento_pwm, evento_periodo;
volatile uint16_t flag_alarma = 0;
volatile uint8_t DAC_on = 2, flag_buzzer = 0;

void SCTIMER_Inicializacion (void) {
    	sctimer_pwm_signal_param_t pwmParam;
    	sctimer_config_t sctimerInfo;
    	SCTIMER_GetDefaultConfig (&sctimerInfo);
    	SCTIMER_Init (SCT0, &sctimerInfo);
    	pwmParam.output = kSCTIMER_Out_4;
    	pwmParam.level = kSCTIMER_LowTrue;
    	pwmParam.dutyCyclePercent = 99;
    	SCTIMER_SetupPwm (SCT0, &pwmParam, kSCTIMER_CenterAlignedPwm, 1600, CLOCK_GetFreq (kCLOCK_Fro), &evento_pwm);
    	SCTIMER_StopTimer (SCT0, kSCTIMER_Counter_U);
    	SCT0->OUTPUT |= (1 << 4);
    	CLOCK_EnableClock (kCLOCK_Swm);
    	SWM_SetMovablePinSelect (SWM0, kSWM_SCT_OUT4, kSWM_PortPin_P1_0);
    	CLOCK_DisableClock (kCLOCK_Swm);
    	SCTIMER_CreateAndScheduleEvent (SCT0, kSCTIMER_OutputRiseEvent, 0, 4, kSCTIMER_Counter_U, &evento_periodo);
    	SCTIMER_EnableInterrupts (SCT0, 1 << evento_periodo);
    	EnableIRQ(SCT0_IRQn);
}

void DAC_Inicializacion (void) {
	dac_config_t configuracion_dac;
	CLOCK_EnableClock (kCLOCK_Swm);
	SWM_SetFixedPinSelect (SWM0, kSWM_DAC_OUT1, true);
	CLOCK_DisableClock (kCLOCK_Swm);
	POWER_DisablePD (kPDRUNCFG_PD_DAC1);
	configuracion_dac.settlingTime = kDAC_SettlingTimeIs25us;
	DAC_Init (DAC1, &configuracion_dac);
}

void INDICADOR_Enciendo (uint8_t mseg, uint8_t brillo) {
	SCTIMER_UpdatePwmDutycycle (SCT0, kSCTIMER_Out_4, brillo, evento_pwm);
	flag_buzzer = mseg;
	DAC_on = 0;
}

void INDICADOR_Alarma (uint8_t mseg_on, uint16_t mseg_off, uint8_t brillo) {
	if (flag_alarma == 0) {
		INDICADOR_Enciendo (TIEMPO_INDICACION, brillo); // Enciendo 50ms
		flag_alarma = mseg_off;
	}
}
