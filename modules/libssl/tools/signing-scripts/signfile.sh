#sign using an RSA key

openssl sha1 -binary $1 >digest.bin
openssl rsautl -inkey verify_key.priv -in digest.bin -sign -pkcs -out sign.bin
openssl base64 -e -in sign.bin -out sign.b64

perl createsig.pl '//' sign.b64 $1 $2

#openssl rsautl -inkey verify_key.priv -in sign.bin -verify -pkcs -out digest_out.bin
openssl rsautl -inkey verify_key.b64 -pubin -in sign.bin -verify -pkcs -out digest_out.bin
cmp digest_out.bin digest.bin
