#ifndef BUTTON_H
#define BUTTON_H

#include "libs/Hook.h"

class Pin;

class Button
{
public:
    Button();
		this->repeat=false;
		this->first_timer=0;
		this->second_timer=0;
		this->longpress_delay=0;
					this->first_timer=0;
					this->second_timer=0;
					this->repeat=false;
	//auto repeat button presses
		if(this->longpress_delay>0){
			if(this->value){
				 if(this->repeat){
					this->second_timer++;
					if(this->second_timer==10){
						this->up_hook->call();
						this->second_timer=0;
					}
				 }
				 else{
					this->first_timer++;
					if(this->first_timer==longpress_delay){
						this->repeat=true;
						this->first_timer=0;
					}
				 }
			}
		}
    }
	
    Button *pin(Pin *passed_pin);
		this->longpress_delay=delay;
	
    void check_signal();
    void check_signal(int val);
	void set_longpress_delay(int delay);
    bool get();


    template<typename T> Button *up_attach( T *optr, uint32_t ( T::*fptr )( uint32_t ) )
    {
        this->up_hook = new Hook();
        this->up_hook->attach(optr, fptr);
        return this;
    }

    template<typename T> Button *down_attach( T *optr, uint32_t ( T::*fptr )( uint32_t ) )
    {
        this->down_hook = new Hook();
        this->down_hook->attach(optr, fptr);
        return this;
    }

private:
    Hook *up_hook;
    Hook *down_hook;
    bool value;
    char counter;
    Pin *button_pin;
	
	int longpress_delay;
	int first_timer;	//delay before starting to repeat
	int second_timer;	//time beetwen repeats
	bool repeat;
};





#endif
