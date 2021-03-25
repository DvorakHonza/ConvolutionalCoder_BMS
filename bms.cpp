#include <iostream>
#include <getopt.h>
#include <regex>
#include <ctype.h>

#include "ConvolutionalCoder.h"

void printErrorHelpAndExit(std::string err, int exitCode){
    std::cerr << err << " Try -h" << std::endl;
    exit(exitCode);
}

int main(int argc, char **argv) {
    int opt;
    char mode = '-';
    std::string input;
    std::string output;
    ConvolutionalCoder coder;

    if (argc < 2 ) {
        printErrorHelpAndExit("Missing parameters", 2);
    }

    while ((opt = getopt(argc, argv, "hedX:Y:Z:")) != -1) {
        switch(opt) {
            case 'h':
                std::cerr << "Usage:" << std::endl;
                std::cerr << "  ./bms {-e|-d} [-X {degree}, -Y {upper}, -Z {lower}]" << std::endl;
                std::cerr << "    -e\t\tEncode string from standard input" << std::endl;
                std::cerr << "    -d\t\tDecode string from standard input" << std::endl;
                std::cerr << "    -X degree\tSet number of delay units to degree" << std::endl;
                std::cerr << "    -Y upper\tSet upper feedback to upper" << std::endl;
                std::cerr << "    -Z lower\tSet lower feedback to lower" << std::endl;
                std::cerr << "    Values degree, upper, lower are expected to be positive integer values." << std::endl;
                exit(2);
                break;
            case 'e':
                if (mode == '-' || mode == 'e') {
                    mode = 'e';
                } else {
                    printErrorHelpAndExit("Parameters -e and -d cannot be used simultaneously.", 2);
                }
                break;

            case 'd':
                if (mode == '-' || mode == 'd') {
                    mode = 'd';
                } else {
                    printErrorHelpAndExit("Parameters -e and -d cannot be used simultaneously.", 2);
                }
                break;

            case 'X':
                //Nastavení počtu posuvných paměťových bloků
                if (optarg <= 0) {
                    printErrorHelpAndExit("Coder has to have more than 0 feedback units.", 2);
                }
                coder.setDegree(strtol(optarg, NULL, 10));
                break;
            
            case 'Y':
                //Nastavení horní zpětné vazby
                coder.setUpper(strtol(optarg, NULL, 10));
                break;

            case 'Z':
                //Nastavení dolní zpětné vazby
                coder.setLower(strtol(optarg, NULL, 10));
                break;

            default:
                printErrorHelpAndExit("Unknown parameter.", 2); 
        }
    }

    //Čtení ze stdin
    std::getline(std::cin, input);

    //Podle specifikovaného módu se provede daná operace a výstup se vytiskne na standardní výstup
    if (mode == 'e') {
        std::cout << coder.encode(input) << std::endl;
    } else {
        std::cout << coder.decode(input) << std::endl;
    }
    
    return 0;
}