/*
 * Copyright (c) 2010 Nicira Networks.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OX_MATCH_H
#define OX_MATCH_H 1

#include <stdint.h>
#include <map>
#include <string>
#include <sys/types.h>
#include <netinet/in.h>
#include "../libc/ofpbuf.h"
#include "../libc/hmap.h"
#include "../libc/packets.h"
#include "ofl-structs.h"


#define OXM_HEADER__(VENDOR, FIELD, HASMASK, LENGTH) \
    (((VENDOR) << 16) | ((FIELD) << 9) | ((HASMASK) << 8) | (LENGTH))
#define OXM_HEADER(VENDOR, FIELD, LENGTH) \
    OXM_HEADER__(VENDOR, FIELD, 0, LENGTH)
#define OXM_HEADER_W(VENDOR, FIELD, LENGTH) \
    OXM_HEADER__(VENDOR, FIELD, 1, (LENGTH) * 2)

#define OXM_HEADER_VL(VENDOR,FIELD) \
    OXM_HEADER__(VENDOR,FIELD,0,0)

#define OXM_HEADER_VL_W(VENDOR,FIELD) \
   OXM_HEADER__(VENDOR,FIELD,1,0)
   
#define OXM_VENDOR(HEADER) ((HEADER) >> 16)
#define OXM_FIELD(HEADER) (((HEADER) >> 9) & 0x7f)
#define OXM_TYPE(HEADER) (((HEADER) >> 9) & 0x7fffff)
#define OXM_HASMASK(HEADER) (((HEADER) >> 8) & 1)
#define OXM_LENGTH(HEADER) ((HEADER) & 0xff)
#define VENDOR_FROM_TYPE(TYPE) ((TYPE) >> 7)
#define FIELD_FROM_TYPE(TYPE)  ((TYPE) & 0x7f)

#define OXM_MAKE_WILD_HEADER(HEADER) \
        OXM_HEADER_W(OXM_VENDOR(HEADER), OXM_FIELD(HEADER), OXM_LENGTH(HEADER))

/* ## ------------------------------- ## */
/* ## OpenFlow 1.2-compatible fields. ## */
/* ## ------------------------------- ## */

/* Physical or virtual port on which the packet was received.
 *
 * Prereqs: None.
 *
 * Format: 16-bit integer. */
#define    OXM_OF_IN_PORT      OXM_HEADER    (0x8000, 0, 4)


/* Physical port on which the packet was received.
 *
 * Prereqs: None.
 *
 * Format: 32-bit integer. */
#define    OXM_OF_IN_PHY_PORT      OXM_HEADER    (0x8000, 1, 4)

/* Metadata passed btw tables. */
#define OXM_OF_METADATA OXM_HEADER     (0x8000, 2, 8)
#define OXM_OF_METADATA_W OXM_HEADER_W (0x8000, 2, 8)

/* Ethernet destination address.*/
#define    OXM_OF_ETH_DST   OXM_HEADER  (0x8000,3,6) 
#define    OXM_OF_ETH_DST_W OXM_HEADER_W(0x8000,3,6) 

/* Ethernet source address.*/
#define    OXM_OF_ETH_SRC   OXM_HEADER  (0x8000, 4,6)
#define    OXM_OF_ETH_SRC_W OXM_HEADER_W(0x8000,4,6) 

/* Ethernet frame type. */
#define    OXM_OF_ETH_TYPE     OXM_HEADER    (0x8000, 5, 2)

/* VLAN id. */
#define    OXM_OF_VLAN_VID OXM_HEADER  (0x8000, 6, 2)
#define    OXM_OF_VLAN_VID_W OXM_HEADER_W  (0x8000, 6, 2)

 /* VLAN priority. */
#define    OXM_OF_VLAN_PCP   OXM_HEADER  (0x8000, 7, 1)


/* IP ToS (DSCP field, 6 bits). */
#define    OXM_OF_IP_DSCP      OXM_HEADER    (0x8000, 8, 1)

/* IP ECN */
#define    OXM_OF_IP_ECN      OXM_HEADER    (0x8000, 9, 1)

