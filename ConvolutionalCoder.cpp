#include <iostream>
#include <bits/stdc++.h>
#include "ConvolutionalCoder.h"

    int ConvolutionalCoder::getFeedback(int feedback) {
        int index = 1;
        int res = -1;
        for (int i = 0; i < (degree + 1); i++) {
            if (feedback & index) {
                if (res == -1) {
                    res = (state & index) >> i;
                } else {
                    res ^= ((state & index) >> i);
                }
            }
            index <<= 1;
        }
        return res;
    }

    int ConvolutionalCoder::getNextValue() {
        int lowerFeedback = getFeedback(lower);
        int upperFeedback = getFeedback(upper);
        return (upperFeedback << 1) | lowerFeedback;
    }

    void ConvolutionalCoder::nextState(int inputBit) {
        inputBit <<= degree;
        state >>= 1;
        state |= inputBit;
    }

    std::string ConvolutionalCoder::binaryToString(int number, int length) {
        std::string output;
        for (int i = 0; i < length; i++) {
            if (number & 1) {
                output.insert(output.begin(), '1');
            } else {
                output.insert(output.begin(), '0');
            }
            number >>= 1;
        }
        return output;
    }

    ConvolutionalCoder::ConvolutionalCoder() {};
    
    void ConvolutionalCoder::setDegree(int dg) { degree = dg; };

    int ConvolutionalCoder::getDegree() { return degree; };
    
    void ConvolutionalCoder::setUpper(int up) { upper = up; };
    
    int ConvolutionalCoder::getUpper() { return upper; };

    void ConvolutionalCoder::setLower(int lw) { lower = lw; }
    
    int ConvolutionalCoder::getLower() { return lower; };

    std::string ConvolutionalCoder::encode(std::string input) {
        //validace vstupního řetězce
        input = validateEncodeInput(input);
        int lsb;
        std::string output;
        //Iterace přes všechny bity vstpního řetězce
        for (int i = input.size() - 1; i >= 0; i--) {
            for (int j = 0; j < 8; j++) {
                //Generování nového stavu kodéru
                lsb = input[i] & 1;
                nextState(lsb);
                //Přiřazení zakódovaného bitu na začátek výstupního řetězce
                output.insert(0, binaryToString(getNextValue(), 2));
                //Posunutí na další vstupní bit
                input[i] >>= 1;
            }
        }
        //Kódování posuvných paměťových bloků
        for (int i = 0; i < degree; i++) {
            state >>= 1;
            output.insert(0, binaryToString(getNextValue(), 2));
        }
        return output;
    };

    void ConvolutionalCoder::printTrellis(Trellis ETrellis, Trellis STrellis) {
        for (int i = 0; i < ETrellis.size(); i++) {
            std::cout << i << ": ";
            for (int j = 0; j < ETrellis[i].size(); j++) {
                std::cout << STrellis[i][j] << "," << ETrellis[i][j] << " | ";
            }
            std::cout << std::endl;
        }
    }

    int ConvolutionalCoder::hammingDistance(int val1, int val2, int n) {
        int hd = 0;
        for (int i = 0; i < n; i++) {
            hd += ((val1 & 1) ^ (val2 & 1));
            val1 >>= 1;
            val2 >>= 1;
        }
        return hd;
    }

    std::vector<int> ConvolutionalCoder::chunkInput(std::string input) {
        std::vector<int> observations;
        int pairs = input.size() / 2;
        for (int i = 0; i < pairs; i++) {
            std::string lastTwoDigits = input.substr(input.size() - 2, 2);
            observations.push_back(stoi(lastTwoDigits, NULL, 2));
            input.erase(input.size() - 2);
        }
        return observations;
    }

    int ConvolutionalCoder::getMinIndex(Trellis trellis) {
        int min = INT_MAX;
        int index = trellis.size() - 1;
        int last = (trellis[0]).size() - 1;
        for (int i = 0; i < trellis.size(); i++) {
            if (trellis[i][last] < min) {
                min = trellis[i][last];
                index = i;
            }
        }
        return index;
    }

    int ConvolutionalCoder::extractEncodedBit(int currentState) {
        return (currentState & (1 << degree)) >> degree;
    }

    std::string ConvolutionalCoder::decode(std::string input) {
        //Validace a filtrování vstupního řetězce
        input = validateDecodeInput(input);
        std::string output;
        //Počet stavů kodéru
        int numberOfStates = 1 << (degree + 1);
        //Vstupní řetězec rozdělený do dvojic
        std::vector<int> observations = chunkInput(input);

        Trellis errorTrellis(numberOfStates);
        Trellis stateTrellis(numberOfStates);

        //Inicializace řádků stavového a chybového trellis
        for (int i = 0; i < numberOfStates; i++) {
            (stateTrellis[i]).resize(observations.size(), 0);
            (errorTrellis[i]).resize(observations.size(), INT_MAX >> 1);
        }
        //Inicializace Trellis pro první dvojici, nastavujeme pouze pro stav 0, protože víme, že kodér začíná ve stavu 0
        nextState(0);
        errorTrellis[state][0] = hammingDistance(observations[0], getNextValue(), 2);
        state = 0;
        nextState(1);
        errorTrellis[state][0] = hammingDistance(observations[0], getNextValue(), 2);
        
        //Výpočet Trellis
        for (int i = 1; i < observations.size(); i++) {
            for (int j = 0; j < numberOfStates; j++) {
                for (int k = 0; k < 2; k++) {
                    //Ze stavu j získáme nový stav přečtením vstupního bitu k
                    state = j;
                    nextState(k);
                    //Výpočet Hammingovi vzdálenosti mezi pozorovanou hodnotou a hodnout, která by byla vygenerována v daném stavu s daným vstupem
                    //Přičtením k předchozí chybě získáme novou hodnotu akumulované chyby
                    int hd = errorTrellis[j][i - 1] + hammingDistance(observations[i], getNextValue(), 2);
                    //Pokud je získaná akumulovaná chyba menší nebo rovna aktuální chybě v daném stavu je předchozí cesta zrušena a nahrazena novou
                    if (hd <= errorTrellis[state][i]) {
                        stateTrellis[state][i] = j;
                        errorTrellis[state][i] = hd;
                    }
                }
            }            
        }

        //Nalezní nejlepší cesty
        int minIndexState = getMinIndex(errorTrellis);
        char messageChar = 0;

        //Průchod nejlepší cesty a dekódovaní zprávy
        for (int i = observations.size() - 1; i >= 0; i--) {
            //Do výstupního řetězce přidáváme pouze bity, které byly obsaženy ve vstupním řetězci
            //Prvních degree znaků jsou zakódované paměťové bloky
            if (i < observations.size() - degree - 1) {
                //Získání vstupního bity 
                int bit = extractEncodedBit(minIndexState);
                messageChar <<= 1;
                messageChar |= bit;
                //Přidání dekodóvaného znaku do výstupního řetězce
                if ((i % 8) == 0) {
                    output.push_back(messageChar);
                }
            }
            //Další stav na získané nejlepší cestě
            minIndexState = stateTrellis[minIndexState][i];
        }

        return output;
    };

    void printAndExit(std::string err, int exitCode){
        std::cerr << err << std::endl;
        exit(exitCode);
    }

    std::string ConvolutionalCoder::validateEncodeInput(std::string inp) {
        std::regex rgx("([A-Z][a-z][0-9])+");
        std::string res;
        //Pokud se vstupní řetězec skládá pouze z malých nebo velkých písmen anglické abecedy nebo čísel a je neprázdný, nic se s ním nedělá
        if (std::regex_match(inp, rgx)) {
            return inp;
        //Pokud obsahuje nepovolené znaky, tak je vynecháme
        } else {
            for (int i = 0; i < inp.size(); i++) {
                if (isalpha(inp[i]) || isdigit(inp[i])) {
                    res.push_back(inp[i]);
                }
            }
            //Pokud výsledný řetězec je prázdný skončí s chybou
            if (res.empty()) {
                printAndExit("Invalid input string.", 2);
            }
        }
        return res;
    }

    std::string ConvolutionalCoder::validateDecodeInput(std::string inp) {
        std::regex rgx("[01]*");
        std::string res;
        //Vstupní řetězec konvolučního kodéru s kódovým poměrem 1/2 musí být sudé délky, aby byl validním vstupem pro dekódování 
        if (inp.size() % 2 != 0) {
            printAndExit("String for decoding must contain even number of bits.", 2);
        }
        //Pokud vstup obsahuje pouze znaky 0 a 1 a je sudé délky nic se s ním neprovádí
        if (std::regex_match(inp, rgx)) {
            return inp;
        //Jinak jsou nepovolené znaky odfiltrovány
        } else {
            for (int i = 0; i < inp.size(); i++) {
                if (inp[i] == '0' || inp[i] == '1') {
                    res.push_back(inp[i]);
                }
            }
            //Pokud je vstupní řetězec po odfiltrování nevhodných hodnot liché délky skončí s chybou
            if (res.size() % 2 != 0) {
                printAndExit("Input string contained invalid characters.", 2);
            }
            //Pokud je vstupní řetězec po odfiltrování nevhodných hodnot prázdný skončí s chybou
            if (res.empty()) {
                printAndExit("Invalid input string.", 2);
            }
        }
    return res;
}
