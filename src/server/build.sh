rm -rf asn1_source build
mkdir asn1_source build
asn1c RRCConnectionSetupComplete.asn1 -D asn1_source -no-gen-OER -no-gen-example
gcc -Iasn1_source server.c asn1_source/*.c -o build/server -lsctp -DPDU=RRCConnectionSetupComplete -DASN_DISABLE_OER_SUPPORT
