#include <stdio.h>
#include <signal.h>
#include <execinfo.h>
#include <sys/wait.h>
#include <pthread.h>

#include "osal/Debug.h"
#include "osal/File.h"
#include "osal/Task.h"
#include "main/rpmain.h"

#include "pfsp_mw_start.h"

#define NUM_OF_PRE_ARGC 	(4)
#define MAX_LOGGING_TEXT_LENGTH (128)
#define MAX_MAIN_ARGC_VALUE		(50)
#define MAX_SERVICE_NAME 		(50)

#define DEFAULT_CONFIG_FILE_NAME	"../../pfsp_config/xml/run.sh"

#define PROCESS_NAME "PFSP_p"
#define THREAD_NAME  "PFSP"

extern void SetServiceIncludePath(const char* pszIncludePath);

class CMainActivityNotify : public IMainActivityNotify
{
	void PostInitialize(void);
	void PostFinalize(void);
};

int InitPortMap(char* argv[])
{
  char **s_szInitArgv = 0;

  s_szInitArgv = new char*[NUM_OF_PRE_ARGC+1];
  for( int i = 0 ; i < NUM_OF_PRE_ARGC ; i++ )
  {
    s_szInitArgv[i] = new char[strlen(argv[i])+1];
    strcpy(s_szInitArgv[i], argv[i]);
  }
  
  s_szInitArgv[NUM_OF_PRE_ARGC] = new char[strlen("-initial")+1];
  strcpy(s_szInitArgv[NUM_OF_PRE_ARGC],"-initial");

  richosmain(NUM_OF_PRE_ARGC + 1, s_szInitArgv);

  for( int i = 0 ; i <= NUM_OF_PRE_ARGC ; i++ )
  {
    delete [] s_szInitArgv[i];
  }

  delete [] s_szInitArgv;

  return 0;
}

int Parse_RunSh(int *s_nArgc, char* argv[], char *s_szArgv[])
{
  char *szTemp = new char[MAX_SERVICE_NAME];
  int nCnt = 0, nRet;
  const char* pcConfigFileName = argv[1];

  if( *s_nArgc >= 3 )
  {
    if( strstr(argv[0], "PFSP") )
    {
      strcpy(s_szArgv[0], "PFSP");
      //DEBG_I_Print("Parse_RunSh() -> s_szArgv[0]: %s \n", s_szArgv[0]);
    }
    
    for( int i = 1 ; i < *s_nArgc ; i++ )
    {
      strcpy(s_szArgv[i], argv[i]);
      //DEBG_I_Print("Parse_RunSh() -> s_szArgv[%d]: %s \n", i, s_szArgv[i]);
    }

    nRet = 0;
  }
  else
  {
    if( pcConfigFileName == 0 )
    {
      pcConfigFileName = DEFAULT_CONFIG_FILE_NAME;
    }

    FILE_T pRunFile = FILE_Open(pcConfigFileName, FILE_OPTION_ACCESS_READ);
    if( pRunFile != FILE_FAILURE )
    {
      int nFileLength = FILE_Seek(pRunFile, 0, FILE_OPTION_SEEK_END);
      FILE_Seek(pRunFile, 0, FILE_OPTION_SEEK_BEGIN);

      char* pszRunSh = new char[nFileLength+1];
      DEBG_Assert(pszRunSh);

      if( pRunFile != 0 )
      {
        int ret = FILE_Read(pRunFile, pszRunSh, nFileLength);
        if( ret >= 0 )
        {
          pszRunSh[ret] = 0;
        }

        FILE_Close(pRunFile);
      }

      szTemp = strtok(pszRunSh, " ");

      for( ; szTemp != 0; )
      {
        strcpy(s_szArgv[nCnt], szTemp);
        ++nCnt;
        szTemp = strtok(0, " ");
      }

      DEBG_Assert(nCnt < MAX_MAIN_ARGC_VALUE);

      *s_nArgc = nCnt;

      delete [] pszRunSh;
      delete [] szTemp;

      nRet = 0;
    }
    else
    {
      DEBG_E_Print("File open error (%s:%s): %s\n", __FILE__, __FUNCTION__ , pcConfigFileName);
      nRet = -1;
    }
  }
  
  //DEBG_I_Print("\n\n main argument by %s.\n\n", pcConfigFileName);
  
  return nRet;
}

