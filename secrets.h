#ifndef secrets_h
#define secrets_h

const char AWS_IOT_ENDPOINT[] = "a2xlv4pa85hm8o-ats.iot.us-east-2.amazonaws.com";

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
//pon aca el contenido de tu certificado CA1
-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
//pon aca el contenido de tu certificado de dispositivo
-----END CERTIFICATE-----
)KEY";

// Device Private Key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
//pon aca el contenido de tu llave privada
-----END RSA PRIVATE KEY-----
)KEY";

#endif
