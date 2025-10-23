#include "pageio.h"
#include <stdio.h>
#include <string.h>

int main() {

    printf("=== MiniDB - Test paginare ===\n");

    if (open_data_file("data/data.db") < 0) {
        printf("Eroare: nu pot deschide fișierul data.db\n");
        return 1;
    }

    char page[PAGE_SIZE];
    memset(page, 0, PAGE_SIZE);
    strcpy(page, "Salut, aceasta este pagina 0!");

    printf("Scriu pagina 0...\n");
    write_page(0, page);

    memset(page, 0, PAGE_SIZE);
    read_page(0, page);
    printf("Conținut citit: %s\n", page);

    page_id pid = allocate_page();
    printf("Am alocat o nouă pagină cu ID = %u\n", pid);

    close_data_file();
    printf("=== Test finalizat ===\n");
    return 0;
}
