#pragma once

#include <cassert>
#include <dlfcn.h>

class PfspMwStart
{
public:
    PfspMwStart(void);
    virtual ~PfspMwStart(void);

    int PfspMwStartMain(int argc, char* argv[]);
protected:

private:
    void* loading_lib(const char* name)
    {
        void* handle = dlopen (name, RTLD_NOW | RTLD_GLOBAL);
        if( handle == 0 )
        {
            printf("error openning %s\n", name);
            return 0;
        }
        return handle;
    }

    void LoadLibsOfAllComponents()
    {
        void* libhandle = 0;
        libhandle = loading_lib("libPFSP_MW.so");
        assert(libhandle);

        libhandle = loading_lib("libPFSP_plugin_error_handler.so");
        assert(libhandle);

        libhandle = loading_lib("libPFSP_plugin_mode_manager.so");
        assert(libhandle);

        libhandle = loading_lib("libPFSP_base_host_component_0.so");
        assert(libhandle);

        libhandle = loading_lib("libPFSP_base_host_component_1.so");
        assert(libhandle);
    }
};
