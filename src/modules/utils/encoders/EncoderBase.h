#ifndef ENCODERBASE_H
#define ENCODERBASE_H

class EncoderBase {
    public:
        EncoderBase(){}
        virtual ~EncoderBase(){}
        virtual int get_position_value()=0;
};


#endif
