#ifndef SALCONV2C_H_
#define SALCONV2C_H_

static inline int16_t Convert2C(uint8_t *values) {
    int16_t result = 0;

    int16_t negative = (*(values + 1) & (1 <<7)) != 0;
    if (negative) {
        // if negative then preform 2's complement to int conversion
        result = ((*(values + 1) | ~((1 << 8) - 1)) << 8 ) | *values;
    } else {
        result = (*(values + 1) << 8) + *values;
    }
    return result;
}


#endif