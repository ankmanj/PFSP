#include "base_host_component.h"


IMPLEMENT_SERVICE_CLASS(CService3Base)

 CService3Base::CService3Base()
{
	
}

 CService3Base::~CService3Base()
{
	
}

int16_t CService3Base::MwCompInitialize(std::string component_name, uint32_t component_id, uint32_t number_of_components)
{
	int16_t result = application_service_.Initialize(component_name, component_id,number_of_components);

	return result;
}

int16_t CService3Base::MwCompStart(void)
{
	int16_t result = application_service_.Start();

	return result;
}

int16_t CService3Base::MwCompStop(void)
{
	int16_t result = application_service_.Stop();

	return result;
}

int16_t CService3Base::MwCompFinalize(void)
{
	int16_t result = application_service_.Finalize();

	return result;
}

int16_t CService3Base::MwCompMessage(AServiceMessage *poMessage)
{
	int16_t result = application_service_.Message(poMessage);

	return result;
}

int16_t CService3Base::MwCompCommand(int32_t nArgc, char_t *pszArgv[])
{
	return -1;
}

void CService3Base::MwCompNotify(void *pvArg)
{
	
}

