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
    printf "\t${GREEN}c${NC} ||  ${GREEN}cc${NC} || ${GREEN}1${NC}\n"
    printf "\t${PURPLE}g${NC} || ${PURPLE}go${NC} || ${PURPLE}2${NC}\n"
    printf "\t${CYAN}n${NC} || ${CYAN}nim${NC} || ${CYAN}3${NC}\n\n"
    printf "\t${YELLOW}r${NC} || ${YELLOW}rs${NC} || ${YELLOW}rust${NC} or ${YELLOW}4${NC}\n"
    printf "\t${RED}p${NC} || ${RED}py${NC} || ${RED}python${NC} || ${RED}5${NC}\n"
    printf "\t${BLUE}j${NC} || ${BLUE}js${NC}  || ${BLUE}javascript${NC} || ${BLUE}6${NC}\n"
    echo "Runner codes accept flags between two dashes. Pass -?- to view help."
    echo -e "Example 1: ${BLD}bash ./poxh.sh c -?-${NC}"
    echo -e "Example 2: ${BLD}bash ./poxh.sh c -h- myString${NC}"
    echo -e "Example 3: ${BLD}bash ./poxh.sh 2 -zb- myString${NC}"
    echo "You can pass -s as the second argument after the language name"
    echo "Passing -s will stop the script from echoing the language name"
    echo -e "Example 4: ${BLD}bash ./poxh.sh -s -*- myString1 myString1${NC}"
    echo -e "Example 5: ${BLD}for i in {1..6}; do bash ./poxh.sh $i -h- str1 str2; done${NC}"
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
        if [[ "${1}" == "-s" ]]; then shift; else printf "${to_print}"; fi
        sudo chmod +x $folder/poxh.sh && $folder/poxh.sh $@
        ;;
esac