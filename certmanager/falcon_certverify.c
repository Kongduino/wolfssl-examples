/* falcon_certverify.c
 *
 * Copyright (C) 2021 wolfSSL Inc.
 *
 * This file is part of wolfSSL.
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */

#include <stdio.h>
#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/error-crypt.h>
#include <wolfssl/test.h>

int main(void)
{
    int ret;
    WOLFSSL_CERT_MANAGER* cm = NULL;

    const char* caCert     = "./falcon1024_root_cert.pem";
    const char* verifyCert = "./falcon1024_entity_cert.pem";

    wolfSSL_Init();
#ifdef DEBUG_WOLFSSL
    wolfSSL_Debugging_ON();
#endif

    cm = wolfSSL_CertManagerNew();
    if (cm == NULL) {
        printf("wolfSSL_CertManagerNew() failed\n");
        return -1;
    }

    wolfSSL_CertManagerSetVerify(cm, myVerify);

    ret = wolfSSL_CertManagerLoadCA(cm, caCert, NULL);
    if (ret != SSL_SUCCESS) {
        printf("wolfSSL_CertManagerLoadCA() failed (%d): %s\n",
                ret, wolfSSL_ERR_reason_error_string(ret));
        ret = -1; goto exit;
    }

    ret = wolfSSL_CertManagerVerify(cm, verifyCert, SSL_FILETYPE_PEM);
    if (ret != SSL_SUCCESS) {
        printf("wolfSSL_CertManagerVerify() failed (%d): %s\n",
                ret, wolfSSL_ERR_reason_error_string(ret));
        ret = -1; goto exit;
    }
    printf("Verification Successful!\n");

exit:
    wolfSSL_CertManagerFree(cm);
    wolfSSL_Cleanup();

    return ret;
}
