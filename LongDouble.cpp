//
//  LongDouble.cpp
//  ArithCoding
//
//  Created by Ivanna Vasilkova on 21.10.2020.
//  Copyright © 2020 Ivanna Vasilkova. All rights reserved.
//
#include "includes.h"
#include <stdio.h>
class LongDouble {
public:
    vector<int> digits;
    long exponent; //экспонента для сдвига
    
    LongDouble(vector<int> d, long exp){
        digits = vector<int>(d);
        exponent = exp;
    }
    
    LongDouble(){
        digits = vector<int>(1,0);
    }
    
    LongDouble(const string &value){
        size_t index;
        index = 0;
        
        exponent = value.length() - index; // для целых чисел
        while(index < value.length()){
            if(value[index] == '.')
                exponent = index;
            else
                digits.push_back(value[index] - '0');
            index ++;
        }
    }
    
    
    LongDouble operator* (const LongDouble& x) const{
        size_t len = digits.size() + x.digits.size();
        LongDouble result;
        result.digits = vector<int> (len,0);
        result.exponent = exponent + x.exponent;
        
        for(size_t i = 0; i < digits.size(); i++)
            for(size_t j = 0; j < x.digits.size(); j++)
                result.digits[i + j + 1] += digits[i] * x.digits[j];
        
        for(size_t i = len - 1; i > 0; i--) {
            result.digits[i - 1] += result.digits[i] / 10;
            result.digits[i] %= 10;
        }
        result.removeZeroes();
        return result;
    }
    
    LongDouble operator- (const LongDouble& x) const {
        LongDouble result;
        long exp1 = exponent;
        long exp2 = x.exponent;
        long exp = max(exp1, exp2);
        
        vector<int> d1(digits);
        vector<int> d2(x.digits);
        
        while(exp1 != exp){
            d1.insert(d1.begin(), 0);
            exp1++;
        }
        
        while(exp2 != exp){
            d2.insert(d2.begin(), 0);
            exp2++;
        }
        
        size_t size = max(d1.size(), d2.size());
        
        while (d1.size() != size)
            d1.push_back(0);
        
        while (d2.size() != size)
            d2.push_back(0);
        
        
        size_t len = 1 + size;
        result.digits = vector<int>(len,0);
        
        for(size_t i = 0; i < size; i++)
            result.digits[i+1] = d1[i] - d2[i];
        
        for (size_t i = len - 1; i > 0; i--){
            if (result.digits[i] < 0) {
                result.digits[i] +=10;
                result.digits[i - 1]--;
            }
        }
        
        result.exponent = exp + 1;
        result.removeZeroes();
        return result;
    }
    
    LongDouble operator+ (const LongDouble& x) const {
        LongDouble result;
        long exp1 = exponent;
        long exp2 = x.exponent;
        long exp = max(exp1, exp2);
        
        vector<int> d1(digits);
        vector<int> d2(x.digits);
        
        while(exp1 != exp){
            d1.insert(d1.begin(), 0);
            exp1++;
        }
        
        while(exp2 != exp){
            d2.insert(d2.begin(), 0);
            exp2++;
        }
        
        size_t size = max(d1.size(), d2.size());
        
        while (d1.size() != size)
            d1.push_back(0);
        
        while (d2.size() != size)
            d2.push_back(0);
        
        
        size_t len = 1 + size;
        
        result.digits = vector<int>(len, 0);
        
        
        for (size_t i = 0; i < size; i++)
            result.digits[i + 1] = d1[i] + d2[i];
        
        for (size_t i = len - 1; i > 0; i--) {
            result.digits[i - 1] += result.digits[i] / 10;
            result.digits[i] %= 10;
        }
        
        result.exponent = exp + 1; // восстанавливаем экспоненту
        result.removeZeroes(); // удаляем нули, как в конструкторе из строки, если таковые имеются
        
        
        return result; // возвращаем число
    }
    
    void removeZeroes() {
        size_t n = max((long) 1, exponent); // определяем, до какого момента нужно будет идти для удаления нулей справа
        
        // пока справа нули
        while (digits.size() > n && digits[digits.size() - 1] == 0)
            digits.erase(digits.end() - 1); // удаляем их
        
        // пока число цифр больше одной и вектор начинается с нуля
        while (digits.size() > 1 && digits[0] == 0) {
            digits.erase(digits.begin()); // удаляем нули из начала вектора
            exponent--; // и уменьшаем экспоненту
        }
        
        // если справа всё ещё остались нули
        while (digits.size() > 1 && digits[digits.size() - 1] == 0)
            digits.erase(digits.end() - 1); // то удалим их
        
        // если в результате осталась всего одна нулевая цифра, то превратим её в честный ноль
        if (digits.size() == 1 && digits[0] == 0) {
            exponent = 1;
        }
    }
    
    bool operator>(const LongDouble& x) const {
        
        
        if (exponent != x.exponent)
            return (exponent > x.exponent);
        
        vector<int> d1(digits);
        vector<int> d2(x.digits);
        size_t size = max(d1.size(), d2.size());
        
        while (d1.size() != size)
            d1.push_back(0);
        
        while (d2.size() != size)
            d2.push_back(0);
        
        for (size_t i = 0; i < size; i++)
            if (d1[i] != d2[i])
                return (d1[i] > d2[i]);
        
        return false;
    }
    
    bool operator==(const LongDouble& x) const {
        
        if (exponent != x.exponent)
            return false;
        
        if (digits.size() != x.digits.size())
            return false;
        
        for (size_t i = 0; i < digits.size(); i++)
            if (digits[i] != x.digits[i])
                return false;
        
        return true;
    }
    
    bool operator!=(const LongDouble& x) const {
        return !(*this == x);
    }
    
    bool operator>=(const LongDouble& x) const {
        return *this > x || *this == x;
    }
    
    
    vector<int> toString(){
        vector<int> d1(digits);
        return d1;
    }
    
    size_t countVector(){
        vector<int> d1(digits);
        return d1.size();;
    }
    
    LongDouble superRound (size_t size){
        
        size_t realSize = std::min(size,digits.size());
        vector<int> vect(digits);
        
        vector<int>::const_iterator first = vect.begin();
        vector<int>::const_iterator last = vect.begin() + realSize;
        vector<int> newVec(first, last);
        vector<int> zeroooes;
        if (exponent < 0 ) zeroooes = vector<int>(-exponent,0);
        zeroooes.insert(zeroooes.end(), newVec.begin(), newVec.end());
        
        LongDouble newLg = * new LongDouble(zeroooes, 0);
        
        return newLg;
        
    }
    
    
    LongDouble round (long size){
        cout<<" exponent  "<<exponent<<endl;
        if(size <= -exponent)
            
            return LongDouble("0");
        return LongDouble("1.0");
    }
    
    
};
