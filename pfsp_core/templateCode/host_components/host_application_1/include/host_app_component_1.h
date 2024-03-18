#pragma once

#include "TestApp1_internal_data.h"
#include "../../../pfsp_host_component_base/pfsp_host_component_base.h"

class HostAppComponent1 : public PfspHostComponentBase<HostComponent0InternalDataT>
{
public:
    HostAppComponent1(void);
    virtual ~HostAppComponent1(void){};

    virtual bool HostCompInit(std::string component_name, HostComponentStatusT &host_message) override;

    virtual bool HostCompRun(HostComponentStatusT &host_message) override;

    virtual bool HostCompStop(HostComponentStatusT &host_message) override;

    virtual bool HostCompShutdown(HostComponentStatusT &host_message) override;

    virtual bool HostCompResetData(void) override;

    virtual bool HostCompGetMessageFromPlugin(void) override;

    //virtual bool HostCompSendMessageToPlugin(const PfspHostMessageTypeE output_message_type, const unsigned int output_message_value) override;

protected:

private:
    uint32_t frame_count {0};

    HostComponent0InternalDataT* internal_data_ {};
};

#if 0

#ifdef __cplusplus
extern "C" {
#endif
    PfspHostComponentBase* create()
    {
        return new HostCompComponent1();
    }

    void destroy(PfspHostComponentBase* p)
    {
        delete p;
    }
#ifdef __cplusplus
}
#endif

#endif

/*
static std::shared_ptr<PfspHostComponentBase> create()
{
    return std::make_shared<HostAppComponent1>();
}

static void destory(std::shared_ptr<PfspHostComponentBase> p)
{
    delete p;
}
*/