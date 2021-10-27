#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "Entry.h"
#include "EntryList.h"
#include "../mylib/BKTree/BKTree.h"

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

    //BK_Tree test
    BK_tree ix = NULL;
    build_entry_index(&el,MT_EXACT_MATCH,&ix);
    print_BK_tree(ix);

    printf("Find words similar to henn with max distance 2:\n");

    entry_list result;
    lookup_entry_index("henn", ix, 2, &result);
    print_entry_list(result, print_entry_word);

    destroy_entry_list(result);
    destroy_entry_index(ix);

    if (destroy_entry_list(el) == EC_FAIL)
        printf("Error! Destroy entry list failed\n");

    /****************** lookup test *************************/
    printf("\n\n\n");
    /* add every word mentioned in the example to an entry list */
    entry e = NULL;
    entry_list entryList; create_entry_list(&entryList, destroy_entry);
    create_entry("melt", &e); add_entry(entryList, e);
    create_entry("small", &e); add_entry(entryList, e);
    create_entry("fell", &e); add_entry(entryList, e);
    create_entry("felt", &e); add_entry(entryList, e);
    create_entry("fall", &e); add_entry(entryList, e);
    create_entry("help", &e); add_entry(entryList, e);
    create_entry("hell", &e); add_entry(entryList, e);

    /* build a BK tree */
    BK_tree index = NULL;
    build_entry_index(&entryList,MT_EXACT_MATCH,&index);
    print_BK_tree(index);

    printf("Find words similar to henn with max distance 2:\n");

    entry_list res;
    lookup_entry_index("henn", index, 2, &res);
    print_entry_list(res, print_entry_word);

    destroy_entry_list(res);
    destroy_entry_index(index);
    destroy_entry_list(entryList);
    /********************************************************/


    return 0;
}
