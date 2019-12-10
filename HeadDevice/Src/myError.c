#include "myError.h"
#include "string.h"
#include <stdlib.h>
#include "I2C_ClearBusyFlagErratum.h"
#include "RestartI2C1.h"

#define ErrBufSize 200
static char buffForError[ErrBufSize];

 enum 
{
	FALSE,
	TRUE
	
};

myError NewError(HAL_StatusTypeDef error, char* msg){	
	memset(buffForError, 0 , ErrBufSize);
	strcpy(buffForError, msg);
	struct myMsg m = {buffForError, strlen(buffForError)};
	myError err = {error, m};
	return err;
}

myError Wrap (myError err, char* msg){ // after wrap can be free free result!!!
	const size_t len1 = strlen(msg);
	const size_t len2 = err.msg.size;
	char messageBuff[len2 + 1];
	strcpy(messageBuff, err.msg.msg);
	memcpy(buffForError, msg, len1);
	memcpy(buffForError + len1, messageBuff, len2 + 1); // +1 to copy the null-terminator
	err.msg.size = strlen(buffForError);
	return err;
}

myError I2CErrorHandling(I2C_HandleTypeDef* hi2c,HAL_StatusTypeDef status){
	RestartI2C1(hi2c);
	myError er = I2C_ClearBusyFlagErratum(hi2c, 30);
	if (er.error != HAL_OK){

		return Wrap(er, "cant clear BusyFlag:"); 					
	}else{
		if ( status == HAL_BUSY){// Get three projections of the accelerometer	
			return NewError(status, "hal busy error was handled\r");				
		} 
		
		if (status == HAL_TIMEOUT){
			return NewError(status, "was: hal timeout was handled\r");	
		}			
	}	
	return NewError(status, "was: hal error was handled\r");				

}
