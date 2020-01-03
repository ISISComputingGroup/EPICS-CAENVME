asynStatus GalilController::drvUserCreate(asynUser *pasynUser, const char* drvInfo, const char** pptypeName, size_t* psize)
{
   char *drvInfocpy;				//copy of drvInfo
   char *charstr;				//The current token
   char *tokSave = NULL;			//Remaining tokens

   if (strncmp(drvInfo, "USER_", 5) == 0)
     {
     //take a copy of drvInfo
     drvInfocpy = epicsStrDup((const char *)drvInfo);
     //split drvInfocpy into tokens
     //first token is DRVCMD = CMD, OCTET, OCTET_VAL, or VAR
     charstr = epicsStrtok_r((char *)drvInfocpy, " ", &tokSave);
     if (!abs(strcmp(charstr, GalilUserCmdString)))
        pasynUser->reason = GalilUserCmd_;
     if (!abs(strcmp(charstr, GalilUserOctetString)))
        pasynUser->reason = GalilUserOctet_;
     if (!abs(strcmp(charstr, GalilUserOctetValString)))
        pasynUser->reason = GalilUserOctetVal_;
     if (!abs(strcmp(charstr, GalilUserVarString)))
        pasynUser->reason = GalilUserVar_;
     //Second token is GalilStr
     charstr = epicsStrtok_r(NULL, "\n", &tokSave);
     //Store copy of GalilStr in pasynuser userdata
     if (charstr != NULL)
        pasynUser->userData = epicsStrDup(charstr);
     //Free the ram we used
     free(drvInfocpy);
     return asynSuccess;
     }
  else
     {
     return asynMotorController::drvUserCreate(pasynUser, drvInfo, pptypeName, psize);
     }
}

asynStatus GalilController::drvUserDestroy(asynUser *pasynUser)
{
   const char *functionName = "drvUserDestroy";
   if ( pasynUser->reason >= GalilUserCmd_ && pasynUser->reason <= GalilUserVar_ )
      {
      asynPrint(pasynUser, ASYN_TRACE_FLOW,
          "%s:%s: index=%d uservar=%s\n", 
          driverName, functionName, pasynUser->reason, (const char*)pasynUser->userData);
      free(pasynUser->userData);
      pasynUser->userData = NULL;
      return(asynSuccess);
      }
  else
      {
      return asynMotorController::drvUserDestroy(pasynUser);
      }
}
