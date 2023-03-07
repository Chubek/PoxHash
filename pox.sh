RED="\033[1;31m"
GREEN="\033[1;32m"
YELLOW="\033[1;33m"
BLUE="\033[1;34m"
PURPLE="\033[1;35m"
CYAN="\033[1;36m"
NC="\033[0m"
language=""

help() {
    echo
    printf "${YELLOW}PoxHash${NC} Code Runner by ${BLUE}Chubak Bidpaa${NC}\n"
    echo "Help:"
    printf "\tPass the name of the language as the first argument,\n"
    printf "and the strings which you wish to hash as the consecutive\n"
    printf "arguments. Supperted language names as argument are:\n\n"
    printf "\t${RED}python${NC}\t${GREEN}c${NC}\t${YELLOW}rust${NC}\n"
    printf "\t${BLUE}js${NC}\t${PURPLE}go${NC}\t${CYAN}nim${NC}\n\n"
    printf "\tExample: ./poxhash.sh python HashThis\n\n"
    echo "PoxHash was designed and first implemented by Chubak Bidpaa in 2023"
    echo "Licensed under GPLv3"
    echo
    exit 0;
}

wrong_arg() {
    echo "${RED}Wrong argument number!${NC}"
    exit 1;
}

check_language() {
    if [[ "${language}" == "python" ]]; then
        printf "Selected language: ${RED}Python${NC}\n"
    elif [[ "${language}" == "rust" ]]; then
        printf "Selected language: ${YELLOW}Rust${NC}\n"
    elif [[ "${language}" == "c" ]]; then
        printf "Selected language: ${GREEN}C${NC}\n"
    elif [[ "${language}" == "js" ]]; then
        printf "Selected language: ${BLUE}JavaScript${NC}\n"
    elif [[ "${language}" == "go" ]]; then
        printf "Selected language: ${PURPLE}Go${NC}\n"
    elif [[ "${language}" == "nim" ]]; then
        printf "Selected language: ${CYAN}Nim${NC}\n"
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
        sudo chmod +x $language/pox.sh && $language/pox.sh $@
esac