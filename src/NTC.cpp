//#include <iostream>
#define NDEBUG
#include <assert.h> // assert
#include "NTC.h"
//==========================================================================================
NTC::NTC( const double a, const double b, const double c, const double d ) {
    NTC( a, b, c ); // call the 3 parameter constructor
    D = d;          // and add the missing details for the fourth parameter
    Equation = EQUATION_4;
}
//==========================================================================================
NTC::NTC( const double a, const double b, const double c ) {
    A = a;
    B = b;
    C = c;
    Method = NTC_BOTTOM;
    Scale = SCALE_C;
    bHyst = false;
    Rs = 0.0;
    Adc = 0;
    MaxAdc = 1023;
    Correct = 0.0;
    Equation = EQUATION_3;
    SetDecimals( 1 );
}
//==========================================================================================
void NTC::SetCorrection( double correction ) {
    Correct = correction;
}
//==========================================================================================
void NTC::SetDecimals( uint8_t decimals ) {
    decim = decimals;
    if ( decimals == 0 ) {
        strcpy( fmt, "%0d" );
    } else {
        sprintf( fmt, "%cd.%c0%dd", '\%', '\%', decim );
    }
    Factor = pow( 10.0, decim );
    Rounder = 1.0 / pow( 10.0, decim ) / 2.0;
}
//==========================================================================================
char *NTC::c_str( char *buf, uint8_t scale ) {
    uint8_t saved = Scale;
    Scale = scale;
    c_str( buf );
    Scale = saved;
    return buf;
}
//==========================================================================================
char *NTC::c_str( char *buf ) {
    char *p = buf;
    double f = GetTemperature();
    if ( f <= 0 - Rounder ) {
        *p = '-';
        p++;
        *p = '\0';
    }
    f = abs( f ) + Rounder;
    sprintf( p, fmt, (int)f, (int)( f * (int)Factor ) % (int)Factor );
    return buf;
}
//==========================================================================================
double NTC::GetBoilingPointWater( double mBar ) {
    double centigrade = -0.0000104846 * pow( mBar, 2.0 ) + 0.04890953 * mBar + 61.206652;
    if ( Scale == SCALE_C ) {
        return centigrade;
    } else if ( Scale == SCALE_F ) {
        return centigrade * 9.0 / 5.0 + 32.0;
    } else if ( Scale == SCALE_K ) {
        return centigrade + 273.15;
    } else {
#ifndef NDEBUG
        std::cout << "\n\nUnknown scale, use SetScale(SCALE_C), SetScale(SCALE_F) or SetScale(SCALE_K)\n\n"
                  << std::endl;
#endif
        assert( Scale >= SCALE_C );
        assert( Scale <= SCALE_K );
        return 0.0;
    }
}
//==========================================================================================
void NTC::SetScale( uint8_t scale ) {
    Scale = scale;
}
//==========================================================================================
void NTC::SetMaxAdc( uint16_t max ) {
    MaxAdc = (double)max;
}
//==========================================================================================
void NTC::SetValueAdc( uint16_t adc ) {
    Adc = (double)adc;
}
//==========================================================================================
void NTC::SetMethod( uint8_t method ) {
    Method = method;
}
//==========================================================================================
void NTC::SetHysteresis( bool Hysteresis ) {
    bHyst = Hysteresis;
}
//==========================================================================================
void NTC::SetSeriesResistor( long Resistance ) {
    Rs = (double)Resistance; // Rs = series resistor
}
//==========================================================================================
double NTC::GetTemperature( uint8_t scale ) {
    uint8_t saved = Scale;
    Scale = scale;
    double retval = GetTemperature();
    Scale = saved;
    return retval;
}
//==========================================================================================
double NTC::GetTemperature( void ) {
    if ( Method == NTC_TOP ) {
        Rt = Rs * ( MaxAdc - Adc ) / Adc; // Rt = thermistor resistance
    } else if ( Method == NTC_BOTTOM ) {
        Rt = Rs * Adc / ( MaxAdc - Adc );
    } else {
        assert( ( Method == NTC_TOP ) || ( Method == NTC_BOTTOM ) );
    }

    double Kelvin = 0.0;
    if ( Equation == EQUATION_3 ) {
        Kelvin = 1.0 / ( A + B * LN( Rt ) + C * pow( LN( Rt ), 3.0 ) );
    } else if ( Equation == EQUATION_4 ) {
        Kelvin = 1 / ( A + B * LN( Rt ) + C * pow( LN( Rt ), 2 ) + D * pow( LN( Rt ), 3 ) );
    } else {
        assert( Equation == EQUATION_3 || Equation == EQUATION_4 );
    }

    if ( Scale == SCALE_C ) {
        double Centigrade = Kelvin - 273.15;
        return Centigrade + Correct;
    } else if ( Scale == SCALE_F ) {
        double Centigrade = Kelvin - 273.15;
        Centigrade += Correct;
        return Centigrade * 9.0 / 5.0 + 32.0;
    } else if ( Scale == SCALE_K ) {
        return Kelvin + Correct;
    } else {
        assert( Scale == SCALE_C || Scale == SCALE_F || Scale == SCALE_K );
        return 0.0;
    }
}
//==========================================================================================
