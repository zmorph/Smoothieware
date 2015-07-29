#ifndef ENCODERS_H
#define ENCODERS_H

#include "Module.h"
#include <string>

class EncoderBase;

class Encoders : public Module {
    public:
        Encoders();
        virtual ~Encoders() {};

        void on_module_loaded();
        void on_gcode_received(void *);
        void on_main_loop();
		void send_gcode(std::string g);
        uint32_t periodic_encoder_check(uint32_t dummy);
    private:
        EncoderBase* encoders;
        char command [50];

};





#endif
