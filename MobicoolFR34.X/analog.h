#ifndef ANALOG_H
#define	ANALOG_H

void AnalogUpdate(void);
int8_t AnalogGetTemperature(void);
uint16_t AnalogGetVoltage(void);
uint16_t AnalogGetFanCurrent(void);
uint8_t AnalogGetCompPower(void);

#endif	/* ANALOG_H */
