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
    echo
    printf "${YELLOW}PoxHash${NC} Code Runner by ${BLUE}Chubak Bidpaa${NC}\n"
    echo "Help:"
    printf "\tPass the name of the language as the first argument,\n"
    printf "and the strings which you wish to hash as the consecutive\n"
    printf "arguments. Supperted language names as argument are:\n\n"
    printf "${RED}py${NC} or ${RED}python${NC} or ${RED}1${NC}\n"
    printf "${YELLOW}rs${NC} or ${YELLOW}rust${NC} or ${YELLOW}2${NC}\n"
    printf "${GREEN}c${NC} or ${GREEN}3${NC}\n"
    printf "${BLUE}js${NC} or ${BLUE}javascript${NC} or ${BLUE}4${NC}\n"
    printf "${PURPLE}go${NC} or ${PURPLE}5${NC}\n"
    printf "${CYAN}nim${NC} or ${CYAN}6${NC}\n"
    echo "Runner codes accept flags between two dashes. Pass -?- to view help."
    echo "Example 1: bash ./poxh.sh c -?-"
    echo "Example 2: bash ./poxh.sh c -h-"
    echo "PoxHash was designed and first implemented by Chubak Bidpaa in March 2023"
    echo "Licensed under GPLv3"
    echo
    exit 0;
}

wrong_arg() {
    echo "Wrong argument number!$"
    exit 1;
}

check_language() {
    if [ "${language}" = "python" ] || [ "${language}" = "py" ] || [ "${language}" = "1" ]; then
        to_print="Selected: ${RED}Python${NC}\n"
        folder="python"
    elif [ "${language}" = "rust" ] || [ "${language}" = "rs" ] || [ "${language}" = "2" ]; then
        to_print="Selected: ${YELLOW}Rust${NC}\n"
        folder="rust"
    elif [ "${language}" = "c" ] || [ "${language}" = "3" ]; then
        to_print="Selected: ${GREEN}C${NC}\n"
        folder="c"
    elif [ "${language}" = "js" ] || [ "${language}" = "javascript" ] || [ "${language}" = "4" ]; then
        to_print="Selected: ${BLUE}JavaScript${NC}\n"
        folder="js"
    elif [ "${language}" = "go" ] || [ "${language}" = "5" ]; then
        to_print="Selected: ${PURPLE}Go${NC}\n"
        folder="go"
    elif [ "${language}" = "nim" ] || [ "${language}" = "6" ]; then
        to_print="Selected: ${CYAN}Nim${NC}\n"
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