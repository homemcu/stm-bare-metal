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
#ifndef MAC_H_
#define MAC_H_

typedef enum
{
	mac_err_ok = 0,
	mac_err_phyaddr_not_found,
	mac_err_not_linked,
	mac_err_busy,
	mac_err_tx_need_start,
	mac_err_rx_invalid
} mac_error;

#endif // MAC_H_