#
/*
 *    Copyright (C) 202
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of sdrplay_tcp
 *
 *    sdrplay_tcp is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    sdrplay_tcp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with sdrplay_tcp; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#pragma once

#include	<QThread>
#include	<QSettings>
#include	<QSemaphore>
#include	<atomic>
#include	<complex>
#include	<stdio.h>
#include	<queue>
#include	"ringbuffer.h"
#include	"device-handler.h"
#include	"ui_sdrplay-widget-v3.h"
#include	<sdrplay_api.h>

#include	<mutex>
#include	<QScopedPointer>
#include	"Rsp-device.h"

class		baseConverter;
#ifndef	KHz
#define	KHz(x)	(1000 * x)
#endif
#ifndef	MHz
#define	MHz(x)	(1000 * KHz (x))
#endif

class	generalCommand;
class	xml_fileWriter;
class	errorLogger;
#include        "dlfcn.h"
typedef void    *HINSTANCE;

#ifdef __MINGW32__
//#include      "dlfcn.h"
#define GETPROCADDRESS  GetProcAddress
#else
#define GETPROCADDRESS  dlsym
#endif

class Server;

class	sdrplayHandler_v3 final:
	           public deviceHandler, public Ui_sdrplayWidget_v3 {
Q_OBJECT
public:
			sdrplayHandler_v3	(Server *,
	                                         QSettings *,
	                                         RingBuffer<std::complex<uint8_t>> *,
	                                         errorLogger *);
			~sdrplayHandler_v3	();

	bool		restartReader		(int32_t);
	void		stopReader		();
	void		resetBuffer		();
	int16_t		bitDepth		();
	QString		deviceName		();

	void		tcp_setFrequency	(int);
	void		tcp_setSampleRate	(int);
	void		tcp_setGainMode		(bool);
	void		tcp_setGain		(int);
	void		tcp_setAgc		(int);
	void		tcp_setPpm		(int);
	void		tcp_setBiasT		(bool);

	void            updatePowerOverload (
	                                 sdrplay_api_EventParamsT *params);
	std::atomic<bool>	receiverRuns;
	int		theGain;
	int		shifter;
	sdrplay_api_CallbackFnsT	cbFns;
	void		processBuffer	(std::complex<int16_t> *, int);

private:
public:
	sdrplay_api_Open_t              sdrplay_api_Open;
	sdrplay_api_Close_t             sdrplay_api_Close;
	sdrplay_api_ApiVersion_t        sdrplay_api_ApiVersion;
	sdrplay_api_LockDeviceApi_t     sdrplay_api_LockDeviceApi;
	sdrplay_api_UnlockDeviceApi_t   sdrplay_api_UnlockDeviceApi;
	sdrplay_api_GetDevices_t        sdrplay_api_GetDevices;
	sdrplay_api_SelectDevice_t      sdrplay_api_SelectDevice;
	sdrplay_api_ReleaseDevice_t     sdrplay_api_ReleaseDevice;
	sdrplay_api_GetErrorString_t    sdrplay_api_GetErrorString;
	sdrplay_api_GetLastError_t      sdrplay_api_GetLastError;
	sdrplay_api_DebugEnable_t       sdrplay_api_DebugEnable;
	sdrplay_api_GetDeviceParams_t   sdrplay_api_GetDeviceParams;
	sdrplay_api_Init_t              sdrplay_api_Init;
	sdrplay_api_Uninit_t            sdrplay_api_Uninit;
	sdrplay_api_Update_t            sdrplay_api_Update;
	sdrplay_api_SwapRspDuoActiveTuner_t sdrplay_api_SwapRspDuoActiveTuner;
	sdrplay_api_SwapRspDuoDualTunerModeSampleRate_t
		               sdrplay_api_SwapRspDuoDualTunerModeSampleRate;
	sdrplay_api_SwapRspDuoMode_t	sdrplay_api_SwapRspDuoMode;
	sdrplay_api_DeviceT             *chosenDevice;
	QScopedPointer<RspDevice>	theRsp;

	Server			*theServer;
	errorLogger		*theErrorLogger;
	std::atomic<bool>	failFlag;
	std::atomic<bool>	successFlag;
	float			denominator;
	std::atomic<bool>       threadRuns;
	void			run			();
	bool			messageHandler		(generalCommand *);

	QString			recorderVersion;
	
	int16_t			hwVersion;
	QSettings		*sdrplaySettings;
	bool			agcMode;
	int16_t			nrBits;
	int			lnaUpperBound;
	float			apiVersion;
	QString			serial;
	QString			deviceModel;
	int			GRdBValue;
	int			lnaState;
	HINSTANCE		Handle;
	double			ppmValue;
	bool			biasT;
	std::queue<generalCommand *>	serverQueue;
	QSemaphore		serverJobs;
	HINSTANCE               fetchLibrary            ();
	void                    releaseLibrary          ();
	bool			loadFunctions		();
	int			errorCode;
	int			setAntennaSelect	(int);

	void			computeGain		(int, int,
	                                                 int &, int &);
	sdrplay_api_Bw_MHzT	getBandwidth		(int);
	std::mutex		locker;
	baseConverter		*theConverter;
	void			set_converter		(int, int);
signals:
	void			newGRdBValue		(int);
	void			newLnaValue		(int);
	void			newAgcSetting		(bool);
	void			showTunerGain		(double);
private slots:
	void			setIfGainReduction	(int);
	void			setLnaGainReduction	(int);
	void			setAgcControl		(int);
	void			setPpmControl		(int);
	void			setSelectAntenna_RSPdx	(const QString &);
	void			setSelectAntenna_RSP2	(const QString &);
	void			setSelectAntenna_duo	(const QString &);
	void			setSelectTuner		(const QString &);
	void			setBiasT		(int);
	void			setNotch		(int);
	void			reportOverloadState	(bool);
	void			displayGain		(double);
public slots:
	void			setLnaBounds		(int, int);
	void			setSerial		(const QString &);
	void			setApiVersion		(float);
	void			showLnaGain		(int);
	void			showState		(const QString &);
//
//	just for the Duo
	void			enableBiasT		(bool);
signals:
	void			setLnaBoundsSignal	(int, int);
	void			setDeviceNameSignal	(const QString &);
	void			setSerialSignal		(const QString &);
	void			setApiVersionSignal	(float);
	void			setAntennaSelectSignal	(bool);
	void			overloadStateChanged	(bool);

	void			newData			(int);
};

