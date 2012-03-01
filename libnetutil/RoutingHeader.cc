
/***************************************************************************
 * RoutingHeader.cc -- The RoutingHeader Class represents an IPv6 Routing  *
 * extension header.                                                       *
 ***********************IMPORTANT NMAP LICENSE TERMS************************
 *                                                                         *
 * The Nmap Security Scanner is (C) 1996-2012 Insecure.Com LLC. Nmap is    *
 * also a registered trademark of Insecure.Com LLC.  This program is free  *
 * software; you may redistribute and/or modify it under the terms of the  *
 * GNU General Public License as published by the Free Software            *
 * Foundation; Version 2 with the clarifications and exceptions described  *
 * below.  This guarantees your right to use, modify, and redistribute     *
 * this software under certain conditions.  If you wish to embed Nmap      *
 * technology into proprietary software, we sell alternative licenses      *
 * (contact sales@insecure.com).  Dozens of software vendors already       *
 * license Nmap technology such as host discovery, port scanning, OS       *
 * detection, version detection, and the Nmap Scripting Engine.            *
 *                                                                         *
 * Note that the GPL places important restrictions on "derived works", yet *
 * it does not provide a detailed definition of that term.  To avoid       *
 * misunderstandings, we interpret that term as broadly as copyright law   *
 * allows.  For example, we consider an application to constitute a        *
 * "derivative work" for the purpose of this license if it does any of the *
 * following:                                                              *
 * o Integrates source code from Nmap                                      *
 * o Reads or includes Nmap copyrighted data files, such as                *
 *   nmap-os-db or nmap-service-probes.                                    *
 * o Executes Nmap and parses the results (as opposed to typical shell or  *
 *   execution-menu apps, which simply display raw Nmap output and so are  *
 *   not derivative works.)                                                *
 * o Integrates/includes/aggregates Nmap into a proprietary executable     *
 *   installer, such as those produced by InstallShield.                   *
 * o Links to a library or executes a program that does any of the above   *
 *                                                                         *
 * The term "Nmap" should be taken to also include any portions or derived *
 * works of Nmap.  This list is not exclusive, but is meant to clarify our *
 * interpretation of derived works with some common examples.  Our         *
 * interpretation applies only to Nmap--we don't speak for other people's  *
 * GPL works.                                                              *
 *                                                                         *
 * If you have any questions about the GPL licensing restrictions on using *
 * Nmap in non-GPL works, we would be happy to help.  As mentioned above,  *
 * we also offer alternative license to integrate Nmap into proprietary    *
 * applications and appliances.  These contracts have been sold to dozens  *
 * of software vendors, and generally include a perpetual license as well  *
 * as providing for priority support and updates.  They also fund the      *
 * continued development of Nmap.  Please email sales@insecure.com for     *
 * further information.                                                    *
 *                                                                         *
 * As a special exception to the GPL terms, Insecure.Com LLC grants        *
 * permission to link the code of this program with any version of the     *
 * OpenSSL library which is distributed under a license identical to that  *
 * listed in the included docs/licenses/OpenSSL.txt file, and distribute   *
 * linked combinations including the two. You must obey the GNU GPL in all *
 * respects for all of the code used other than OpenSSL.  If you modify    *
 * this file, you may extend this exception to your version of the file,   *
 * but you are not obligated to do so.                                     *
 *                                                                         *
 * If you received these files with a written license agreement or         *
 * contract stating terms other than the terms above, then that            *
 * alternative license agreement takes precedence over these comments.     *
 *                                                                         *
 * Source is provided to this software because we believe users have a     *
 * right to know exactly what a program is going to do before they run it. *
 * This also allows you to audit the software for security holes (none     *
 * have been found so far).                                                *
 *                                                                         *
 * Source code also allows you to port Nmap to new platforms, fix bugs,    *
 * and add new features.  You are highly encouraged to send your changes   *
 * to nmap-dev@insecure.org for possible incorporation into the main       *
 * distribution.  By sending these changes to Fyodor or one of the         *
 * Insecure.Org development mailing lists, it is assumed that you are      *
 * offering the Nmap Project (Insecure.Com LLC) the unlimited,             *
 * non-exclusive right to reuse, modify, and relicense the code.  Nmap     *
 * will always be available Open Source, but this is important because the *
 * inability to relicense code has caused devastating problems for other   *
 * Free Software projects (such as KDE and NASM).  We also occasionally    *
 * relicense the code to third parties as discussed above.  If you wish to *
 * specify special license conditions of your contributions, just say so   *
 * when you send them.                                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       *
 * General Public License v2.0 for more details at                         *
 * http://www.gnu.org/licenses/gpl-2.0.html , or in the COPYING file       *
 * included with Nmap.                                                     *
 *                                                                         *
 ***************************************************************************/
