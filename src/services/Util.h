#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <compiler.h>

typedef uint8_t u8;
typedef uint16 u16;
typedef uint32 u32;

typedef struct Pin { 
     uint8_t  mux; 
     uint8_t  cfg; 
     uint8_t  group; 
     uint8_t  pin; 
     uint8_t  chan; 
 } Pin; 


