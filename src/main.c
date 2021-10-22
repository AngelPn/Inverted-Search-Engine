#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "Entry.h"
#include "EntryList.h"

int main(){

    entry e1 = NULL, e2 = NULL, e3 = NULL, e4 = NULL;
    if (create_entry("hell", &e1) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("felt", &e2) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("henn", &e3) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("ken", &e4) == EC_FAIL)
        printf("Error! Create entry failed\n");

    entry_list el = NULL;
    if (create_entry_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    if (add_entry(el, e1) == EC_FAIL)
        printf("Error! Add entry failed\n");
    if (add_entry(el, e2) == EC_FAIL)
        printf("Error! Add entry failed\n");
    if (add_entry(el, e3) == EC_FAIL)
        printf("Error! Add entry failed\n");
    if (add_entry(el, e4) == EC_FAIL)
        printf("Error! Add entry failed\n");

    print_entry_list(el, print_entry_word);

    if (destroy_entry_list(el) == EC_FAIL)
        printf("Error! Destroy entry list failed\n");

    return 0;
}
