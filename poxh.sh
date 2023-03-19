#!/bin/bash

RED="\033[1;31m"
GREEN="\033[1;32m"
YELLOW="\033[1;33m"
BLUE="\033[1;34m"
PURPLE="\033[1;35m"
CYAN="\033[1;36m"
NC="\033[0m"
BLD="\033[1m"
language=""
folder=""
to_print=""
shifted_next=""

help() {
    printf "${YELLOW}PoxHash${NC} Code Runner by  Chubak Bidpaa -- Help\n"
    printf "Pass the name of the language as the first argument,\n"
    printf "The second argument should be the flags to the runner program\n"
    printf "and then the strings which you wish to hash as the consecutive\n"
    printf "arguments. Supperted language names as argument are:\n\n"
    echo "Note: language aliases are case-insensetive"
    printf "Possible aliases for C: ${BLD}c, cc, 1${NC}\n"
    printf "Possible aliases for Go: ${BLD}g, go, 2${NC}\n"
    printf "Possible aliases for Nim: ${BLD}n, nim, 3${NC}\n"
    printf "Possible aliases for Rust: ${BLD}r, rs, rust, 4${NC}\n"
    printf "Possible aliases for Python: ${BLD}p, py, python, 5${NC}\n"
    printf "Possible aliases for JavaScript: ${BLD}j, js, javascript, 6${NC}\n\n"
    echo "Runner codes accept flags between two dashes. Pass -?- to view help."
    printf "Example 1: ${BLD}bash ./poxh.sh c -?-${NC}\n"
    printf "Example 2: ${BLD}bash ./poxh.sh py -h- myString${NC}\n"
    printf "Example 3: ${BLD}bash ./poxh.sh 2 -zb- myString${NC}\n"
    echo
    echo "You can pass -s as the second or third argument after the language alias"
    echo "You can pass -c as the second or third argument after the language alias"
    echo "Passing -s will stop the script from echoing the language name"
    echo "Passing -s will force recompilation, if possible"
    echo
    echo "PoxHash was designed and first implemented by Chubak Bidpaa in March 2023"
    echo "Licensed under MIT"
    echo
    exit 0;
}

wrong_arg() {
    echo "Wrong argument number!$"
    exit 1;
}

check_language() {
    if [ "${language}" = "python" ] || [ "${language}" = "p" ] || [ "${language}" = "py" ] || [ "${language}" = "5" ]; then
        to_print="+${RED}Python${NC}\n"
        folder="python"
    elif [ "${language}" = "rust" ] || [ "${language}" = "r" ] || [ "${language}" = "rs" ] || [ "${language}" = "4" ]; then
        to_print="+${YELLOW}Rust${NC}\n"
        folder="rust"
    elif [ "${language}" = "c" ] || [ "${language}" = "cc" ]  || [ "${language}" = "1" ]; then
        to_print="+${GREEN}C${NC}\n"
        folder="c"
    elif [ "${language}" = "js" ] || [ "${language}" = "j" ] || [ "${language}" = "javascript" ] || [ "${language}" = "6" ]; then
        to_print="+${BLUE}JavaScript${NC}\n"
        folder="js"
    elif [ "${language}" = "go" ] || [ "${language}" = "g" ] || [ "${language}" = "2" ]; then
        to_print="+${PURPLE}Go${NC}\n"
        folder="go"
    elif [ "${language}" = "nim" ] || [ "${language}" = "n" ] || [ "${language}" = "3" ]; then
        to_print="+${CYAN}Nim${NC}\n"
        folder="nim"
    else
        printf "${RED}Wrong argument as language passed!${NC}\n"
        echo "Pass -h to the script or run it without any args to view help"
        exit 1;
    fi
}

case "$#" in
    0) help;;
    1) if [[ "${1}" == "-h" ]]; then help; else wrong_arg; fi;;
    *)  
        language=$1 && shift
        language=$(printf "${language}" | tr '[:upper:]' '[:lower:]')        
        check_language
        
        silent="0"
        compile="0"
        for arg in $1 $2; do
            if [ "$arg" = "-s" ]; then
                if [ "$silent" = "1" ]; then 
                    printf "${RED}Error:${NC} can pass -s and -c twice"
                    exit 1;
                fi
                silent="1"
                shift
            elif [ "$arg" = "-c" ]; then
                if [ "$compile" = "1" ]; then 
                    printf "${RED}Error:${NC} can pass -s and -c twice"
                    exit 1;
                fi
                compile="1"
                shift
            fi                
        done

        if [ "$silent" = "0" ]; then
            printf $to_print
            echo
        fi

        export COMPILE=$compile && sudo chmod +x $folder/poxh.sh && $folder/poxh.sh $@
        ;;
esac