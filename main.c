#include <stdio.h>
#include <stdlib.h>

#include "resistors.h"

#define INPUT_LEN 10

#define PRINT_TOTAL(x)  printf("Total combinations found: %d\n\n\n", x); \
                        x = 0;

// Lazy input check routine
char check_input_value(char* strin){
    char idx = 0;
    for(idx=0; idx<INPUT_LEN && (*(strin+idx)!= '\0'); idx++){
        if((*(strin+idx) < '0') || (*(strin+idx) > '9')){
            if((*(strin+idx) == ',')){
                *(strin+idx) = '.';
            }
            if(*(strin+idx) != '.'){
                return 0;
            }
        }
    }
    return 1;
}

int main(int argc, char** argv){
    // Ask needed resistor value
    printf("Wanted value (in Ohms): ");
    char cVal[INPUT_LEN];
    scanf("%s", &cVal);
    if(!check_input_value(cVal)){
        printf("entry error, not a valid number!\n");
        return 0;
    }
    double dValue = atof(cVal);

    // Ask needed tolerance for res research in database
    printf("Wanted search tolerance (in percents): ");
    char cSearchTol[INPUT_LEN];
    scanf("%s", &cSearchTol);
    if(!check_input_value(cSearchTol)){
        printf("entry error, not a valid number!\n");
        return 0;
    }
    double dSearchTol= atof(cSearchTol);

    // Process value to find's limits
    double dTolMin = (double) ( dValue - (dValue * dSearchTol / 100) );
    double dTolMax = (double) ( dValue + (dValue * dSearchTol / 100) );

    // Initialize variables
    int idx_srch = 0, idx_curr = 0, iFound = 0;
    double res = 0.0;
    eSerID eID = eE12;
    int nb_of_structs = get_nb_series();

    // Search
    for(eID = eE12; eID < nb_of_structs; eID++){      //loop structs
        //Single resistor topo search
        printf("*** Result(s) for single-resistor topography %s: ***\n", serName(eID));
        //loop elems array in struct
        for(idx_curr = 0; idx_curr < stResSets[eID].iNbElems; idx_curr++){
            res = stResSets[eID].cdElems[idx_srch];
            if((res >= dTolMin) && (res <= dTolMax)){
                iFound += 1;
                printf(" -> value\t%.2lf Ohms\tfound\n", res);
            }
        }
        PRINT_TOTAL(iFound);

        //Series resistors topo search
        printf("*** Result(s) for double-resistor topography in serie %s: ***\n", serName(eID));
        //loop elems array in struct (reference res)
        for(idx_curr = 0; idx_curr < stResSets[eID].iNbElems; idx_curr++){
            //loop elems array in struct (search res)
            for(idx_srch = idx_curr; idx_srch < stResSets[eID].iNbElems; idx_srch++){
                res = stResSets[eID].cdElems[idx_curr] + stResSets[eID].cdElems[idx_srch];
                if((res >= dTolMin) && (res <= dTolMax)){
                    iFound += 1;
                    printf(" -> values\t%.2lf\t\tand\t%.2lf\tgive: %.2lf Ohms\n", stResSets[eID].cdElems[idx_curr],
                           stResSets[eID].cdElems[idx_srch], res);
                }
            }
        }
        PRINT_TOTAL(iFound);

        //Parallel resistors topo search
        printf("*** Result(s) for double-resistor topography in parallel %s: ***\n", serName(eID));
        //loop elems array in struct (reference res)
        for(idx_curr = 0; idx_curr < stResSets[eID].iNbElems; idx_curr++){
            //loop elems array in struct (search res)
            for(idx_srch = idx_curr; idx_srch < stResSets[eID].iNbElems; idx_srch++){
                res =   (stResSets[eID].cdElems[idx_curr] * stResSets[eID].cdElems[idx_srch]) /
                        (stResSets[eID].cdElems[idx_curr] + stResSets[eID].cdElems[idx_srch]);
                if((res >= dTolMin) && (res <= dTolMax)){
                    iFound += 1;
                    printf(" -> values\t%.2lf\tand\t%.2lf\tgive: %.2lf Ohms\n", stResSets[eID].cdElems[idx_curr],
                           stResSets[eID].cdElems[idx_srch], res);
                }
            }
        }
        PRINT_TOTAL(iFound);
    }
    return 0;
}