/* IP protocol. */
#define    OXM_OF_IP_PROTO   OXM_HEADER  (0x8000, 10, 1) 

 /* IP source address. */
#define    OXM_OF_IPV4_SRC      OXM_HEADER  (0x8000,11, 4)
#define    OXM_OF_IPV4_SRC_W  OXM_HEADER_W  (0x8000,11, 4) 

/* IP destination address. */
#define    OXM_OF_IPV4_DST     OXM_HEADER  (0x8000,12 , 4) 
#define    OXM_OF_IPV4_DST_W     OXM_HEADER_W  (0x8000,12 , 4) 

/* TCP source port. */
#define    OXM_OF_TCP_SRC      OXM_HEADER  (0x8000, 13, 2)

 /* TCP destination port. */ 
#define    OXM_OF_TCP_DST    OXM_HEADER    (0x8000, 14, 2)

/* UDP source port. */
#define    OXM_OF_UDP_SRC      OXM_HEADER  (0x8000, 15, 2)

 /* UDP destination port. */ 
#define    OXM_OF_UDP_DST    OXM_HEADER    (0x8000, 16, 2)

/* SCTP source port. */
#define    OXM_OF_SCTP_SRC      OXM_HEADER  (0x8000, 17, 2)

 /* SCTP destination port. */ 
#define    OXM_OF_SCTP_DST    OXM_HEADER    (0x8000, 18, 2)

/* ICMPv4 type. */
#define    OXM_OF_ICMPV4_TYPE   OXM_HEADER  (0x8000, 19, 1) 

/* ICMPv4 code. */
#define    OXM_OF_ICMPV4_CODE   OXM_HEADER  (0x8000, 20, 1)

 /* ARP operation code. */
#define    OXM_OF_ARP_OP      OXM_HEADER  (0x8000,21, 2)

 /* IPv4 source address of ARP. */
#define    OXM_OF_ARP_SPA      OXM_HEADER  (0x8000,22, 4)
#define    OXM_OF_ARP_SPA_W  OXM_HEADER_W  (0x8000,22, 4) 

/* IPv4 destination address of ARP. */
#define    OXM_OF_ARP_TPA     OXM_HEADER    (0x8000,23 , 4) 
#define    OXM_OF_ARP_TPA_W   OXM_HEADER_W  (0x8000,23 , 4) 

/* ARP Ethernet destination address.*/
#define    OXM_OF_ARP_SHA   OXM_HEADER  (0x8000,24,6) 
#define    OXM_OF_ARP_SHA_W OXM_HEADER_W(0x8000,24,6) 

/* ARP Ethernet source address.*/
#define    OXM_OF_ARP_THA   OXM_HEADER  (0x8000, 25,6)
#define    OXM_OF_ARP_THA_W OXM_HEADER_W(0x8000,25,6) 

/* IPv6 source address */
#define OXM_OF_IPV6_SRC OXM_HEADER (0x8000, 26, 16)
#define OXM_OF_IPV6_SRC_W OXM_HEADER_W(0x8000, 26, 16)

/* IPv6 destination address*/
#define OXM_OF_IPV6_DST   OXM_HEADER (0x8000, 27, 16) 
#define OXM_OF_IPV6_DST_W OXM_HEADER_W(0x8000, 27, 16)

/* IPv6 flow label*/
#define OXM_OF_IPV6_FLABEL   OXM_HEADER (0x8000, 28, 4)
#define OXM_OF_IPV6_FLABEL_W OXM_HEADER_W (0x8000, 28, 4)

/* ICMPv6 message type field */
#define OXM_OF_ICMPV6_TYPE OXM_HEADER (0x8000, 29, 1) 

/* ICMPv6 type code */
#define OXM_OF_ICMPV6_CODE OXM_HEADER (0x8000, 30, 1) 

/* IPv6 nd target*/
#define OXM_OF_IPV6_ND_TARGET   OXM_HEADER (0x8000, 31, 16) 

