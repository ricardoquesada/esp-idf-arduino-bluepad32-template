#include <Arduino.h>

class Racer
{
    const int m1_1 = 21;
    const int m1_2 = 22;
    const int m2_1 = 25;
    const int m2_2 = 32;

    u8_t _speed = 3;

public:
    void speed_up()
    {
        if (_speed < 3)
            _speed++;
        Console.printf("RACER: speed(%i)=%i\n", _speed, speed());
    }
    void speed_down()
    {
        if (_speed > 0)
            _speed--;
        Console.printf("RACER: speed(%i)=%i\n", _speed, speed());
    }

    u8_t speed()
    {
        switch (_speed)
        {
        case 0:
            return 200;
        case 1:
            return 220;
        case 2:
            return 240;

        case 3:
        default:
            return 255;

        }
    }

    void setup(){
        pinMode(m1_1, OUTPUT);
        pinMode(m1_2, OUTPUT);
        pinMode(m2_1, OUTPUT);
        pinMode(m2_2, OUTPUT);
    }

    void m1(bool p1, bool p2)
    {
        analogWrite(m1_1, p1 ? speed() : 0);
        analogWrite(m1_2, p2 ? speed() : 0);
    }

    void m2 (bool p1,  bool p2)
    {
        analogWrite(m2_1, p1 ? speed() : 0);
        analogWrite(m2_2, p2 ? speed() : 0);
    }

    void fwd()
    {
        m1(1, 0);
        m2(1, 0);
    }

    void rev()
    {
        m1(0, 1);
        m2(0, 1);
    }

    void f_left()
    {
        m1(0, 0);
        m2(1, 0);
    }
    void r_left()
    {
        m1(0, 1);
        m2(0, 0);
    }

    void f_right()
    {
        m1(1, 0);
        m2(0, 0);
    }
    void r_right()
    {
        m1(0, 0);
        m2(0, 1);
    }
    void spinleft()
    {
        m1(0, 1);
        m2(1, 0);
    }
    void spinright()
    {
        m1(1, 0);
        m2(0, 1);
    }
    void brake()
    {
        analogWrite(m1_1, 255);
        analogWrite(m1_2, 255);
        analogWrite(m2_1, 255);
        analogWrite(m2_2, 255);
    }
    void free()
    {
        analogWrite(m1_1, 0);
        analogWrite(m1_2, 0);
        analogWrite(m2_1, 0);
        analogWrite(m2_2, 0);
    }
};
