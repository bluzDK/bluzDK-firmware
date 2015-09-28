#include "particle_data_service.h"
#include "nrf51_config.h"

void particle_service_send_data(uint8_t* data, uint16_t len)
{
    scs_data_send(&m_scs, data, len);
}
