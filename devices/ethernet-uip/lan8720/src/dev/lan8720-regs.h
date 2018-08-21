#ifndef LAN8720_REGS_H_
#define LAN8720_REGS_H_

#define LAN8720_BCR             0x00     // Basic Control Register
#define LAN8720_BSR             0x01     // Basic Status Register
#define LAN8720_PHYI1           0x02     // PHY Identifier 1
#define LAN8720_PHYI2           0x03     // PHY Identifier 2
#define LAN8720_ANLPAR          0x05     // Auto-Negotiation Link Partner Ability Register

#define LAN8720_BCR_RAN         0x0200   // Restart Auto-Negotiate (Bit is self-clearing)
#define LAN8720_BCR_SR          0x8000   // Soft Reset

#define LAN8720_BSR_ANC         0x0020   // Auto-Negotiate Complete
#define LAN8720_BSR_LS          0x0004   // Link Status

#define LAN8720_ANLPAR_10       0x0020   // 10Mbps able
#define LAN8720_ANLPAR_10FULL   0x0040   // 10Mbps with full duplex
#define LAN8720_ANLPAR_100      0x0080   // 100Mbps able
#define LAN8720_ANLPAR_100FULL  0x0100   // 100Mbps with full duplex

#endif // LAN8720_REGS_H_
