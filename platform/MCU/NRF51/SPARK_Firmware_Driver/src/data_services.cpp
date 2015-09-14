#include "data_management_layer.h"
#include "socket_manager.h"

//Data Services Functions
extern "C" {
void data_service_init(void)
{
    DataService* socketService = SocketManager::instance();
    DataManagementLayer::registerService(socketService);
}
}