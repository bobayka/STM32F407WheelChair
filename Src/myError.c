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
	
			if ( status == HAL_BUSY){// Get three projections of the accelerometer	
				myError er = I2C_ClearBusyFlagErratum(hi2c, 50);
				if (er.error != HAL_OK){
					RestartI2C1(hi2c);
					return Wrap(er, "cant clear BusyFlag:"); 					
				}						
				return NewError(status, "hal busy error\r");				
			} 
			RestartI2C1(hi2c);
			if (status == HAL_TIMEOUT){
				return NewError(status, "hal timeout\r");	
			}			
			return NewError(status, "hal error\r");				

}