static int argc_forCallback = 0;
static char *argv_forCallback[MAX_MAIN_ARGC_VALUE];

void MainTask(int argc, char* argv[])
{
  CMainActivityNotify oMailActivityNotify;
  char **s_szArgv = 0;
  int ret;

  s_szArgv = new char*[MAX_MAIN_ARGC_VALUE];

  for( int i = 0 ; i < MAX_MAIN_ARGC_VALUE ; i++ )
  {
    s_szArgv[i] = new char[MAX_SERVICE_NAME];
  }

  ret = Parse_RunSh(&argc, argv, s_szArgv);
  if( !ret )
  {
    uint32_t count = 0;

    argc_forCallback = argc;
    for( count = 0 ; count < argc ; count++ ) // ( count = 0 ; count < MAX_MAIN_ARGC_VALUE ; count++ )
    {
      argv_forCallback[count] = new char[strlen(s_szArgv[count])+1];  //new char[MAX_SERVICE_NAME];
      strcpy(argv_forCallback[count], s_szArgv[count]);
      //DEBG_I_Print("Before richosmain() -> s_szArgv[%d]: %s \n", count, argv_forCallback[count]);
    }

    richosmain(argc, s_szArgv, &oMailActivityNotify);
  }

  for( int i = 0 ; i < MAX_MAIN_ARGC_VALUE ; i++ )
  {
    delete [] s_szArgv[i];
  }

  delete [] s_szArgv;

  return;
}

void CMainActivityNotify::PostInitialize(void)  // Will be changed to the master service concept
{
}

void CMainActivityNotify::PostFinalize(void)  // Will be changed to the master service concept
{ 
}

void TerminateMainTask(int argc, char* argv[])
{
  CMainActivityNotify oMailActivityNotify;
  char **s_szArgv = 0;
  int ret;

  s_szArgv = new char*[MAX_MAIN_ARGC_VALUE];
  for( int i = 0 ; i < MAX_MAIN_ARGC_VALUE ; i++ )
  {
    s_szArgv[i] = new char[MAX_SERVICE_NAME];
  }

  ret = Parse_RunSh(&argc, argv, s_szArgv);
  if( !ret )
  {
    Terminate(argc, s_szArgv, &oMailActivityNotify);
  }

  for( int i = 0 ; i < MAX_MAIN_ARGC_VALUE ; i++ )
  {
    delete [] s_szArgv[i];
  }

  delete [] s_szArgv;

  return;
}

// =====================************************************ ============================ Need to fix
void eventCallback(FAULT_T callback_event)
{
  if (callback_event == SIGINT)
  {
    printf("************************************ New TerminateMainTask will be started \n\n");
    TerminateMainTask(argc_forCallback, argv_forCallback);
    printf("\n\n************************************ New TerminateMainTask was done \n\n");
// =====================************************************ ============================ Need to fix
  }
}
// =====================************************************ ============================ Need to fix

int thread_main(int argc, char* argv[])
{
  int32_t nResult = 0;

  Port_DEBG_RegisterFaultView( FALL ,0, eventCallback); // jhlee - add callback

  printf("\n\nPFSP engine has been started\n\n");
  
  MainTask(argc, argv);

  nResult = GetReturnCode();

  return nResult;
}

