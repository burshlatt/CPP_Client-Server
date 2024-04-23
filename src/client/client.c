#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>

#include <asn_application.h>
#include <RRCConnectionSetup.h>

int Encode(RRCConnectionSetup_t* rcs, uint8_t** encoded) {
    asn_enc_rval_t encodeResult = der_encode_to_buffer(&asn_DEF_RRCConnectionSetup, &rcs, encoded, sizeof(encoded));

    if (encodeResult.encoded == -1) {
        return 0;
    }

    return 1;
}

void FillStruct(RRCConnectionSetup_t* rcs, int transaction_id) {
    rcs->rrc_TransactionIdentifier = transaction_id;
    rcs->criticalExtensions.present = RRCConnectionSetup__criticalExtensions_PR_c1;
    rcs->criticalExtensions.choice.c1.present = RRCConnectionSetup__criticalExtensions__c1_PR_rrcConnectionSetup_r8;
    // Заполняем дополнительные поля внутри блока choice по вашим требованиям
}

void Throw(const char *s) {
    perror(s);
    exit(1);
}

void Client() {
    uint8_t encoded_data[1024];
    long rrc_transaction_id = 123456;
    
    RRCConnectionSetup_t rrcConnectionSetup;
    memset(&rrcConnectionSetup, 0, sizeof(rrcConnectionSetup));
    FillStruct(&rrcConnectionSetup, rrc_transaction_id);

    if (Encode(&rrcConnectionSetup, &encoded_data) != 0) {
        Throw("Failed to encode RRCConnectionSetup\n");
    }

    struct sockaddr_in servaddr = {
        .sin_family = AF_INET,
        .sin_port = htons(62324),
        .sin_addr.s_addr = inet_addr("127.0.0.1"),
    };

    int conn_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

    if (conn_fd < 0) {
        Throw("socket()");
    }

    int ret = connect(conn_fd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    if (ret < 0) {
        Throw("connect()");
    }

    ret = sctp_sendmsg(conn_fd, (void *)encoded_data, strlen(encoded_data) + 1, NULL, 0, 0, 0, 0, 0, 0 );

    if (ret < 0) {
        Throw("sctp_sendmsg");
    }

    close(conn_fd);
}

int main() {
    Client();
    return 0;
}
