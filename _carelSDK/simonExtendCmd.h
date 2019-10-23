

#ifndef CARELSDK_SIMONEXTENDCMD_H_
#define CARELSDK_SIMONEXTENDCMD_H_


bool simonExtendCmd_Timer_GetEnable(uint8_t ch);
bool simonExtendCmd_Timer_GetGoalState(uint8_t ch);
void simonExtendCmd_Timer_CTL(uint8_t onoff, uint16_t time, uint8_t ch);

#endif /* CARELSDK_SIMONEXTENDCMD_H_ */
