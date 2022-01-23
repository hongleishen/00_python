/**************************************************************************
  Copyright (c) 2015 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @file   descript.h
  @brief  Endpoint 0 descriptor definitions for MUSBHSFC firmware

  @date    2015-04-26
  @author  Guo Yushi
  @version 0.1.0

  History: 1. 2015-04-26 the initial version
**************************************************************************/

#ifndef _DESC_H_
#define _DESC_H_

/* Define maximum number of interfaces in a configuration */
#define MAXIFS                   16

/* Define values for Standard Device Descriptor */
#define DEVDSC_CLASS_HS          0x00   /* High speed Device Class */
#define DEVDSC_SUBCLASS_HS       0x00   /* High speed Device SubClass */
#define DEVDSC_PROTOCOL_HS       0x00   /* High speed Device Protocol */
#define DEVDSC_VENDOR            0x2dbb /* Vendor ID */
#define DEVDSC_PRODUCT           0x0500 /* Product ID */
#define DEVDSC_RELEASE           0x0001 /* Release number */
#define DEVDSC_MANUI             1      /* Manufacturer string index */
#define DEVDSC_PRODI             2      /* Product string index */
#define DEVDSC_SERIALI           3      /* Serial number string index */
#define DEVDSC_CONFIGS_HS        1      /* Number of high speed configurations */
#define DEVDSC_CLASS_FS          0x00   /* Full speed Device Class */
#define DEVDSC_SUBCLASS_FS       0x00   /* Full speed Device SubClass */
#define DEVDSC_PROTOCOL_FS       0x00   /* Full speed Device Protocol */
#define DEVDSC_CONFIGS_FS        1      /* Number of full speed configurations */


/* Define values for descriptors in configuration 1 high speed */
#define CFGHS1_NUMIFS            1      /* Number of interfaces */
#define CFGHS1_CFGI              0      /* Configuration description string index */
#define CFGHS1_ATTRIBUTES        0xc0   /* Configuration attributes */
#define CFGHS1_MAXPOWER          0x0    /* Maximum power (units of 2 mA) */

/* Interface 0, alternate 0 */
#define CFGHS1_I0A0_NUMEPS       4      /* Number of endpoints */
#define CFGHS1_I0A0_IFCLASS      0xff   /* Interface class */
#define CFGHS1_I0A0_IFSUBCLASS   0      /* Interface subclass */
#define CFGHS1_I0A0_IFPROTOCOL   0      /* Interface protocol */
#define CFGHS1_I0A0_IFI          0      /* Interface string index */
#define CFGHS1_I0A0_0_EPADDR     0x81   /* Endpoint address (endpoint 1 IN) */
#define CFGHS1_I0A0_0_ATTRIB     0x02   /* Endpoint attributes (iso) */
#define CFGHS1_I0A0_0_MAXP       0x200  /* Endpoint max packet size (256 * 3) */
#define CFGHS1_I0A0_0_INTER      0      /* Endpoint interval */
#define CFGHS1_I0A0_1_EPADDR     0x82   /* Endpoint address (endpoint 2 IN) */
#define CFGHS1_I0A0_1_ATTRIB     0x02   /* Endpoint attributes (iso) */
#define CFGHS1_I0A0_1_MAXP       0x200  /* Endpoint max packet size (256 * 3) */
#define CFGHS1_I0A0_1_INTER      0      /* Endpoint interval */

#define CFGHS1_I0A0_2_EPADDR     0x83   /* Endpoint address (endpoint 2 IN) */
#define CFGHS1_I0A0_2_ATTRIB     0x02   /* Endpoint attributes (iso) */
#define CFGHS1_I0A0_2_MAXP       512     /* Endpoint max packet size (256 * 3) */
#define CFGHS1_I0A0_2_INTER      0      /* Endpoint interval */

