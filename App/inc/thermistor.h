/*

Description: contains data from thermistor datasheet

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Sergey Dushin, Yury Rassadin
*/
/**
  ******************************************************************************
  * @file    thermistor.h
  * @author  ICS Mutant Ninja Turtles Team
  * @brief   contains array of reference info from B57861-S 103-F40 datasheet
  *          this is 8016 R/T characterictics
  ******************************************************************************
  *
  ******************************************************************************
  */

/* it was modified by Sergey & Yury-------------------------------------*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __THERMISTOR_H__
#define __THERMISTOR_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
const long ResTemp[] = {
 96.3,
 67.01,
 47.17,
 33.65,
 24.26,
 17.7,
 13.04,
 9.707,
 7.293,
 5.533,
 4.232,
 3.265,
 2.539,
 1.99,
 1.571,
 1.249,
 1.0000,
 0.8057,
 0.6531,
 0.5327,
 0.4369,
 0.3603,
 0.2986,
 0.2488,
 0.2083,
 0.1752,
 0.1481,
 0.1258,
 0.1072,
 0.09177,
 0.07885,
 0.068,
 0.05886,
 0.05112,
 0.04454,
 0.03893,
 0.03417,
 0.03009,
 0.02654,
 0.02348,
 0.02083,
 0.01853,
 0.01653
 };

 /////////// this function searches for linear approximation segment
 /////////// in EPCOS datasheet R/T characteristic # 8016
 static long CalculateTemperature(long r_eq)
    {
 	   long t;
	   uint8_t i = 0;
 	   while (ResTemp[i] > r_eq)
 	      {i++;}
 	   t = -55 + 5*(i - (r_eq-ResTemp[i])/(ResTemp[i-1]-ResTemp[i]));
 	   return t;
    };
/////////////////////////////////////////////////////////////////////////
////////     CODE FROM aterlux.ru         //////////////////////////////
 #define TEMPERATURE_UNDER -550
 //
 #define TEMPERATURE_OVER 1550

 #define TEMPERATURE_TABLE_START -550
 #define TEMPERATURE_TABLE_STEP 50

 typedef uint16_t temperature_table_entry_type;
 typedef uint8_t temperature_table_index_type;
 #define TEMPERATURE_TABLE_READ(i) termo_table[i]


 const temperature_table_entry_type termo_table[] = {
     4054, 4036, 4011, 3978, 3934, 3877, 3804, 3713,
     3602, 3469, 3313, 3136, 2939, 2726, 2503, 2275,
     2048, 1828, 1618, 1424, 1245, 1085, 942, 816,
     706, 611, 528, 458, 397, 344, 299, 261,
     228, 199, 175, 153, 135, 120, 106, 94,
     84, 75, 67
 };


 int16_t calc_temperature(temperature_table_entry_type adcsum) {
   temperature_table_index_type l = 0;
   temperature_table_index_type r = (sizeof(termo_table) / sizeof(termo_table[0])) - 1;
   temperature_table_entry_type thigh = TEMPERATURE_TABLE_READ(r);
   temperature_table_entry_type tlow = TEMPERATURE_TABLE_READ(0);

   if (adcsum <= thigh) {
     #ifdef TEMPERATURE_UNDER
       if (adcsum < thigh)
         return TEMPERATURE_UNDER;
     #endif
     return TEMPERATURE_TABLE_STEP * r + TEMPERATURE_TABLE_START;
   }

   if (adcsum >= tlow) {
     #ifdef TEMPERATURE_OVER
       if (adcsum > tlow)
         return TEMPERATURE_OVER;
     #endif
     return TEMPERATURE_TABLE_START;
   }
	// PRINTF ("(%d,%d), ",l,r);
   while ((r - l) > 1) {

     temperature_table_index_type m = (l + r) >> 1;
     temperature_table_entry_type mid = TEMPERATURE_TABLE_READ(m);
     if (adcsum > mid) {
       r = m;
     } else {
       l = m;
     }
	 //PRINTF ("(%d,%d), ",l,r);
   }

   temperature_table_entry_type vl = TEMPERATURE_TABLE_READ(l);
   if (adcsum >= vl) {
     return l * TEMPERATURE_TABLE_STEP + TEMPERATURE_TABLE_START;
   }
   temperature_table_entry_type vr = TEMPERATURE_TABLE_READ(r);
   temperature_table_entry_type vd = vl - vr;
   int16_t res = TEMPERATURE_TABLE_START + r * TEMPERATURE_TABLE_STEP;
   if (vd) {
     res -= ((TEMPERATURE_TABLE_STEP * (int32_t)(adcsum - vr) + (vd >> 1)) / vd);
   }
   return res;
 }

/////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif

#endif /* __HW_CONF_L1_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
