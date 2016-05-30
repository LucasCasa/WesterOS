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

//el arreglo parameters tiene la forma [anio,mes,dia,hora,minuto,segundo]
void setDate(unsigned int parameters[6]);

void setYear(unsigned char value);
void setMonth(unsigned char value);
void setDay(unsigned char value);
void setHour(unsigned char value);
void setMinute(unsigned char value);
void setSecond(unsigned char value);

unsigned char getYear();
unsigned char getMonth();
unsigned char getDay();
unsigned char getHour();
unsigned char getMinute();
unsigned char getSecond();

#endif