#define CFGHS1_I0A0_3_EPADDR     0x84   /* Endpoint address (endpoint 2 IN) */
#define CFGHS1_I0A0_3_ATTRIB     0x02   /* Endpoint attributes (iso) */
#define CFGHS1_I0A0_3_MAXP       0x200  /* Endpoint max packet size (256 * 3) */
#define CFGHS1_I0A0_3_INTER      0      /* Endpoint interval */

/* Interface 0, alternate 1 */
#define CFGHS1_I0A1_NUMEPS       2      /* Number of endpoints */
#define CFGHS1_I0A1_IFCLASS      0xff   /* Interface class */
#define CFGHS1_I0A1_IFSUBCLASS   0      /* Interface subclass */
#define CFGHS1_I0A1_IFPROTOCOL   0      /* Interface protocol */
#define CFGHS1_I0A1_IFI          0      /* Interface string index */
#define CFGHS1_I0A1_0_EPADDR     0x81   /* Endpoint address (endpoint 2 IN) */
#define CFGHS1_I0A1_0_ATTRIB     0x02   /* Endpoint attributes (bulk) */
#define CFGHS1_I0A1_0_MAXP       0x0200 /* Endpoint max packet size (512) */
#define CFGHS1_I0A1_0_INTER      1      /* Endpoint interval */
#define CFGHS1_I0A1_1_EPADDR     0x82   /* Endpoint address (endpoint 2 OUT) */
#define CFGHS1_I0A1_1_ATTRIB     0x02   /* Endpoint attributes (bulk) */
#define CFGHS1_I0A1_1_MAXP       0x0200 /* Endpoint max packet size (512) */
#define CFGHS1_I0A1_1_INTER      1      /* Endpoint interval */

/* Interface 0, alternate 2 */
#define CFGHS1_I0A2_NUMEPS       2      /* Number of endpoints */
#define CFGHS1_I0A2_IFCLASS      0xff   /* Interface class */
#define CFGHS1_I0A2_IFSUBCLASS   0      /* Interface subclass */
#define CFGHS1_I0A2_IFPROTOCOL   0      /* Interface protocol */
#define CFGHS1_I0A2_IFI          0      /* Interface string index */
#define CFGHS1_I0A2_0_EPADDR     0x81   /* Endpoint address (endpoint 1 IN) */
#define CFGHS1_I0A2_0_ATTRIB     0x02   /* Endpoint attributes (bulk) */
#define CFGHS1_I0A2_0_MAXP       0x200  /* Endpoint max packet size (512) */
#define CFGHS1_I0A2_0_INTER      1      /* Endpoint interval */
#define CFGHS1_I0A2_1_EPADDR     0x01   /* Endpoint address (endpoint 1 OUT) */
#define CFGHS1_I0A2_1_ATTRIB     0x02   /* Endpoint attributes (bulk) */
#define CFGHS1_I0A2_1_MAXP       0x200  /* Endpoint max packet size (512) */
#define CFGHS1_I0A2_1_INTER      1      /* Endpoint interval */

/* Command structure */
typedef struct Command {
	BYTE    bmRequestType;
	BYTE    bRequest;
	WORD    wValue;
	WORD    wIndex;
	WORD    wLength;
} Command, *pCommand;

/* Command bit fields */
#define CMD_DATADIR         0x80

/* Request Type Field */
#define CMD_TYPEMASK        0x60
#define CMD_STDREQ          0x00
#define CMD_CLASSREQ        0x20
#define CMD_VENDREQ         0x40
#define CMD_STDDEVIN        0x80
#define CMD_STDDEVOUT       0x00
#define CMD_STDIFIN         0x81
#define CMD_STDIFOUT        0x01
#define CMD_STDEPIN         0x82
#define CMD_STDEPOUT        0x02
#define CMD_IMI             0xc9

/* Standard Request Codes */
#define GET_STATUS          0x00
#define CLEAR_FEATURE       0x01
#define SET_FEATURE         0x03
#define SET_ADDRESS         0x05
#define GET_DESCRIPTOR      0x06
#define SET_DESCRIPTOR      0x07
#define GET_CONFIGURATION   0x08
#define SET_CONFIGURATION   0x09
#define GET_INTERFACE       0x0A
#define SET_INTERFACE       0x0B
#define SYNCH_FRAME         0x0C
#define IMI_REQ             0x4D

