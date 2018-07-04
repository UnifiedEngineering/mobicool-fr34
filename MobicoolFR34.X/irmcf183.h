#ifndef IRMCF183_H
#define	IRMCF183_H

void Compressor_Init(void);
void Compressor_OnOff(bool on, bool fanon, uint8_t speedidx);
bool Compressor_IsOn(void);
uint8_t Compressor_GetMinSpeedIdx(void);
uint8_t Compressor_GetMaxSpeedIdx(void);
uint8_t Compressor_GetDefaultSpeedIdx(void);

#endif	/* IRMCF183_H */
