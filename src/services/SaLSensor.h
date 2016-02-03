
#ifndef SALSENSOR_H_
#define SALSENSOR_H_

struct SaLSensorDescriptor;

typedef int32_t (*cookSensorData_t)(struct SaLSensorDescriptor *const sensDescr,const uint8_t *const cmd);

struct SaLSensorDescriptor {
    cookSensorData_t cookData;
};

int32_t cookSensorData(struct SaLSensorDescriptor *const sensDescr,const uint8_t *const cmd);

#endif