/* This code was originally part of the Nping tool.                        */

#include "RoutingHeader.h"
#include <assert.h>

/******************************************************************************/
/* CONTRUCTORS, DESTRUCTORS AND INITIALIZATION METHODS                        */
/******************************************************************************/
RoutingHeader::RoutingHeader() {
  this->reset();
} /* End of RoutingHeader constructor */


RoutingHeader::~RoutingHeader() {

} /* End of RoutingHeader destructor */


/** Sets every attribute to its default value */
void RoutingHeader::reset(){
  memset(&this->h, 0, sizeof(nping_ipv6_ext_routing_hdr_t));
  this->length=ROUTING_HEADER_MIN_LEN;
  this->curr_addr=(u8 *)this->h.data;
} /* End of reset() */


/******************************************************************************/
/* PacketElement:: OVERWRITTEN METHODS                                        */
/******************************************************************************/

/** @warning This method is essential for the superclass getBinaryBuffer()
 *  method to work. Do NOT change a thing unless you know what you're doing  */
u8 *RoutingHeader::getBufferPointer(){
  return (u8*)(&this->h);
} /* End of getBufferPointer() */


/** Stores supplied packet in the internal buffer so the information
  * can be accessed using the standard get & set methods.
  * @warning  The RoutingHeader class is able to hold a maximum of
  * sizeof(nping_icmpv6_hdr_t) bytes. If the supplied buffer is longer than
  * that, only the first 1508 bytes will be stored in the internal buffer.
  * @warning Supplied len MUST be at least 8 bytes (min ICMPv6 header length).
  * @return OP_SUCCESS on success and OP_FAILURE in case of error */
int RoutingHeader::storeRecvData(const u8 *buf, size_t len){
  if(buf==NULL || len<ROUTING_HEADER_MIN_LEN){
    this->length=0;
    return OP_FAILURE;
  }else{
     /* Store the first 4 bytes, so we can access lenght and routing type */
     memcpy(&(this->h), buf, 4);

     /* Our behaviour is different depending on the routing type. */
     switch(this->h.type){

         /* Routing Type 0 (deprecated by RFC 5095)*/
         case 0:
             /* Type 0 has a variable length, but the value of its HdrExtLen
              * field must be even (because it must be a multiple of the
              * IPv6 address size). We also make sure that the received buffer
              * has as many bytes as the HdrExtLen field says it has, and 
              * that it doesn't exceed the maximum number of octets we
              * can store in this object. */
             if(this->h.len%2==1 || ((unsigned int)(this->h.len+1))*8 > len || (this->h.len+1)*8 > ROUTING_HEADER_MAX_LEN){
               this->length=0;
               return OP_FAILURE;
             }else{
                int pkt_len=(this->h.len+1)*8;
                this->reset();
                this->length=pkt_len;
                memcpy(&(this->h), buf, this->length);
                return OP_SUCCESS;
             }
         break;

         /* Routing Type 2 (For IPv6 Mobility. See RFC 6275) */
         case 2:
             /* Type 2 has a fixed length. If we have that many octets, store
              * them. We'll perform validation later in validate(). */
             if(len<ROUTING_TYPE_2_HEADER_LEN){
               this->length=0;
               return OP_FAILURE;
             }else{
                this->reset();
                memcpy(&(this->h), buf, ROUTING_TYPE_2_HEADER_LEN);
                this->length=ROUTING_TYPE_2_HEADER_LEN;
                return OP_SUCCESS;
             }
         break;

         /* Unknown routing type */
         default:
             /* If this is some routing type that we don't know about, we'll have
              * to store as much data as the header says it has. Obvioulsy, we
              * check that we received as much data as the HdrExtLen advertises,
              * and that we don't exceed our own internal limit. */
             if( ((unsigned int)(this->h.len+1))*8 > len || (this->h.len+1)*8 > ROUTING_HEADER_MAX_LEN){
               this->length=0;
               return OP_FAILURE;
             }else{
                this->reset();
                this->length=(this->h.len+1)*8;
                memcpy(&(this->h), buf, this->length);
                return OP_SUCCESS;
             }
         break;
     }
  }
 return OP_FAILURE;
} /* End of storeRecvData() */


/* Returns a protocol identifier. This is used by packet parsing funtions
 * that return linked lists of PacketElement objects, to determine the protocol
 * the object represents. */
int RoutingHeader::protocol_id() const {
    return HEADER_TYPE_IPv6_ROUTE;
} /* End of protocol_id() */