/* Standard Descriptor Types */
#define DST_DEVICE       0x01
#define DST_CONFIG       0x02
#define DST_STRING       0x03
#define DST_INTERFACE    0x04
#define DST_ENDPOINT     0x05
#define DST_DEVQUAL      0x06
#define DST_OTHERSPEED   0x07
#define DST_POWER        0x08
#define CMD_DESCMASK     0xFF00
#define CMD_DEVICE       (DST_DEVICE << 8)
#define CMD_CONFIG       (DST_CONFIG << 8)
#define CMD_STRING       (DST_STRING << 8)
#define CMD_DEVQUAL      (DST_DEVQUAL << 8)
#define CMD_OTHERSPEED   (DST_OTHERSPEED << 8)

/* Endpoint transfer types */
#define EP_TFMASK         0x03
#define EP_CONTROL        0x00
#define EP_ISO            0x01
#define EP_BULK           0x10
#define EP_INTR           0x11

/* Standard Device Feature Selectors */
#define FTR_DEVREMWAKE    0x0001
#define FTR_TESTMODE      0x0002

/* Test Mode Selectors */
#define FTR_TESTJ         0x0100
#define FTR_TESTK         0x0200
#define FTR_TESTSE0       0x0300
#define FTR_TESTPKT       0x0400

#ifndef __align
#define __align(x)          __attribute__((aligned(x)))
#endif


/* Standard Device Descriptor */
typedef /*__packed*/ struct StdDevDscr {
	BYTE    bLength;
	BYTE    bDescriptorType;
	WORD    bcdUSB;
	BYTE    bDeviceClass;
	BYTE    bDeviceSubClass;
	BYTE    bDeviceProtocol;
	BYTE    bMaxPacketSize0;
	WORD    idVendor;
	WORD    idProduct;
	WORD    bcdDevice;
	BYTE    iManufacturer;
	BYTE    iProduct;
	BYTE    iSerialNumber;
	BYTE    bNumConfigurations;
} __packed StdDevDscr,  *pStdDevDscr;

/* Standard Device_Qualifier Descriptor */
typedef /*__packed*/ struct StdDevQualDscr {
	BYTE    bLength;
	BYTE    bDescriptorType;
	WORD    bcdUSB;
	BYTE    bDeviceClass;
	BYTE    bDeviceSubClass;
	BYTE    bDeviceProtocol;
	BYTE    bMaxPacketSize0;
	BYTE    bNumConfigurations;
	BYTE    bReserved;
} __packed StdDevQualdscr, *pStdDevQualdscr;

/* Standard Configuration Descriptor */
typedef /*__packed*/ struct StdCfgDesc {
	BYTE    bLength;
	BYTE    bDescriptorType;
	WORD    wTotalLength;
	BYTE    bNumInterfaces;
	BYTE    bConfigurationValue;
	BYTE    iConfiguration;
	BYTE    bmAttributes;
	BYTE    bMaxPower;
}  __packed StdCfgDscr, *pStdCfgDscr;

/* Standard Interface Descriptor */
typedef /*__packed*/ struct StdIfDesc {
	BYTE    bLength;
	BYTE    bDescriptorType;
	BYTE    bInterfaceNumber;
	BYTE    bAlternateSetting;
	BYTE    bNumEndpoints;
	BYTE    bInterfaceClass;
	BYTE    bInterfaceSubClass;
	BYTE    bInterfaceProtocol;
	BYTE    iInterface;
}  __packed StdIfDscr, *pStdIfDscr;

/* Standard Endpoint Descriptor */
typedef /*__packed*/ struct StdEpDscr {
	BYTE    bLength;
	BYTE    bDescriptorType;
	BYTE    bEndpointAddress;
	BYTE    bmAttributes;
	WORD    wMaxPacketSize;
	BYTE    bInterval;
}  __packed StdEpDscr, *pStdEpDscr;

