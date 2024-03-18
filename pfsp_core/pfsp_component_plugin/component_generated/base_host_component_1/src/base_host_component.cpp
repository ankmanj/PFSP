#include "base_host_component.h"


IMPLEMENT_SERVICE_CLASS(CService2Base)

 CService2Base::CService2Base()
{
	
}

 CService2Base::~CService2Base()
{
	
}

int16_t CService2Base::MwCompInitialize(std::string component_name, uint32_t component_id, uint32_t number_of_components)
{
	int16_t result = application_service_.Initialize(component_name, component_id,number_of_components);

	return result;
}

int16_t CService2Base::MwCompStart(void)
{
	int16_t result = application_service_.Start();

	return result;
}

int16_t CService2Base::MwCompStop(void)
{
	int16_t result = application_service_.Stop();

	return result;
}

int16_t CService2Base::MwCompFinalize(void)
{
	int16_t result = application_service_.Finalize();

	return result;
}

int16_t CService2Base::MwCompMessage(AServiceMessage *poMessage)
{
	int16_t result = application_service_.Message(poMessage);

	return result;
}

int16_t CService2Base::MwCompCommand(int32_t nArgc, char_t *pszArgv[])
{
	return -1;
}

void CService2Base::MwCompNotify(void *pvArg)
{
	
}

