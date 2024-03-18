#include "base_host_component.h"

IMPLEMENT_SERVICE_CLASS(CHostApplication2Service)

int16_t CHostApplication2Service::MwCompInitialize(std::string component_name, uint32_t component_id, uint32_t number_of_components)
{
    int16_t result = application_service_.Initialize(component_name, component_id,number_of_components);
 
    return result;
}

int16_t CHostApplication2Service::MwCompStart(void)
{
    int16_t result = application_service_.Start();
 
    return result;
}

int16_t CHostApplication2Service::MwCompStop(void)
{
    int16_t result = application_service_.Stop();
 
    return result;
}

void CHostApplication2Service::MwCompNotify(void *pvArg)
{
}

int16_t CHostApplication2Service::MwCompMessage( AServiceMessage *poMessage )
{
    int16_t result = application_service_.Message(poMessage);
 
    return result;
}

int16_t CHostApplication2Service::MwCompCommand( int32_t nArgc, char_t *pszArgv[] )
{
    return -1;
}

int16_t CHostApplication2Service::MwCompFinalize(void)
{
    int16_t result = application_service_.Finalize();
 
    return result;
}