typedef /*__packed*/ struct StdStrDscr {
	BYTE    bLength;
	BYTE    bDescriptorType;
	BYTE    bInterval[4];
}  __packed StdStrDscr, *pStdStrDscr;

typedef /*__packed*/ struct ImiResponse {
	WORD    wHead;
	WORD    wLength;
	WORD    wOpCode;
	WORD    wOpCount;
	WORD    wResult;
	BYTE    bAttach[54];
} __packed  ImiResponse, *pImiResponse;

typedef /*__packed*/ struct ImiCommand {
	WORD    wHead;
	WORD    wLength;
	WORD    wOpCode;
	WORD    wOpCount;
  //  WORD    wAttribute;
  //  int     bAttach;
	BYTE    data[6];
} __packed  ImiCommand, *pImiCommand;

typedef /*__packed*/ struct accelResponse {
	WORD   nMagic;
	WORD   nType;
	WORD   nPacketID;
	WORD   nBufSize;
	DWORD  nTimeStamp;
	BYTE   data[6];
} __packed  accelResponse;


/*
  Define configuration structures
  The configuration structures need to be modified to match the required configuration
  Each configuration should start with a configuration descriptor
  followed by 1 or more interface descriptors (each of which is followed by
  endpoint descriptors for all the endpoints in that interface).
  The interface descriptors should start with interface 0, alternate setting 0.
  Followed by any alternate settings for interface 0.
  Then interface 1 (if more than 1 interface in the configuration), followed
  by any alternate settings for interface 1.
  This arrangement should be repeated for all interfaces in the configuration.
*/

/* Configuration 1, high-speed */
typedef /*__packed*/ struct  HighSpeedCfg_1 {
  StdCfgDscr    stdCfg;     /* Required Standard Configuration Descriptor */
  StdIfDscr     stdIf00;    /* Interface 0 Alternate 0 Descriptor */
  StdEpDscr     stdEp00_0;  /* 1st Endpoint Descriptor for Interface 0 Alternate 0*/
  StdEpDscr     stdEp00_1;  /* 2nd Endpoint Descriptor for Interface 0 Alternate 0*/
#if 0
  StdEpDscr     stdEp00_2;  /* 1st Endpoint Descriptor for Interface 0 Alternate 0*/
  StdEpDscr     stdEp00_3;  /* 2nd Endpoint Descriptor for Interface 0 Alternate 0*/
  StdIfDscr     stdIf01;    /* Interface 0 Alternate 1 Descriptor */
  StdEpDscr     stdEp01_0;  /* 1st Endpoint Descriptor for Interface 0 Alternate 1*/
  StdEpDscr     stdEp01_1;  /* 2nd Endpoint Descriptor for Interface 0 Alternate 1*/
  StdIfDscr     stdIf02;    /* Interface 1 Alternate 1 Descriptor */
  StdEpDscr     stdEp02_0;  /* 1st Endpoint Descriptor for Interface 0 Alternate 2 */
  StdEpDscr     stdEp02_1;  /* 2nd Endpoint Descriptor for Interface 0 Alternate 2 */
  #endif
} HighSpeedCfg1, *pHighSpeedCfg1;

/* Top level high-speed structure containing pointers to each configuration */
typedef /*__packed*/ struct  HighSpeedCfgSet {
  pHighSpeedCfg1    pCfg1;      /* Pointer to HighSpeedCfg1 */
} __packed  HighSpeedCfgSet, *pHighSpeedCfgSet;



typedef  __align(4)  struct USB_DESCRIPTOR_T_
{
	/* Device descriptors are global */
	StdDevDscr         stdDevDsc;
	StdDevQualdscr     stdDevqDsc;

	/* Add configuration structures as defined in config.h */
	HighSpeedCfg1       hsCfg1;
	HighSpeedCfgSet     hsCfgSet;
}USB_DESCRIPTOR_T;

void usbInitDesc(USB_DESCRIPTOR_T *usbDesc);

#endif /* _DESCRIPT_H_ */
