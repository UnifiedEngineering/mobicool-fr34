#ifndef IRMCF183_H
#define	IRMCF183_H

void Compressor_Init(void);
void Compressor_OnOff(bool on, bool fanon, uint8_t speed);
bool Compressor_IsOn(void);

#endif	/* IRMCF183_H */
