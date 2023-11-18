#include "date_time.h"

#include "logging.h"

#include "stm32f1xx.h"

void Init_RTC() {
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5) {
        /* Backup data register value is not correct or not yet programmed (when
           the first time the program is executed) */

        Logging("RTC not yet configured....\n");
        /* Enable PWR and BKP clocks */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

        /* Allow access to BKP Domain */
        PWR_BackupAccessCmd(ENABLE);

        /* Reset Backup Domain */
        BKP_DeInit();

        /* Enable LSE */
        RCC_LSEConfig(RCC_LSE_ON);
        /* Wait till LSE is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {
        }

        /* Select LSE as RTC Clock Source */
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

        /* Enable RTC Clock */
        RCC_RTCCLKCmd(ENABLE);

        /* Wait for RTC registers synchronization */
        RTC_WaitForSynchro();

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();

        /* Enable the RTC Second */
        RTC_ITConfig(RTC_IT_SEC, ENABLE);

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();

        /* Set RTC prescaler: set RTC period to 1sec */
        RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();

        Logging("RTC configured....\n");

        /* Adjust time by values entered by the user on the hyperterminal */
        // time_adjust();

        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
    } else {
        /* Check if the Power On Reset flag is set */
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) {
            Logging("Power On Reset occurred....\n");
        }
            /* Check if the Pin Reset flag is set */
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) {
            Logging("External Reset occurred....\n");
        }

        Logging("No need to configure RTC....\n");
        // time_adjust();
        /* Wait for RTC registers synchronization */
        RTC_WaitForSynchro();

        /* Enable the RTC Second */
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
    }
    /* Clear reset flags */
    RCC_ClearFlag();
}

uint32_t Get_RTC(void) {
    return RTC_GetCounter();
}

/**
 * @brief  Displays the current time.
 * @param  TimeVar: RTC counter value.
 * @retval None
 */
void Logging_DateTime_Now(void) {
    uint32_t TimeVar, TDD, THH, TMM, TSS;
    TimeVar = RTC_GetCounter();

    // /* Reset RTC Counter when Time is 23:59:59 */
    // if (TimeVar == 0x0001517F)
    // {
    //     RTC_SetCounter(0x0);
    //     /* Wait until last write operation on RTC registers has finished */
    //     RTC_WaitForLastTask();
    // }

    /* Compute  days */
    TDD = TimeVar / 86400;
    /* Compute  hours */
    THH = (TimeVar / 3600) % 24;
    /* Compute minutes */
    TMM = (TimeVar % 3600) / 60;
    /* Compute seconds */
    TSS = (TimeVar % 3600) % 60;

    Logging("Time: %02d-%02d:%0d:%02d\r", TDD, THH, TMM, TSS);
}

/**
 * @brief  Adjusts time.
 * @param  None
 * @retval None
 */
void Adjust_RTC(uint32_t DD, uint32_t HH, uint32_t MM, uint32_t SS) {
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Change the current time */
    RTC_SetCounter((DD * 86400 + HH * 3600 + MM * 60 + SS));
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}
