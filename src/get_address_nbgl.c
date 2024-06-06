#ifdef HAVE_NBGL
#include "get_address.h"
#include "utils.h"
#include "crypto.h"
#include "menu.h"
#include "nbgl_use_case.h"

static uint32_t _account = 0;
static char     _bip44_path[27]; // max length when 44'/12586'/4294967295'/0/0
static char     _address[MINA_ADDRESS_LEN];

typedef struct {
    nbgl_layoutTagValue_t tagValuePair[3];
    nbgl_layoutTagValueList_t tagValueList;
    nbgl_pageInfoLongPress_t infoLongPress;
} TransactionContext_t;

static TransactionContext_t transactionContext;

static uint8_t set_result_get_address(void)
{
    uint8_t tx = 0;
    memmove(G_io_apdu_buffer + tx, _address, sizeof(_address));
    tx += sizeof(_address);
    return tx;
}

static void confirmation_callback(bool confirm) {
    if (confirm) {
        sendResponse(set_result_get_address(), true),
        nbgl_useCaseReviewStatus(STATUS_TYPE_ADDRESS_VERIFIED, ui_idle);
    }
    else {
        sendResponse(0, false);
        nbgl_useCaseReviewStatus(STATUS_TYPE_ADDRESS_REJECTED, ui_idle);
    }
}

void ui_get_address(uint8_t *dataBuffer) {
    _address[0] = '\0';
    _account = read_uint32_be(dataBuffer);

    strncpy(_bip44_path, "44'/12586'/", sizeof(_bip44_path));              // used 11/27 (not counting null-byte)
    value_to_string(&_bip44_path[11], sizeof(_bip44_path) - 11, _account); // at most 21/27 used (max strnlen is 10 when _account = 4294967295)
    strncat(_bip44_path, "'/0/0", 6);                                      // at least 27 - 21 = 6 bytes free (just enough)

    gen_address(_account, _address);

    transactionContext.tagValuePair[0].item = "Path";
    transactionContext.tagValuePair[0].value = _bip44_path;

    transactionContext.tagValueList.nbPairs = 1;
    transactionContext.tagValueList.pairs = transactionContext.tagValuePair;

    #ifdef HAVE_ON_DEVICE_UNIT_TESTS
        nbgl_useCaseSpinner("Unit tests ...");
    #else
        nbgl_useCaseAddressReview(_address,
                                  &transactionContext.tagValueList,
                                  &C_Mina_64px,
                                  "Verify Mina\naddress",
                                  NULL,
                                  confirmation_callback);
    #endif


}

#endif // HAVE_NBGL
