#pragma once
#include <concepts>
#include <iostream>

template<typename I> requires std::integral<I>
class rational
{
    I num;
    I den;
    public:   
        rational(){
            num = 0; den = 1;
        }

        rational(const I& pnum, const I& pden){
            num = pnum; den = pden;
            semplifica();
        }

        // leggere num e den
        I nume() const {return num;};
        I deno() const {return den;};

        rational& operator+=(const rational& other) {
            if(num == 0 && den == 0 || other.den ==0 && other.num == 0)
            {
                num = 0; den = 0;
            }
            else if(num != 0 && den == 0 || other.den == 0 && other.num != 0)
            {
                num = 1; den = 0;
            }
            else
            {
            num = (num * other.den) + (other.num * den);
            den = den * other.den;
            semplifica();
            }
            return *this;
        }

        rational operator+(const rational& other) const {
            rational ret = *this;
            ret += other;
            return ret;
        }

        rational& operator-=(const rational& other) {
            if(num == 0 && den == 0 || other.den ==0 && other.num == 0)
            {
                num = 0; den = 0;
            }
            else if(num != 0 && den == 0 && other.den == 0 && other.num != 0)
            {
                num = 0; den = 0;
            }
            else 
            {
            num = (num * other.den) - (other.num * den);
            den = den * other.den;
            semplifica();
            }
            return *this;
        }

        rational operator-(const rational& other) const {
            rational ret = *this;
            ret -= other;
            return ret;
        }

        rational& operator*=(const rational& other) {
            num = num*other.num;
            den = den *other.den;
            semplifica();
            return *this;
        }

        rational operator*(const rational& other) const {
            rational ret = *this;
            ret*= other;
            return ret;
        }

         rational& operator/=(const rational& other) {
            num = num*other.den;
            den = den *other.num;
            semplifica();
            return *this;
        }

        rational operator/(const rational& other) const {
            rational ret = *this;
            ret/= other;
            return ret;
        }
    private:
        I maxcomdiv(I a,I b)
        {
            if(a<0){a = -a;}
            if(b<0){b = -b;}
            while(b != 0)
            {
                I resto = a%b;
                a = b;
                b = resto;
            }
            return a;
        }
        void semplifica(){
            if(den == 0){return;}
            I mcd = maxcomdiv(num,den);
            num = num/mcd;
            den = den/mcd;
            if(den<0){
                num = -num;
                den = -den;
            }
        }

};

template<typename I>
std::ostream&
operator<<(std::ostream& os, const rational<I>& r)
{
    if(r.deno() == 0)
    {
        if(r.nume() ==0)
        {
            os << "Nan";
        }
        else
        {
            os << "Inf";
        }
    }
    else
    {
        if(r.nume() ==0)
        {
            os << 0;
        }
        else
        {
            if(r.deno()==1)  
            {                
                os << r.nume();
            }
            else
            {
                os <<r.nume() << "/" << r.deno();
            }
        }
    }
return os;
}