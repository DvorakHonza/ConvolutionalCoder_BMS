/**
 * @author Jan Dvořák xdvora2m at stud.fit.vutbr.cz
 * 
 * @brief Hlavičkový soubor třídy ConvolutionalCoder reprezentující konvoluční (de)kodér
 */

#ifndef CONVCODER_H
#define CONVCODER_H

#include <string>
#include <vector>

class ConvolutionalCoder {
    /**
     * @brief Datový typ reprezentující strukturu Trellis
    */
    typedef std::vector<std::vector<int>> Trellis;
    private:
        int degree = 5;
        int upper = 53;
        int lower = 46;
        int state = 0;

        /**
         * @brief Funkce getFeedback() získá podle aktuálního stavu a hodnoty zpětné vazby zakódovaný bit 
         * 
         * @param feedback Bitová maska zpětné vazby
         * @return Zakódovaný bit
         */
        int getFeedback(int feedback);

        /**
         * @brief Zkombinuje bity získané funkcí getFeedback() pro spodní a horní zpětnou vazbu
         * 
         * @return Zakódovanou podobu vstupního bitu
         */
        int getNextValue();

        /**
         * @brief Aktualizuje hodnotu state podle dalšího vstupního bitu
         * 
         * @param inputBit Další vstupní bit
         */
        void nextState(int inputBit);
        
        /**
         * @brief Vytiskne stav struktury Trellis složené ze stavového a chybového Trellis ve tvaru: | předchozí stav, akumulovaná chyba | 
         * 
         * @param ETrellis Trellis obsahující akumulované chyby pro jednotlivé stavy a vstupní hodnoty
         * @param STrellis Trellis obsahující předchozí stavy pro jednotlivé vstupní hodnoty
         */
        void printTrellis(Trellis ETrellis, Trellis STrellis);

        /**
         * @brief Spočítá Hammingovu vzdálenost val1 a val2 na spodních n bitech
         * 
         * @param val1 První vstupní operand
         * @param val2 Druhá vstupní operand
         * @param n Počet spodních bitů, na kterých bude Hammingova vzdálenost počítána
         * @return Výslednou Hammingovu vzdálenost
         */
        int hammingDistance(int val1, int val2, int n);

        /**
         * @brief Rozdělí vstupní řetězec pro dekódování na dvojice nul a jedniček a následně je převede na číselnou hodnotu
         * 
         * @param input Vstupní řetězec pro dekódovaní
         * @return Vektor číselných hodnot odpovídajících vstupnímu řetězci
         */
        std::vector<int> chunkInput(std::string input);
        
        /**
         * @brief Projde poslední sloupec trellis a vrátí index řádku, na kterém se nachází minimální akumulovaná chyba. V případě shodujících se hodnot upředdnostní vyšší index.
         * 
         * @param trellis Trellis s akumulovanými chybami
         * @return Index řádku s minimální chybou
         */
        int getMinIndex(Trellis trellis);

        /**
         * @brief Z currentState odvodí hodnotu bitu, která byla zakódována
         * 
         * @param currentState Stav kodéru před zpracováním vstupního bitu
         * @return Hodnota vstupního bitu
         */
        int extractEncodedBit(int currentState);

        /**
         * @brief Validuje vstupní řetězec a případně filtruje nepovolené znaky
         * 
         * @param inp Vstupní řetězec
         * @return Filtrovaný vstupní řetězec
         */
        std::string validateEncodeInput(std::string inp);

         /**
         * @brief Validuje vstupní řetězec a případně filtruje nepovolené znaky
         * 
         * @param inp Vstupní řetězec
         * @return Filtrovaný vstupní řetězec
         */
        std::string validateDecodeInput(std::string inp);

        /**
         * @brief Převede spodních length bitů binární podoby number na řetězec
         * 
         * @param number Převáděné číslo
         * @param length Délka výsledného řetězce
         * @return Binární podoba number ve formě řetězce
         */
        std::string binaryToString(int number, int length);

    public:
        ConvolutionalCoder();    
        void setDegree(int dg);
        int getDegree();   
        void setUpper(int up);   
        int getUpper();
        void setLower(int lw);  
        int getLower();

        /**
         * @brief Zakóduje vstupní řetězec konvolučním kodérem s konfigurací (degree, upper, lower)
         * 
         * @param input Vstupní řetězec
         * @return Zakódovaný vstupní řetězec
         */
        std::string encode(std::string input);

        /**
         * @brief Dekóduje vstupní řetězec zakódovaný koncolučním kodérem s konfigurací (degree, upper, lower)
         * 
         * @param input Vstupní řetězec
         * @return Dekódovaný vstupní řetězec
         */
        std::string decode(std::string input);
};

#endif //CONVCODER_H