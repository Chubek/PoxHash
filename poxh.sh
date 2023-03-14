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

help() {
    echo
    printf "${YELLOW}PoxHash${NC} Code Runner by ${BLUE}Chubak Bidpaa${NC}\n"
    echo "Help:"
    printf "\tPass the name of the language as the first argument,\n"
    printf "and the strings which you wish to hash as the consecutive\n"
    printf "arguments. Supperted language names as argument are:\n\n"
    printf "${RED}py${NC} or ${RED}python${NC}\n"
    printf "${YELLOW}rs${NC} or ${YELLOW}rust${NC}\n"
    printf "${BLUE}js${NC}\n or ${BLUE}javascript${NC}"
    printf "${GREEN}c${NC}\n"
    printf "${PURPLE}go${NC}\n"
    printf "${CYAN}nim${NC}\n"
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
    if [ "${language}" = "python" ] || [ "${language}" = "py" ]; then
        printf "Selected language: ${RED}Python${NC}\n"
        folder="python"
    elif [ "${language}" = "rust" ] || [ "${language}" = "rs" ]; then
        printf "Selected language: ${YELLOW}Rust${NC} (${BLD}NOTE${NC}: Compile time overhead)\n"
        folder="rust"
    elif [ "${language}" = "c" ]; then
        printf "Selected language: ${GREEN}C${NC} (${BLD}NOTE${NC}: Compile time overhead)\n"
        folder="c"
    elif [ "${language}" = "js" ] || [ "${language}" = "javascript" ]; then
        printf "Selected language: ${BLUE}JavaScript${NC}\n"
        folder="js"
    elif [ "${language}" = "go" ]; then
        printf "Selected language: ${PURPLE}Go${NC} (${BLD}NOTE${NC}: Compile time overhead)\n"
        folder="go"
    elif [ "${language}" = "nim" ]; then
        printf "Selected language: ${CYAN}Nim${NC} (${BLD}NOTE${NC}: Compile time overhead)\n"
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
        check_language
        sudo chmod +x $folder/poxh.sh && $folder/poxh.sh $@
esac