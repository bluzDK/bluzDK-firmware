/**
 Copyright (c) 2015 MidAir Technology, LLC.  All rights reserved.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation, either
 version 3 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __BLE_STREAM_HAL_H
#define __BLE_STREAM_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

void HAL_BLE_STREAM_Init(void);
void HAL_BLE_STREAM_Begin(void);
void HAL_BLE_STREAM_End(void);
uint32_t HAL_BLE_STREAM_Write_Data(uint8_t data);
int32_t HAL_BLE_STREAM_Available_Data(void);
int32_t HAL_BLE_STREAM_Available_Data_For_Write(void);
int32_t HAL_BLE_STREAM_Read_Data(void);
int32_t HAL_BLE_STREAM_Peek_Data(void);
void HAL_BLE_STREAM_Flush_Data(void);
bool HAL_BLE_STREAM_Is_Enabled(void);

#ifdef __cplusplus
}
#endif


#endif  /* __BLE_STREAM_HAL_H */