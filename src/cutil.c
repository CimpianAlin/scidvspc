#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include "tclmy.h"

int logMemory = 0;

//HANDLE GetProcessHeap(VOID);

#ifdef WINCE
#include <tcl.h>
Tcl_Interp * currentTclInterp;

// =======================================================
char * my_Tcl_Alloc(int size) {

  char * buf = Tcl_AttemptAlloc(size);
  if (logMemory)
    printf("Alloc %u %d\n", (unsigned int) buf, size );
  if (buf == NULL) {
    Tcl_Eval(currentTclInterp, "tk_messageBox -type ok -icon error -parent . -title \"Scid\" -message \"Out of memory\nScid should crash rather quickly\"");
  }
  return buf;
}
// =======================================================
char *  my_Tcl_AttemptAlloc(int size){

  char * buf = Tcl_AttemptAlloc(size);
  if (logMemory)
    printf("Alloc %u %d\n", (unsigned int) buf, size );
  return buf;
}
// =======================================================
char * my_Tcl_Realloc(char * ptr, int size) {
  char * buf = Tcl_AttemptRealloc(ptr, size);
  if (logMemory)
    printf("Realloc %u -> %u %d\n", (unsigned int) ptr, (unsigned int) buf, size );
  if (buf == NULL) {
    Tcl_Eval(currentTclInterp, "tk_messageBox -type ok -icon error -parent . -title \"Scid\" -message \"Out of memory\nScid could crash rather quickly\"");
  }
  return buf;
}
// =======================================================
void my_Tcl_Free(char * ptr) {
  if (logMemory)
    printf("Free %u\n", (unsigned int) ptr );

  Tcl_Free(ptr);
}
// =======================================================
Tcl_Channel  my_Tcl_OpenFileChannel (Tcl_Interp * interp, CONST char * fileName, CONST char * modeString, int permissions){
  Tcl_Channel chan = Tcl_OpenFileChannel (currentTclInterp, fileName, modeString, permissions);
  if (chan == NULL) {
    char buf[200];
    sprintf(buf, "tk_messageBox -type ok -icon error -parent . -title \"Error\" -message \"Tcl_OpenFileChannel error %s\"", Tcl_ErrnoMsg(Tcl_GetErrno()));
    Tcl_Eval(currentTclInterp, buf);
  }
  return chan;
}
// =======================================================
// interpreter is set to NULL, so in case of any error, it is not appended to current result
Tcl_Channel  mySilent_Tcl_OpenFileChannel (Tcl_Interp * interp, CONST char * fileName, CONST char * modeString, int permissions){
  Tcl_Channel chan = Tcl_OpenFileChannel (NULL, fileName, modeString, permissions);
  return chan;
}
// =======================================================
int my_Tcl_Close (Tcl_Interp * interp, Tcl_Channel chan) {
  int res = Tcl_Close (currentTclInterp, chan);
  if (res != TCL_OK) {
      char buf[200];
    sprintf(buf, "tk_messageBox -type ok -icon error -parent . -title \"Error\" -message \"Tcl_Close error %s\"", Tcl_ErrnoMsg(Tcl_GetErrno()));
    Tcl_Eval(currentTclInterp, buf);
  }
  return res;
}
// =======================================================
int my_Tcl_Read (Tcl_Channel chan, char * bufPtr, int toRead) {
  int res = Tcl_Read (chan, bufPtr, toRead);
  if (res == -1) {
    char buf[200];
    sprintf(buf, "tk_messageBox -type ok -icon error -parent . -title \"Error\" -message \"Tcl_Read error %s\"", Tcl_ErrnoMsg(Tcl_GetErrno()));
    Tcl_Eval(currentTclInterp, buf);
  }
  return res;
}
// =======================================================
int my_Tcl_Write (Tcl_Channel chan, CONST char * s, int slen){
  int res;
  res = Tcl_Write ( chan, s, slen);
  if (res == -1) {
    char buf[200];
    sprintf(buf, "tk_messageBox -type ok -icon error -parent . -title \"Error\" -message \"Tcl_Write error %s\"", Tcl_ErrnoMsg(Tcl_GetErrno()));
    Tcl_Eval(currentTclInterp, buf);
  }
  return res;
}
// =======================================================
int my_Tcl_Flush (Tcl_Channel chan){
  int res = Tcl_Flush (chan);
  if (res != TCL_OK) {
    char buf[200];
    sprintf(buf, "tk_messageBox -type ok -icon error -parent . -title \"Error\" -message \"Tcl_Flush error %s\"", Tcl_ErrnoMsg(Tcl_GetErrno()));
    Tcl_Eval(currentTclInterp, buf);
  }
  return res;
}
// =======================================================
Tcl_WideInt my_Tcl_Seek (Tcl_Channel chan, Tcl_WideInt offset, int mode){
  Tcl_WideInt res = Tcl_Seek ( chan, offset, mode);
  if (res == -1) {
    char buf[200];
    sprintf(buf, "tk_messageBox -type ok -icon error -parent . -title \"Error\" -message \"Tcl_Seek error %s\"", Tcl_ErrnoMsg(Tcl_GetErrno()));
    Tcl_Eval(currentTclInterp, buf);
  }
  return res;
}
// =======================================================
Tcl_WideInt my_Tcl_Tell (Tcl_Channel chan){
  Tcl_WideInt res = Tcl_Tell (chan);
  if (res == -1) {
    Tcl_Eval(currentTclInterp, "tk_messageBox -type ok -icon error -parent . -title \"Error\" -message \"Tcl_Tell error\"");
  }
  return res;
}
// =======================================================
int my_Tcl_SetChannelOption ( Tcl_Interp * interp, Tcl_Channel chan, CONST char * optionName, CONST char * newValue){
  int res = Tcl_SetChannelOption ( currentTclInterp, chan, optionName, newValue);
  if (res == TCL_ERROR) {
    char buf[200];
    sprintf(buf, "tk_messageBox -type ok -icon error -parent . -title \"Error\" -message \"Tcl_SetChannelOption %s %s error\"", optionName, newValue);
    Tcl_Eval(currentTclInterp, buf);

  }
  return res;
}
// =======================================================
int my_Tcl_Eof (Tcl_Channel chan) {
  return Tcl_Eof (chan);
}

#endif
