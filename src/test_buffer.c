#include "buffer.h"
#include <stdio.h>
#include <string.h>

int main()
{
    printf("=== Test Buffer Cache (LRU) ===\n");

    open_data_file("data/data.db");
    buffer_init(4); // doar 4 cadre

    // Scriem în paginile 0,1,2,3
    for (int i = 0; i < 4; i++) {
        char *p = buffer_get_page(i, true);
        sprintf(p, "Pagina %d din cache", i);
        buffer_unpin_page(i, true);
    }

    // Forțăm eviction (introducem pagina 4)
    char *p4 = buffer_get_page(4, true);
    sprintf(p4, "Pagina nouă (eviction test)");
    buffer_unpin_page(4, true);

    buffer_close();
    close_data_file();

    printf("=== Test finalizat ===\n");
    return 0;
}
