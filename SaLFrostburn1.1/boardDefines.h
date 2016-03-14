#pragma once

#define HAS_SPI_MODULE 1
#include <samd21g18a.h>
#include <samd21.h>
#include <SaLPort.h>

const static Pin senseBat = { .group = 0, .pin = 6 , .chan = 6};


#define INPUT 0
#define OUTPUT 1
#define OUTSTRONG 3
#define TRUE 1
#define FALSE 0

#define HAS_FLASHMEM 1
#define HAS_AT25SF 1
#define AT25SF_MOSI_PIN PIN_PA19
#define AT25SF_MISO_PIN PIN_PA16
#define AT25SF_SCK_PIN PIN_PA17
#define AT25SF_SLAVE_SELECT_PIN PIN_PA20

const static Pin AT25SF_SLAVE_SELECT = {.group = 0, .pin = 20 };

#define HAS_TX 1
#define CC1120_MOSI_PIN PIN_PA19
#define CC1120_MISO_PIN PIN_PA16
#define CC1120_SCK_PIN PIN_PA17
#define CC1120_SLAVE_SELECT_PIN PIN_PA21

const static Pin CC1120_SLAVE_SELECT = {.group = 0, .pin = 21 };
const static Pin CC1120_MISO = { .group = 0, .pin = 16 };
const static Pin CC1120_MOSI = { .group = 0, .pin = 19 };
const static Pin CC1120_SCK = { .group = 0, .pin = 17 };


const static Pin BUZZER = { .group = 0, .pin = 22, .cfg = 0x40 };


#define HAS_MS5607 1
#define MS5607_MOSI_PIN PIN_PA14
#define MS5607_MISO_PIN PIN_PA13
#define MS5607_SCK_PIN PIN_PA15
#define MS5607_SLAVE_SELECT_PIN PIN_PA12
const static Pin MS5607_MOSI = { .group = 0, .pin = 14 };
const static Pin MS5607_MISO = { .group = 0, .pin = 13 };
const static Pin MS5607_SCK = { .group = 0, .pin = 15 };

const static Pin MS5607_SLAVE_SELECT = {.group = 0, .pin = 12 };


#define HAS_ADXL345 1
#define ADXL345_MOSI_PIN PIN_PA14
#define ADXL345_MISO_PIN PIN_PA13
#define ADXL345_SCK_PIN PIN_PA15
#define ADXL345_SLAVE_SELECT_PIN PIN_PA18

const static Pin ADXL345_SLAVE_SELECT = {.group = 0, .pin = 18 };


#define HAS_GPS 1
#define HAS_MTK3329 1
#define MTK3339_TX_PIN PIN_PB22
#define MTK3339_RX_PIN PIN_PB23

#define MY_ADXL345_DATA_RATE ADXL345_DATARATE_1600_HZ
#define MY_ADXL345_RANGE ADXL345_RANGE_2_G | ADXL345_RANGE_FULL_RANGE

#define NVM_DFLL_COARSE_POS    58
#define NVM_DFLL_COARSE_SIZE   6
#define NVM_DFLL_FINE_POS      64
#define NVM_DFLL_FINE_SIZE     10

#define BUZZERPIN PIN_PA22

const static Pin LedPin = {.group = 0, .pin = 27 };

