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

#include "data_management_layer.h"
#include "info_data_service.h"
#include "socket_manager.h"
#include "custom_data_service.h"

//Data Services Functions
extern "C" {
void data_service_init(void)
{
    DataService* socketService = SocketManager::instance();
    DataManagementLayer::registerService(socketService);
    
    InfoDataService* infoService = InfoDataService::instance();
    DataManagementLayer::registerService(infoService);

    CustomDataService* customService = CustomDataService::instance();
    DataManagementLayer::registerService(customService);
}
}