/* IPv6 nd target*/
#define OXM_OF_IPV6_ND_SLL  OXM_HEADER (0x8000, 32, 6)

/* IPv6 dnd target*/
#define OXM_OF_IPV6_ND_TLL   OXM_HEADER (0x8000, 33, 6)  

/* MPLS label. */
#define OXM_OF_MPLS_LABEL OXM_HEADER (0x8000, 34, 4)

/* MPLS TC. */
#define OXM_OF_MPLS_TC OXM_HEADER     (0x8000, 35, 1)

/* ## ------------------------------- ## */
/* ## IPv6 compatible fields. ## */
/* ## ------------------------------- ## */


/* Traffic Class */
#define OXM_OF_IPV6_TC OXM_HEADER (0x0002, 5, 1)
#define OXM_OF_IPV6_TC_W OXM_HEADER_W (0x0002, 5, 1)

/* IPv6 Hop-by-Hop EH ID*/
#define OXM_OF_IPV6_HBH_ID OXM_HEADER (0x0002, 8, 1)
#define OXM_OF_IPV6_HBH_ID_W OXM_HEADER_W (0x0002, 8, 1)  

#define OXM_OF_IPV6_HBH_OPT_CODE OXM_HEADER (0x0002, 9, 1) 

#define OXM_OF_IPV6_HBH_OPT_VALUE OXM_HEADER_VL (0x0002, 10) 

/* IPv6 Destination Option EH ID*/
#define OXM_OF_IPV6_DOH_ID OXM_HEADER (0x0002, 16, 1)
#define OXM_OF_IPV6_DOH_ID_W OXM_HEADER_W (0x0002, 16, 1)

#define OXM_OF_IPV6_DOH_OPT_CODE OXM_HEADER (0x0002, 17, 1)

#define OXM_OF_IPV6_DOH_OPT_VALUE OXM_HEADER_VL (0x0002, 18)


/* IPv6 Routing EH ID*/ 
#define OXM_OF_IPV6_RH_ID OXM_HEADER (0x0002, 24, 1)
#define OXM_OF_IPV6_RH_ID_W OXM_HEADER_W (0x0002, 24, 1)

#define OXM_OF_IPV6_RH_ADDRESS OXM_HEADER (0x0002, 25, 16)

/* IPv6 Fragmentation EH ID*/
#define OXM_OF_IPV6_FH_ID OXM_HEADER (0x0002, 32, 1)
#define OXM_OF_IPV6_FH_ID_W OXM_HEADER_W (0x0002, 32, 1)

/* IPv6 Authentication EH ID*/ 
#define OXM_OF_IPV6_AH_ID OXM_HEADER (0x0002, 40, 1)
#define OXM_OF_IPV6_AH_ID_W OXM_HEADER_W (0x0002, 40, 1)

/* IPv6 Encapsulating Security Payload */ 
#define OXM_OF_IPV6_ESP_ID OXM_HEADER (0x0002, 48, 1) 

/* IPv6 Mobility EH */
#define OXM_OF_IPV6_MH_ID OXM_HEADER (0x0002, 56, 1) 

/* ## ------------------------------- ## */
/* ## TTL fields. ## */
/* ## ------------------------------- ## */

/* MPLS TTL */
#define OXM_OF_MPLS_TTL OXM_HEADER (0x0002, 80, 4)

/* IPv4 TTL */
#define OXM_OF_IPV4_TTL OXM_HEADER (0x0002, 81, 1)

/*Defined in oxm-match.c*/ 
extern std::map<std::string,std::pair<int,int> > fields;

int 
oxm_pull_match(struct ofpbuf * buf, struct ofl_match *match_dst, int match_len);

int oxm_put_match(struct ofpbuf *buf, struct ofl_match *omt);

char *oxm_match_to_string(const uint8_t *, unsigned int match_len);

int oxm_match_from_string(const char *, struct ofpbuf *);

uint32_t oxm_entry_ok(const void *, unsigned int );

int
oxm_field_bytes(uint32_t header);

int
oxm_field_bits(uint32_t header);
                    


#endif /* nx-match.h */
