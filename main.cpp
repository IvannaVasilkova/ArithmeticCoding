//
//  main.cpp
//  ArithCoding
//
//  Created by Ivanna Vasilkova on 11.10.2020.
//  Copyright © 2020 Ivanna Vasilkova. All rights reserved.
//
#include "includes.h"
#include "LongDouble.cpp"




std::ostream& operator<<(std::ostream& os, const LongDouble& value) {
    
    
    if (value.exponent > 0) {
        size_t i = 0;
        size_t e = value.exponent;
        
        while(i < value.digits.size() && i < e)
            os << value.digits[i++];
        
        while (i < e) {
            os << "0";
            i++;
        }
        
        if (i < value.digits.size()) {
            os << ".";
            
            while(i < value.digits.size())
                os << value.digits[i++];
        }
    }
    else if (value.exponent == 0) {
        os << "0.";
        
        for (size_t i = 0; i < value.digits.size(); i++)
            os << value.digits[i];
    }
    else {
        os << "0.";
        
        for (long i = 0; i < -value.exponent; i++)
            os << "0";
        
        for (size_t i = 0; i < value.digits.size(); i++)
            os << value.digits[i];
    }
    
    return os;
}

MapLD partition(MapLD mLD, LongDouble start, LongDouble end){
    MapLD newMLd;
    LongDouble pizza = start;
    LongDouble lenght = end - start;
    for(auto testo : mLD){
        pizza = testo.second * lenght + pizza;
        newMLd[testo.first] = pizza;
        
    }
    return newMLd;
}

Pair neStep(MapLD mld, Pair interval, string symbolchik){
    Pair newInterval;
    MapLD currMap = partition(mld, interval.first, interval.second);
    LongDouble len = interval.second - interval.first;
    len = len*mld[symbolchik];
    newInterval.second = currMap[symbolchik];
    newInterval.first = newInterval.second - len;
    return newInterval;
}

Pair step(MapLD mld, Pair interval, LongDouble number){
    Pair newInterval;
    MapLD currMap = partition(mld, interval.first, interval.second);
    LongDouble len = interval.second - interval.first;
    for (auto i: currMap) {
        string symbolchik = i.first;
        len = len*mld[symbolchik];
        newInterval.second = currMap[symbolchik];
        newInterval.first = newInterval.second - len;
        if (number > newInterval.first && !(number > newInterval.second))
            return newInterval;
    }
    return Pair();
}

LongDouble average(Pair local){
    LongDouble difference;
    LongDouble fid;
    LongDouble average;
    LongDouble half = *new LongDouble("0.5");
    difference = local.second - local.first;
    fid = difference * half;
    average = local.second - fid;
    
    
////    int i = 0;
////    while(local.first.digits[i] == local.second.digits[i]) i++;
////    i-= local.first.exponent;
////    cout<<"i : "<<i<<endl;
//    LongDouble average = local.second - local.first;
////    cout<<average.exponent<<endl;
//    long i = -average.exponent;
////    cout<< local.first.supeRound(i)<<local.first.exponent<<endl;
//    average = local.second + local.first;
//    average = average * LongDouble("0.5");
////    cout<<"average.round(i)"<<average.supeRound(i+1)<<endl;
//    return average.superRound(i+1);
    return average;
    
}


void Encode(string pathInitial, int countSymbols, string pathFinite){
    
    //READ FROM FILE
    
    ifstream f(pathInitial, ios::out | ios::binary);
    map<string,double> m;
    map<string, LongDouble> m1;
    map <string, double>::iterator j;
    MapInt::iterator b;
    
    
    map <string, double>::iterator i;
    
    int count = -1;
    //    double accumulatedAmount = 0.0;
    while (!f.eof())
    {
        char str[countSymbols+1];
        f.get(str, countSymbols+1);
        count++;
        string c = string(str);
        m[c]++;
        
    }
    f.close();
    f.open(pathInitial);
    std::ofstream N;          // поток для записи
    N.open(pathFinite, ios::out | ios::binary);
    m.erase("");
    for (i = m.begin(); i!=m.end(); ++i){
        
        i->second = i->second/count;
        cout<<i->first<<":"<<i->second<<endl;
        N << i->first;
        N<<"#";
        N<<i->second;
        N << "\n";
    }
    N<<"#";
    
    
    for (auto i: m) {
        string a = i.first;
        m1[a] = LongDouble(to_string(i.second));
    }
    Pair local(LongDouble("0.0"),LongDouble("1.0"));
    while (!f.eof()){
        char str[countSymbols+1];
        f.get(str, countSymbols+1);
        string c = string(str);
        if(c!="")
            local = neStep(m1, local, c);
    }
//    cout<<local.first<<":"<<local.second<<endl;
    LongDouble example = average(local);
    LongDouble halp = *new LongDouble("0.5");
    LongDouble tmp = *new LongDouble("0.5");
    LongDouble zero = *new LongDouble("0");
    vector<bool> byte;
    
    size_t cnt = example.countVector()-1;
//    cout<<cnt;
    cout<<'\n';
    int cofkoekf = 0;
    while (example.round(cnt)!= zero){
          cout<<"______________________________"<<endl;
        if (example >= tmp){
            cofkoekf++;
            example = example - tmp;
            cout<<"example   "<<example<<endl;
            cout<<"tmp   "<<tmp<<endl;
            cout<<"example - tmp   "<<example - tmp<<endl;
              if (cofkoekf==25) break;
            byte.push_back(1);
        }
        else {
            cofkoekf++;
           cout<<"example else  "<<example<<endl;
            byte.push_back(0);
            
        }
        tmp = halp * tmp;
//        cout<<example<<endl;
    }
    for (bool i: byte) cout<<i;
    
    char buf = 0;
    int bitNum = 0;
    for(int n=0; n<byte.size(); n++)
    {buf = buf | byte[n]<<(7-bitNum);
        bitNum++;
        if (bitNum==8) { bitNum=0;   N<<buf; buf=0; }
    }
    
    if (bitNum) {
        N<<buf;
    }
    N.close();
}

void Decode(string pathFinite, string pathDecode){
    MapLD map;
    Pair local(LongDouble("0.0"),LongDouble("1.0"));
    std::ifstream input(pathFinite);
       std::string key, value;
       std::getline(input, key, '#');
         std::cout << '\n';
       while (key != ""){
           std::getline(input, value);
           std::cout << "key --  " <<key << " value --  " << value << '\n';
           map[key] = value;
          
           std::cout << '\n';
           std::getline(input, key, '#');
       }
    std::ofstream M;  M.open(pathDecode);
    
    
    
    setlocale(LC_ALL,"Russian");
    vector<bool> boolbool;
    char byte;
    byte = input.get();
    while(!input.eof())
    {
        for(int i = 0; i < 8; i++)
            boolbool.push_back(byte & (1 << (7-i) ));
        byte = input.get();
        
    }
    
    input.close();
//    step(map, local, <#LongDouble number#>);
    
}



int main(int argc, const char * argv[]) {
    string pathInitial = "3.txt";
    int countSymbols = 1;
    string pathFinite ="output.txt";
    string pathDecode = "final.txt";
    Encode(pathInitial,countSymbols, pathFinite);
     Decode(pathFinite, pathDecode);
}









//    char tmp = 0;
//    bool right = false;
//
//    for(int i = 0; i < vect.size(); i++){
//        if (right){
//            tmp = mapByte[vect[i]] | tmp;
//            N<<tmp;
//        }
//        else {tmp = (mapByte[vect[i]] <<4) | tmp;
//            N<<tmp;
//        }
//        right = !right;
//    }
