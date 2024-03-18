#include "base_host_component.h"

IMPLEMENT_SERVICE_CLASS(CHostApplication1Service)

int16_t CHostApplication1Service::MwCompInitialize(std::string component_name, uint32_t component_id, uint32_t number_of_components)
{
    int16_t result = application_service_.Initialize(component_name, component_id,number_of_components);

    return result;
}

int16_t CHostApplication1Service::MwCompStart(void)
{
    int16_t result = application_service_.Start();
 
    return result;
}

int16_t CHostApplication1Service::MwCompStop(void)
{
    int16_t result = application_service_.Stop();
 
    return result;
}

int16_t CHostApplication1Service::MwCompFinalize(void)
{
    int16_t result = application_service_.Finalize();
 
    return result;
}

int16_t CHostApplication1Service::MwCompMessage( AServiceMessage *poMessage )
{
    int16_t result = application_service_.Message(poMessage);
 
    return result;
}

int16_t CHostApplication1Service::MwCompCommand( int32_t nArgc, char_t *pszArgv[] )
{
    return -1;
}

void CHostApplication1Service::MwCompNotify(void *pvArg)
{
}
