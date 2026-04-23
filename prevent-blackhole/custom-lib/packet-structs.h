#ifndef PACKET_STRUCTS_H
#define PACKET_STRUCTS_H

#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/netstack.h"

#define IPV6_PROTO_BHVERIF 253 // a protocol number used in experiments by RFC3692
#define UDP_PORT_BHVERIF 12036 // a custom UDP port used in experiments for blackhole verification packets

#define VERIFICATION_REQ_TYPE 1
#define SUSPECT_VERIFICATION_REQ_TYPE 2
#define SUSPECT_CLEAN_RESP_TYPE 3
#define VERIFICATION_CONF_TYPE 4

#define UIP_BUF_BH_VERIF     ((struct generic_bh_verification_packet *)UIP_IP_PAYLOAD(uip_ext_len + UIP_UDPH_LEN))

#define UIP_BUF_VER_REQ     ((struct verification_request_packet *)UIP_IP_PAYLOAD(uip_ext_len + UIP_UDPH_LEN))
#define UIP_BUF_SUSPECT_VER_REQ ((struct suspect_verification_request_packet *)UIP_IP_PAYLOAD(uip_ext_len + UIP_UDPH_LEN))
#define UIP_BUF_SUSPECT_CLEAN_RESP ((struct suspect_clean_response_packet *)UIP_IP_PAYLOAD(uip_ext_len + UIP_UDPH_LEN))
#define UIP_BUF_VER_CONF ((struct verification_confirmation_packet *)UIP_IP_PAYLOAD(uip_ext_len + UIP_UDPH_LEN))

// packet structures for the different types of packets used in the detection mechanism
struct generic_bh_verification_packet {
    uint8_t type; // Packet type identifier
};
typedef struct generic_bh_verification_packet generic_bh_verif_pkt_t;

// will be broadcasted to all neighbors, including the suspected blackhole node
struct verification_request_packet {
    uint8_t type; // Packet type identifier
    uip_ip6addr_t suspect_ip; // IP address of the suspected blackhole node
};
typedef struct verification_request_packet verification_req_t;

// will be sent directly to the suspected blackhole node
struct suspect_verification_request_packet {
    uint8_t type; // Packet type identifier
    uip_ip6addr_t initiator_ip; // IP address of the node initiating the verification
};
typedef struct suspect_verification_request_packet suspect_verification_req_t;

// will be sent to whatever node send the suspect verification request by the suspected node to prove that it is not a blackhole
struct suspect_clean_response_packet {
    uint8_t type; // Packet type identifier
    uip_ip6addr_t initiator_ip; // IP address of the node initiating the verification
    uip_ip6addr_t suspect_ip; // IP address of the suspected blackhole node
};
typedef struct suspect_clean_response_packet suspect_clean_resp_t;

// will be sent by the verifier to the initiator to confirm whether the suspected node is a blackhole or not
struct verification_confirmation_packet {
    uint8_t type; // Packet type identifier
    uip_ip6addr_t suspect_ip; // IP address of the suspected blackhole node
};
typedef struct verification_confirmation_packet verification_conf_t;

#endif /* PACKET_STRUCTS_H */