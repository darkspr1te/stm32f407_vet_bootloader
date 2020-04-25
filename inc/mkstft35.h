#ifndef __MKS35__
#define __MKS35__

SPI_HandleTypeDef hspi1;	// SD-card
TIM_HandleTypeDef htim2;	// Buzzer


char SPISD_Path[4];     /* USER logical drive path */

#define SOFTWARE_VERSION "0.1"
#define LOADER_VARIANT "iz3man loader"
#define HARDWARE "MKSTFT3.5-V1"
#define RENAME_FILE

#if !defined(MAIN_PR_OFFSET)
#define MAIN_PR_OFFSET 0xC000
#endif
#if !defined(FIRMWARE)
#define FIRMWARE                "0:/mkstft35.bin"
#endif 



typedef void (*Callable)();

#define u8 uint8_t
#define u16 uint16_t
const uint32_t *mcuFirstPageAddr = (const uint32_t *) (0x8000000 + MAIN_PR_OFFSET);

#endif 
