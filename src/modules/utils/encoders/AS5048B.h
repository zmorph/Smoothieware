#ifndef AS55048B_H
#define AS55048B_H

#include "libs/Kernel.h"
#include "I2C.h" // mbed.h lib
#include "libs/utils.h"
#include "EncoderBase.h"
#include <string>
#include <math.h>


#define AS5048B_ADDRESS_LL 0x80 //For both address pins pulled down
#define AS5048B_ADDRESS_LH 0x82
#define AS5048B_ADDRESS_HL 0x84
#define AS5048B_ADDRESS_HH 0x86 //For both address pins pulled up

#define X_AXIS_ENCODER_ADDRESS AS5048B_ADDRESS_LL
#define Y_AXIS_ENCODER_ADDRESS AS5048B_ADDRESS_HH


#define ANGLE_REG 0xFE

class AS5048B : public EncoderBase {
    public:
        AS5048B(){
            // I2C com
            this->i2c = new mbed::I2C(P0_0, P0_1);
            this->i2c->frequency(100000);
        }

        ~AS5048B(){
            delete this->i2c;
        }

		void set_AS5048B_address(int value)
        {
            address_AS5048B=value;
        }
		
    private:

    	int get_position_value(uint8_t address)
    	{
    		uint8_t upper, lower; 
    		this->i2c->start();
    		this->i2c->write(address);
    		this->i2c->write(ANGLE_REG);
    		this->i2c->start();
    		this->i2c->write(address | 1);
    		upper = this->i2c->read(1);
    		lower = this->i2c->read(0);
    		this->i2c->stop();
    		return ((upper << 8) + (lower << 2)) >> 2;
    	}

        mbed::I2C* i2c;
		char address_AS5048B;
};


#endif
