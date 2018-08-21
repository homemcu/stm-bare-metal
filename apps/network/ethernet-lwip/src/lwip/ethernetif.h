 /*
* Copyright (c) 2018 Vladimir Alemasov
* All rights reserved
*
* This program and the accompanying materials are distributed under 
* the terms of GNU General Public License version 2 
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#ifndef ETHERNETIF_H_
#define ETHERNETIF_H_

err_t ethernetif_init(struct netif *netif);
void ethernetif_input(void *pvParameters);

#endif /* ETHERNETIF_H_ */
