#include "NTC.h"

#define  ADC_PIN  A0

//const double A = 0.0008422026204827800000;
//const double B = 0.0002626547700231960000;
//const double C = 0.0000001187502823269870;
//class NTC ntc( A, B, C );

// OR

class NTC ntc(0.0008422026204827800000, 
              0.0002626547700231960000,
              0.0000001187502823269870);

void setup() {
    Serial.begin( 115200 );

    Serial.println( F("https://www.vishay.com/en/thermistors/ntc-rt-calculator/\n") );
    Serial.println( F("for Vishay SAP-nr NTHS1206N02N1002J, use these Steinhart/Hart values:\n") );
    Serial.println( F("A=0.0008422026204827800000") );
    Serial.println( F("B=0.0002626547700231960000") );
    Serial.println( F("C=0.0000001187502823269870\n") );

    ntc.SetScale( SCALE_C );        // Set the default scale: SCALE_C, SCALE_F or SCALE_K
                                    // and use .GetTemperature() to convert the temperature
                                    // or use .GetTemperature(SCALE_F) to convert to Fahrenheit

    ntc.SetMaxADC( 4095 );          // Max ADC value: 10-bit AVR=1023, 12-bit STM32=4095
                                    // AVCC is assumed as reference (also connect the voltage divider to AVCC)

    ntc.SetMethod( NTC_BOTTOM );    // NTC_TOP or NTC_BOTTOM
                                    // NTC_TOP, the NTC is on top and the series resistor at bottom
                                    // NTC_BOTTOM, the NTC is at the botton and the series resistor on top

    ntc.SetSeriesResistor( 4700 );  // A series resistor of 4k7
                                    // for best performance (the highest resolution in ADC reading) take the resistance
                                    // of the NTC at both ends of the scale. To calculate the series resistance to use:
                                    // Rs = SQRT( R-min * R-max )

    ntc.SetDecimals( 1 );           // The c_str method now reports the temperature with 1 decimal

    ntc.SetHysteresis( true );      // Set the hysteresis to avoid jumping between consecutive values

    Serial.println();
    Serial.println( F("When you want to calibrate your temperature sensor") );
    Serial.println( F("by measuring boiling water, keep in mind that the") );
    Serial.println( F("boiling point depends on the air pressure.\n") );
    Serial.println( F("The method .GetBoilingPointWater(mbar) returns the boiling") );
    Serial.println( F("temperature of water for the given pressure in mbar.\n") );
    Serial.println( F("The boiling point returned by .GetBoilingPointWater is") );
    Serial.println( F("accurate up to 2 decimals, between 950 and 1050 mbar.\n") );
    ntc.SetScale( SCALE_C );
    Serial.println( F("At 950     mbar, water boils at  98.21 C") );
    Serial.println( F("At 955     mbar, water boils at  98.35 C") );
    Serial.println( F("At 960     mbar, water boils at  98.50 C") );
    Serial.println( F("At 965     mbar, water boils at  98.64 C") );
    Serial.println( F("At 970     mbar, water boils at  98.78 C") );
    Serial.println( F("At 975     mbar, water boils at  98.93 C") );
    Serial.println( F("At 980     mbar, water boils at  99.07 C") );
    Serial.println( F("At 985     mbar, water boils at  99.21 C") );
    Serial.println( F("At 990     mbar, water boils at  99.35 C") );
    Serial.println( F("At 995     mbar, water boils at  99.49 C") );
    Serial.println( F("At 1000    mbar, water boils at  99.63 C") );
    Serial.println( F("At 1005    mbar, water boils at  99.77 C") );
    Serial.println( F("At 1010    mbar, water boils at  99.91 C") );
    Serial.println( F("At 1013.25 mbar, water boils at 100    C") );
    Serial.println( F("At 1015    mbar, water boils at 100.05 C") );
    Serial.println( F("At 1020    mbar, water boils at 100.19 C") );
    Serial.println( F("At 1025    mbar, water boils at 100.32 C") );
    Serial.println( F("At 1030    mbar, water boils at 100.46 C") );
    Serial.println( F("At 1035    mbar, water boils at 100.60 C") );
    Serial.println( F("At 1040    mbar, water boils at 100.73 C") );
    Serial.println( F("At 1045    mbar, water boils at 100.87 C") );
    Serial.println( F("At 1050    mbar, water boils at 101.00 C") );
}

void loop() {
    char buf[16]; // provide a buffer to the c_str function to hold the formatted conversion result
    int adc = analogRead( ADC_PIN );
    ntc.SetValueADC( adc );
    Serial.print( "ADC=" );
    Serial.println( adc );
    ntc.SetDecimals( 0 ); // The c_str method now reports the temperature WITHOUT decimals
    Serial.println( "\t\t" );
    Serial.println( ntc.c_str( buf ) );
    Serial.println( " C" );
    ntc.SetDecimals( 1 ); // The c_str method now reports the temperature with 1 decimal
    Serial.println( "\t" );
    Serial.println( ntc.c_str( buf ) );
    Serial.println( " C" );
    ntc.SetDecimals( 2 ); // The c_str method now reports the temperature with 2 decimals
    Serial.println( "\t" );
    Serial.println( ntc.c_str( buf ) );
    Serial.println( " C  " );
    ntc.SetDecimals( 1 );
    Serial.println( "\t" );
    Serial.println( ntc.c_str( buf, SCALE_K ) );
    Serial.println( " K" );
    Serial.println( "\t\t" );
    Serial.println( ntc.c_str( buf, SCALE_F ) );
    Serial.println( " F" );
    Serial.println( "\t" );
    Serial.println( ntc.GetTemperature( SCALE_F ) );
    Serial.println( );
}
