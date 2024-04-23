rm -rf asn1_source build
mkdir asn1_source build
asn1c rrc.asn1 -D asn1_source -pdu=RRCConnectionSetup -no-gen-OER -no-gen-example -fcompound-names -fno-include-deps
gcc -Iasn1_source client.c asn1_source/*.c -o build/client -lsctp -DPDU=RRCConnectionSetup -DASN_DISABLE_OER_SUPPORT
