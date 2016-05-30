#ifndef RTC_H
#define RTC_H
	#define ANIO 9
	#define MES 8
	#define DIA 7
	#define HORA 4
	#define MIN 2
	#define SEC 0

	/* agarra el valor desado */
	unsigned char getValue(char r);
	/* setea el valor value*/
	void setValue(char r,unsigned char value);

	uint8_t DecimaltoBCD(uint8_t n);
	uint8_t BCDtoDecimal(uint8_t num);

	unsigned char RTCparameters(char arg);
	void set_date(char arg, uint32_t value);

#endif
