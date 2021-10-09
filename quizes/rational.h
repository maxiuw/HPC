#include <iostream>

class Rational{

    friend std::istream& operator>>(std::istream&, Rational&);
    friend std::ostream& operator<<(std::ostream&, const Rational&);

    public:
        // default
        Rational(): num(0), den(1){}
        // copy constructor
        Rational(Rational& src):num(src.num), den(src.den){}
        // move constructor 
        Rational(Rational&& src):num(std::move(src.num)), den(std::move(src.den)){}
        // normal creating 
        Rational(int numer,int denom):num(numer),den(denom){}
        
        Rational operator+(const Rational& add){
            int numerator = num*add.den + den*add.num;
            int denum = this->den*add.den;
            return Rational(numerator,denum);
        }
        bool operator<(const Rational& comp){
            return num*comp.den < comp.num*den;
        }

    private:
        int num;
        int den;

};

std::istream& operator>>(std::istream &is, Rational &r){
    return (is >> r.num >> r.den);
}
std::ostream& operator<<(std::ostream &os, const Rational &r) {
    return (os << r.num <<"/"<<r.den);
}