/** Determines if the data stored in the object after an storeRecvData() call
  * is valid and safe to use. This mainly checks the length of the data but may
  * also test the value of certain protocol fields to ensure their correctness.
  * @return the length, in bytes, of the header, if its found to be valid or
  * OP_FAILURE (-1) otherwise. */
int RoutingHeader::validate(){

  /* Check the object's length makes sense*/
  if(this->length < ROUTING_HEADER_MIN_LEN || this->length%8!=0) {
      return OP_FAILURE;
  }

  switch(this->h.type){
     /* Routing Type 0 (deprecated by RFC 5095)*/
     case 0:
         /* Here we check that:
          * 1) The length in HdrExtLen is even.
          * 2) The length in HdrExtLen matches the octects stored in this object.
          * 3) The length in HdrExtLen does not exceed our internal limit. */
         if(this->h.len%2==1 || (this->h.len+1)*8 != this->length || (this->h.len+1)*8 > ROUTING_HEADER_MAX_LEN){
           return OP_FAILURE;
         }

         /* Also, for Type 0, the value in the SegmentsLeft field should be less
          * than or equal to the number of addresses in the packet. We verify
          * that using the value of the HDrExtLen field which, divided by two,
          * yields the number of addresses in the packet. It certainly doesn't
          * make sense for the packet to say there are 5 hops left when we
          * have less than 5 IPv6 addresses. We allow it to be less than
          * the number of addresses present in the packet because the RFC 2460
          * only talkes about segleft being greater than HDrExtLen/2, not less. */
         if(this->h.segleft > this->h.len/2){
           return OP_FAILURE;
         }
     break;

     /* Routing Type 2 (For IPv6 Mobility. See RFC 6275) */
     case 2:
         /* Check that we have the exact number of octets we expect. */
         if(this->length!= ROUTING_TYPE_2_HEADER_LEN){
           return OP_FAILURE;
         }
         /* Also check that the HdrExtLen and SegmentsLeft fields have the
          * value that RFC 6275 dictates. */
         if(this->h.segleft!=1 || this->h.len!=2){
           return OP_FAILURE;
         }
     break;

     /* Unknown routing type */
     default:
         /* If this is some routing type that we don't know about, we just
          * check that the length makes sense because we cannot make assumptions
          * about the semantics of other fields. */
         if( this->length!=(this->h.len+1)*8 || (this->h.len+1)*8>ROUTING_HEADER_MAX_LEN){
           return OP_FAILURE;
         }
     break;

  }
  return this->length;
} /* End of validate() */


/** Prints the contents of the header and calls print() on the next protocol
  * header in the chain (if there is any).
  * @return OP_SUCCESS on success and OP_FAILURE in case of error. */
int RoutingHeader::print(FILE *output, int detail) const {
  fprintf(output, "Routing[nh=%d len=%d type=%d segleft=%d]", this->h.nh, this->h.len, this->h.type, this->h.segleft);
  // TODO: @todo : Implement this
  if(this->next!=NULL){
    print_separator(output, detail);
    next->print(output, detail);
  }
  return OP_SUCCESS;
} /* End of print() */


/******************************************************************************/
/* PROTOCOL-SPECIFIC METHODS                                                  */
/******************************************************************************/

/** Set Next Header field */
int RoutingHeader::setNextHeader(u8 val){
  this->h.nh = val;
  return OP_SUCCESS;
} /* End of setNextHeader() */


/** Returns next header id */
u8 RoutingHeader::getNextHeader(){
  return this->h.nh;
} /* End of getNextHeader() */


/** Set routing type */
int RoutingHeader::setRoutingType(u8 val){
  this->h.type = val;
  return OP_SUCCESS;
} /* End of setRoutingType() */


/** Returns the routing type */
u8 RoutingHeader::getRoutingType(){
  return this->h.type;
} /* End of getRoutingType() */


/** Set number of segments left */
int RoutingHeader::setSegmentsLeft(u8 val){
  this->h.segleft = val;
  return OP_SUCCESS;
} /* End of setSegmentsLeft() */


/** Returns the number of segments left */
u8 RoutingHeader::getSegmentsLeft(){
  return this->h.segleft;
} /* End of getSegmentsLeft() */


/** Set number of segments left */
int RoutingHeader::addAddress(struct in6_addr val){
  /* Check we don't exceed max length */
  if((this->length + 16)>ROUTING_HEADER_MAX_LEN)
    return OP_FAILURE;
  memcpy(this->curr_addr, val.s6_addr, 16);
  this->curr_addr+=16;
  this->h.len+=2;
  this->length+=16;
  return OP_SUCCESS;
} /* End of setSegmentsLeft() */
