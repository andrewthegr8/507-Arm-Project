/*******************************************************************************
* Copyright © 2019 TRINAMIC Motion Control GmbH & Co. KG
* (now owned by Analog Devices Inc.),
*
* Copyright © 2023 Analog Devices Inc. All Rights Reserved. This software is
* proprietary & confidential to Analog Devices, Inc. and its licensors.
*******************************************************************************/

/*
 * TMC429 library definitions
 *
 * This file contains all macro and function definitions of the TMC429 library.
*/

#ifndef TMC_IC_TMC429_H_
#define TMC_IC_TMC429_H_

	#include "tmc/helpers/API_Header.h"
	#include "TMC429_Register.h"
	#include "main.h"

	//Redefine some functions names. This fix was suggested by chat and should be re-evaluated if we run into errors in deployment
	#define Write429U16(Address, Value) Write429Short((Address), (Value))
	#define Write429U24(Address, Value) Write429Int((Address), (Value))

	#define SPI_DEV_TMC429 (&hspi1) //Both chips on the same SPI bus, so we can use the same handle for both

	//We need to add functionality so that we can select which TMC chip we want to talk to
	/**
	 * @brief Identifier for which TMC429 motion IC is being addressed.
	 */
	typedef enum
	{
		MOTION_IC_1 = 0,
		MOTION_IC_2 = 1
	} MotionIC_t;

	/**
	 * @brief Configuration structure describing a TMC429 SPI device and its CS pin.
	 */
	typedef struct
	{
		SPI_HandleTypeDef *hspi; /**< SPI handle used to communicate with this chip */
		GPIO_TypeDef *csPort;   /**< Chip select GPIO port */
		uint16_t csPin;         /**< Chip select GPIO pin */
	} MotionIC_Config_t;



	// user must provide this function
	/**
	 * @brief Select which TMC429 device (chip) subsequent SPI operations target.
	 * @param ic MotionIC_t enum identifying the target chip.
	 */
	void SelectMotionIC(MotionIC_t ic);

	/**
	 * @brief Provide board-specific configuration for the two motion ICs.
	 * @param configs Pointer to an array (or single) MotionIC_Config_t structures.
	 */
	void TMC429_SetMotionICs(MotionIC_Config_t *configs);

	/**
	 * @brief Low-level SPI read/write helper; user board must provide an implementation.
	 * @param p_SPI_DeviceHandle Pointer to a SPI device handle (may be ignored)
	 * @param rx Pointer to receive byte(s) buffer.
	 * @param tx Pointer to transmit byte(s) buffer.
	 */
	void ReadWriteSPI(void* p_SPI_DeviceHandle, uint8_t * rx, uint8_t * tx);

	// TMC429 library functions
	void Init429(void);
	void ReadWrite429(uint8_t *Read, uint8_t *Write);
	void Write429Zero(uint8_t Address);
	void Write429Bytes(uint8_t Address, uint8_t *Bytes);
	void Write429Datagram(uint8_t Address, uint8_t HighByte, uint8_t MidByte, uint8_t LowByte);
	//void Write429U16(uint8_t Address, uint16_t Value);
	//void Write429U24(uint8_t Address, uint32_t Value);
	
	//Added these 2 functions as wrappers for the Write429U16 and Write429U24 functions
	void Write429Short(uint8_t Address, int32_t Value);
	void Write429Int(uint8_t Address, int32_t Value);
	
	//uint8_t Read429Status(void);
	uint8_t Read429Bytes(uint8_t Address, uint8_t *Bytes);
	uint8_t Read429SingleByte(uint8_t Address, uint8_t Index);
	int32_t Read429Int12(uint8_t Address);
	int32_t Read429Int24(uint8_t Address);
	void Set429RampMode(uint8_t Axis, uint8_t RampMode);
	void Set429SwitchMode(uint8_t Axis, uint8_t SwitchMode);
	uint8_t SetAMax(uint8_t Motor, uint32_t AMax);
	void HardStop(uint32_t Motor);

#endif /* TMC_IC_TMC429_H_ */
