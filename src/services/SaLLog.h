
#ifndef SALLOG_H_
#define SALLOG_H_





struct SaLAltimeterV1Log {
    char			type;	/* 0  */
    uint8_t			csum;	/* 1  */
    uint16_t		tick;	/* 2  */
    union {					/* 4 */
        struct {
            uint8_t		flight;			/*  4 */
            uint32_t	groundHeight;	/*  5*/
            uint16_t	groundAccelX;	/*  9*/
            uint16_t	groundAccelY;	/*  11*/
            uint16_t	groundAccelZ;	/*  13*/
        } flight;						/* 15*/
        struct {
            uint32_t	press;			/*  4*/
            uint32_t	temp;			/*   8*/
            uint16_t	accelX;			/*  12*/
            uint16_t	accelY;			/*  14*/
            uint16_t	accelZ;			/*  16*/
            uint16_t	accelSum;		/*  18*/
        } sensor;						/*  20*/
        struct {
            uint8_t state;				/* 4*/
            uint8_t kicker;				/* 5*/
        } state;						/*6*/
        struct {
            uint16_t	v_batt;			/* 4*/
            uint8_t		pyros;			/* 6*/
        } sences;						/* 7*/
        struct {
            float latitude;			/* 4*/
			float longitude;		/* 8*/	
        } gps;




    } u;








};


#endif