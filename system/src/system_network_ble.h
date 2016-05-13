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

#pragma once

#include "system_network_internal.h"


class BLENetworkInterface : public ManagedIPNetworkInterface<WLanConfig, BLENetworkInterface>
{

protected:

    virtual void on_finalize_listening(bool complete) override
    {
    }
    
    virtual void on_start_listening() override { /* n/a */ }
    virtual bool on_stop_listening() override { /* n/a */ return false; }
    virtual void on_setup_cleanup() override { /* n/a */ }

    virtual void connect_init() override { /* n/a */ }

    virtual void connect_finalize() override {
        
    }

    void on_now() override {
        
    }

    void off_now() override {
        
    }

    void disconnect_now() override {
        
    }

public:

    void fetch_ipconfig(WLanConfig* target) {

    }

    void start_listening() override
    {
        
    }

    bool listening() override
    {
        return ManagedNetworkInterface::listening();
    }

    void setup() override
    {
        //cellular_init(NULL);
    }

    // todo - associate credentials with presense of SIM card??
    bool clear_credentials() override { /* n/a */ return true; }
    bool has_credentials() override
    {
        return true;
    }
    int set_credentials(NetworkCredentials* creds) override { return -1; }
    void connect_cancel(bool cancel) override { /* n/a */ }

    void set_error_count(unsigned count) override
    {
    }
};

