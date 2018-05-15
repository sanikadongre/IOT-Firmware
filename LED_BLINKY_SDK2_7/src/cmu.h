//***********************************************************************************
// Include files
//***********************************************************************************
#include "main.h"
#include "em_cmu.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void cmu_init(void);
void cmu_letimer(CMU_ClkDiv_TypeDef prescaler);
void cmu_i2c(CMU_ClkDiv_TypeDef prescaler);
