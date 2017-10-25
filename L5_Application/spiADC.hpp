/*
 * spiADC.hpp
 *
 *  Created on: May 14, 2016
 *      Author: Abraham
 */

#ifndef L5_APPLICATION_SPIADC_HPP_
#define L5_APPLICATION_SPIADC_HPP_

namespace  spiADC
{
       char spiTransfer(char in);
       void SSP_Init(void);
       void control_init(void);
       void ADC_DS(void);
       void ADC_CS(void);
       int read_ADC(char data);
};

#endif /* L5_APPLICATION_SPIADC_HPP_ */