int process_main(int argc, char* argv[])
{
  CMainActivityNotify oMailActivityNotify;
  pid_t* pPidFork = 0;
  char **s_szArgv = 0, **s_szRpArgv = 0;
  int nStateChild, nIndex = 0, nPidIdx = 0, nParentPid = 0, nResult = 0, nRpArgc;


  Port_DEBG_RegisterFaultView(FALL ,0, eventCallback); // jhlee - add callback

  printf("\n\nPFSP engine has been started\n\n");

  s_szArgv = new char*[MAX_MAIN_ARGC_VALUE];

  for( int i = 0 ; i < MAX_MAIN_ARGC_VALUE ; i++ )
  {
    s_szArgv[i] = new char[MAX_SERVICE_NAME];
  }

  nResult = Parse_RunSh(&argc, argv, s_szArgv);

  if( nResult != 0 )
  {
    return -1;
  }

  //Initial Port Map
  InitPortMap(s_szArgv);

  pPidFork = new pid_t[(argc - NUM_OF_PRE_ARGC)];
  s_szRpArgv = new char*[MAX_MAIN_ARGC_VALUE];

  for( nIndex = NUM_OF_PRE_ARGC ; nIndex < argc ; nIndex++, nPidIdx++ )
  {
    if( (argv[3][0] == '-') || (argv[4][0] == '-') )
    {
      for( int i = 0 ; i < argc ; i++)
      {
        s_szRpArgv[i] = new char[strlen(s_szArgv[i])+1];
        strcpy(s_szRpArgv[i],s_szArgv[i]);
      }
      nRpArgc = argc;
      nIndex = argc;
    }
    else
    {
      for( int i = 0 ; i < NUM_OF_PRE_ARGC ; i++ )
      {
        s_szRpArgv[i] = new char[strlen(s_szArgv[i])+1];
        strcpy(s_szRpArgv[i],s_szArgv[i]);
      }
      s_szRpArgv[NUM_OF_PRE_ARGC] = new char[strlen(s_szArgv[nIndex])+1];
      strcpy(s_szRpArgv[NUM_OF_PRE_ARGC],s_szArgv[nIndex]);
      nRpArgc = NUM_OF_PRE_ARGC + 1;
    }

    pPidFork[nPidIdx] = fork();	// store pids

    if( pPidFork[nPidIdx] == -1 )
    {
      DEBG_I_Print("(%s): fork failed\n\n", s_szArgv[nPidIdx]);
      DEBG_Assert(0);
      exit(1);
      return 1;
    }
    else if( pPidFork[nPidIdx] == 0 )
    {
      // child process routine
      richosmain(nRpArgc, s_szRpArgv, &oMailActivityNotify);

      for( int i = 0 ; i < MAX_MAIN_ARGC_VALUE ; i++ )
      {
        delete [] s_szArgv[i];
        delete [] s_szRpArgv[i];
      }

      delete [] s_szRpArgv;
      return nResult;
    }
    else
    {
      nParentPid = getpid();
    }
  }

  if( nParentPid == getpid() )
  {
    for( int idx=0 ; idx < nPidIdx ; idx++ )
    {
      //(void)wait(&nStateChild);
      pid_t state_info = wait(&nStateChild);
		  DEBG_I_Print("[NAME:(%d)]\n", state_info);
    }

    nResult = GetReturnCode();

    delete [] pPidFork;
    delete [] s_szArgv;

    DEBG_I_Print(("exit\n"));
  }

  return nResult;
}

PfspMwStart::PfspMwStart(void)
{
    
}

PfspMwStart::~PfspMwStart(void)
{

}

int PfspMwStart::PfspMwStartMain(int argc, char* argv[])
//int main(int argc, char* argv[])
{
  int32_t nResult = 0;
  
  LoadLibsOfAllComponents();
  
  if( strstr(argv[0], THREAD_NAME) )
  {
    nResult = thread_main(argc, argv);
  }
  else if( strstr(argv[0], PROCESS_NAME) )
  {
    nResult = process_main(argc, argv);
  }
  else
  {
    DEBG_I_Print(("usage : PFSP_p or PFSP_t /node config_file sub_config_dir /services.\n"));
    return -1;
  }

  return nResult;
}