#include "bank_service.h"
#include "ui.h"

int main() {
    using namespace bank;

    Bank bank;
    initBank(bank);

    runMainMenu(bank);

    destroyBank(bank);
    return 0;
}
