#ifndef NTC_H
#define NTC_H
//==========================================================================================
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h> // log
//==========================================================================================
#define  LN             log  // in math.h, (natural logaritm) ln is called "log" and the
                            // "real log" is called "log10" (how confusing)
#define  NTC_TOP        1
#define  NTC_BOTTOM     2

#define  SCALE_C        1
#define  SCALE_F        2
#define  SCALE_K        3

#define  EQUATION_3     3
#define  EQUATION_4     4

//==========================================================================================
class NTC {
private:
    uint8_t Method, Scale, Equation;
    uint8_t decim;
    double A, B, C, D, MaxAdc, Adc, Rt, Rs, mBar;
    double Hyster, Factor, Rounder, Correct;
    bool bHyst;
    char fmt[16];
public:
    NTC( const double A, const double B, const double C );
    NTC( const double A, const double B, const double C, const double D );
    void SetDecimals( uint8_t decimals );
    double GetBoilingPointWater( double mBar );
    void SetScale( uint8_t scale );
    void SetMaxADC( uint16_t max );
    void SetValueADC( uint16_t Adc );
    void SetMethod( uint8_t method );
    void SetHysteresis( bool Hysteresis );
    void SetSeriesResistor( long Resistance );
    void SetCorrection( double Correction );
    double GetTemperature( uint8_t scale );
    double GetTemperature( void );
    char *c_str( char *buf, uint8_t scale );
    char *c_str( char *buf );
};
//==========================================================================================
#endif // NTC_H
