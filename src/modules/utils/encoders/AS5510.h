#ifndef AS5510_H
#define AS5510_H

#include "libs/Kernel.h"
#include "I2C.h" // mbed.h lib
#include "libs/utils.h"
#include "EncoderBase.h"
#include <string>
#include <math.h>

#define AS5510ADDRESS_L 0xAC
#define AS5510ADDRESS_H 0xAE

class AS5510 : public EncoderBase {
    public:
        AS5510(){
            // I2C com
            this->i2c = new mbed::I2C(P0_0, P0_1);
            this->i2c->frequency(100000);
        }

        ~AS5510(){
            delete this->i2c;
        }

		void set_AS5510_adress(int value)
        {
            address_AS5510=value;
        }
		
    private:

    	int get_position_value()
    	{
    		uint8_t upper, lower; 
    		this->i2c->start();
    		this->i2c->write(AS5510ADDRESS_L);
    		this->i2c->write(0x00);
    		this->i2c->start();
    		this->i2c->write(AS5510ADDRESS_L | 1);
    		lower = this->i2c->read(1);
    		upper = this->i2c->read(0);
    		this->i2c->stop();
    		return (upper << 8) + lower;
    	}

        mbed::I2C* i2c;
		char address_AS5510;
};


#endif
