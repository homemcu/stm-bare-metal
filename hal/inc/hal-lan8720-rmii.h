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
#ifndef HAL_LAN8720_RMII_H_
#define HAL_LAN8720_RMII_H_

mac_error hal_lan8720_init(const uint8_t *mac);
mac_error hal_lan8720_get_transmission_status(void);
mac_error hal_lan8720_write_frame(const uint8_t *frame, uint16_t size);
void hal_lan8720_start_transmission(void);
mac_error hal_lan8720_get_reception_status(uint16_t *size);
uint16_t hal_lan8720_read_frame(uint8_t *frame, uint16_t size);
void hal_lan8720_release_reception_buffer(void);

#endif // HAL_LAN8720_RMII_H_