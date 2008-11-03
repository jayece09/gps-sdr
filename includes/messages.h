/*! \file messages.h
	Define messages used for RS422 interface
*/
/************************************************************************************************
Copyright 2008 Gregory W Heckler

This file is part of the GPS Software Defined Radio (GPS-SDR)

The GPS-SDR is free software; you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GPS-SDR is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with GPS-SDR; if not,
write to the:

Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
************************************************************************************************/

#ifndef MESSAGES_H
#define MESSAGES_H


#define CCSDS_APID_BASE	(0x0)	//!< The CCSDS APID Base number for our receiver

/* Enum the packet ID #s */
enum CCSDS_PACKET_IDS
{
	BOARD_HEALTH_M_ID,
	TASK_HEALTH_M_ID,
	CHANNEL_HEALTH_M_ID,
	SPS_M_ID,
	CLOCK_M_ID,
	SV_POSITION_M_ID,
	EKF_M_ID,
	MEASUREMENT_M_ID,
	PSEUDORANGE_M_ID,
	EPHEMERIS_M_ID,
	ALMANAC_M_ID,
	COMMAND_ACQ_M_ID
};


/* CCSDS Packet Header */
typedef struct CCSDS_PH
{

	uint16 pid;	//!< Packet ID
	uint16 psc;	//!< Packet sequence control
	uint16 pdl; //!< Packet data length

};


/* CCSDS Command Header */
typedef struct CCSDS_CH
{

	uint16 pid;	//!< Packet ID
	uint16 psc;	//!< Packet sequence control
	uint16 pdl; //!< Packet data length

};


typedef struct Board_Health_M
{

	//!< Grab FPGA versions
	uint32 trk_version;		//!< Tracking FPGA version
	uint32 acq_version;		//!< Acquisition FPGA version

	//!< DSA Info
	uint32 dsa0;			//!< Current state of RF DSA on RF chain 0
	uint32 dsa1;			//!< Current state of IF DSA on RF chain 0
	uint32 dsa2;			//!< Current state of RF DSA on RF chain 1
	uint32 dsa3;			//!< Current state of IF DSA on RF chain 1

	//!< LO lock indicator
	uint32 lo_locked;		//!< Is the LO (1.54 GHz downmix clock) locked to the synthesizer

	//!< Overflows on A/Ds
	uint32 ovrflw0;			//!< Overflow counter on A/D 0
	uint32 ovrflw1;			//!< Overflow counter on A/D 1
	uint32 ovrflw2;			//!< Overflow counter on A/D 2
	uint32 ovrflw3;			//!< Overflow counter on A/D 3

	//!< Acquisition SRAM
	uint32 sram_bad_mem;	//!< Debug info from Steve's POST
	uint32 sram_bad_hi;		//!< Debug info from Steve's POST
	uint32 sram_bad_lo;		//!< Debug info from Steve's POST

	//!< Missed interrupts
	uint32 missed_interrupts;//!< Missed interrupts

	//!< Software revision
	uint32 software_major;	//!< Major version
	uint32 software_minor;	//!< Minor version
	uint32 software_postfix;//!< Alpha, beta, or gold

};


typedef struct_Task_Health_M
{

	uint32 execution_tic[MAX_TASKS];	//!< Execution counters
	uint32 start_tic[MAX_TASKS];		//!< Nucleus tic at function entry
	uint32 stop_tic[MAX_TASKS];			//!< Nucleus tic at function exit

};


/*! \ingroup MESSAGES
 * Packet dumped to telemetry and to disk to keep track of each channel
 */
typedef struct Channel_Health_M
{

	uint32	chan;			//!< The channel number
	uint32	state;			//!< channel's state
	uint32	svnum;			//!< SV/PRN number the channel is tracking
	uint32	antenna;		//!< Antenna channel is tracking off of
	uint32	accum_len;		//!< acummulation length (1 or 20 ms)

	float	w;				//!< 3rd order PLL state
	float	x;				//!< 3rd order PLL state
	float	f;				//!< 3rd order PLL state
	uint32	CN0;			//!< CN0 estimate
	int32	pp_fltr;		//!< Filtered version of I^2+Q^2

	uint32	bit_lock;		//!< Bit lock?
	uint32	frame_lock;		//!< Frame lock?
	uint32	navigate;		//!< Navigate on this channel flag
	uint32	active_count;	//!< Number of accumulations that have been processed
	uint32 	subframe;		//!< Current subframe number
	uint32  bit_epoch;		//!< Best estimate of bit edge position

};


/*! \ingroup MESSAGES
	Raw PVT navigation solution
*/
typedef struct SPS_M
 {

	double x;			//!< ECEF x coordinate (meters)
	double y;			//!< ECEF y coordinate (meters)
	double z;			//!< ECEF z coordinate (meters)
	double vx;			//!< ECEF x velocity (meters/sec)
	double vy;			//!< ECEF x velocity (meters/sec)
	double vz;			//!< ECEF x velocity (meters/sec)
	double ax;			//!< ECEF x acceleration (meters/sec/sec)
	double ay;			//!< ECEF x acceleration (meters/sec/sec)
	double az;			//!< ECEF x acceleration (meters/sec/sec)

	double time;		//!< time in seconds
	double clock_bias;	//!< clock bias in seconds
	double clock_rate;  //!< clock rate in meters/second
	double latitude;	//!< latitude in decimal radians
	double longitude;	//!< longitude in decimal radians
	double altitude;	//!< height in meters

	double gdop;		//!< geometric dilution of precision
	double pdop;		//!< position dilution of precision
	double tdop;		//!< time dilution of precision
	double hdop;		//!< hdop diultion of precision
	double vdop;		//!< vertical dilution of precision

	int32 nsvs;			//!< This is a mask, not a number
	int32 converged;	//!< declare convergence
	int32 tic;			//!< global_tic associated with this solution

	int32 stale_ticks;			//!< count the number of tics since the last good sltn
	int32 converged_ticks;		//!< count number of converged tics
	int32 nav_channels;			//!< count number of SVs used in last PVT estimation
	int32 initial_convergence;	//!< Flag set ONCE if the first convergence has occured

	int32 chanmap[MAX_CHANNELS];

};


/*! \ingroup MESSAGES
	Contains the clock state
*/
typedef struct Clock_M
{

	double receiver_time;		//!< Elapsed receiver time
	double rate;	 			//!< Clock rate
	double bias; 				//!< Clock bias
	double time0;				//!< Guess of gps second at initialization
	double time;				//!< Best estimate of GPStime
	double time_raw;			//!< Uncorrected time
	double week;				//!< GPS week
	uint32 state;				//!< Clock state

};


/*! \ingroup MESSAGES
	Contains the SV position used for the PVT solution
*/
typedef struct SV_Position_M
{

	double x;				//!< ECEF x coordinate (meters)
	double y;				//!< ECEF y coordinate (meters)
	double z;				//!< ECEF z coordinate (meters)
	double vx;				//!< ECEF x velocity (meters/sec)
	double vy;				//!< ECEF x velocity (meters/sec)
	double vz;				//!< ECEF x velocity (meters/sec)
	double ax;				//!< ECEF x acceleration (meters/sec/sec)
	double ay;				//!< ECEF x acceleration (meters/sec/sec)
	double az;				//!< ECEF x acceleration (meters/sec/sec)
	double elev;			//!< Satellite elevation (radians)
	double azim;			//!< Satellite azimuth (radians)
	double clock_bias;		//!< SV clock bias
	double frequency_bias;	//!< SV clock rate bias
	double transit_time;	//!< Time of flight from SV to receiver
	double time;			//!< Time used in SV position calculation
	double latitude;		//!< Latitude using WGS-84 ellipsoid in decimal radians
	double longitude;		//!< Longitude using WGS-84 ellipsoid in decimal radians
	double altitude;		//!< height in meters

};


/*! \ingroup MESSAGES
	EKF navigation solution
*/
typedef struct EKF_M
 {

	double x;			//!< ECEF x coordinate (meters)
	double y;			//!< ECEF y coordinate (meters)
	double z;			//!< ECEF z coordinate (meters)
	double vx;			//!< ECEF x velocity (meters/sec)
	double vy;			//!< ECEF x velocity (meters/sec)
	double vz;			//!< ECEF x velocity (meters/sec)
	double ax;			//!< ECEF x acceleration (meters/sec/sec)
	double ay;			//!< ECEF x acceleration (meters/sec/sec)
	double az;			//!< ECEF x acceleration (meters/sec/sec)

	double time;		//!< time in seconds
	double clock_bias;	//!< clock bias in seconds
	double clock_rate;  //!< clock rate in meters/second
	double latitude;	//!< latitude in decimal radians
	double longitude;	//!< longitude in decimal radians
	double altitude;	//!< height in meters

	double gdop;		//!< geometric dilution of precision
	double pdop;		//!< position dilution of precision
	double tdop;		//!< time dilution of precision
	double hdop;		//!< hdop diultion of precision
	double vdop;		//!< vertical dilution of precision

	int32 nsvs;			//!< This is a mask, not a number
	int32 converged;	//!< declare convergence
	int32 tic;			//!< global_tic associated with this solution

	int32 stale_ticks;			//!< count the number of tics since the last good sltn
	int32 converged_ticks;		//!< count number of converged tics
	int32 nav_channels;			//!< count number of SVs used in last PVT estimation
	int32 initial_convergence;	//!< Flag set ONCE if the first convergence has occured

	int32 chanmap[MAX_CHANNELS];

};


/*! \ingroup STRUCTS
 * The measurement dumped to the PVT object
 */
typedef struct Pseudorange_M
{

	double	code_time;					//!< The code time
	double 	code_phase; 				//!< Code phase (chips)
	double 	carrier_phase;				//!< Carrier phase (cycles)
	double 	carrier_phase_prev;			//!< Carrier phase prev (cycles)
	double 	carrier_phase_prev_prev;	//!< Carrier phase prev prev (cycles)
	double 	code_phase_mod;				//!< Code phase (chips), mod 1023
	double 	carrier_phase_mod;			//!< Carrier phase (cycles), mod 1
	double 	code_nco;					//!< Code NCO
	double 	carrier_nco;				//!< Carrier NCO
	int32  	_1ms_epoch;					//!< _1ms_epoch
	int32  	_20ms_epoch;				//!< _20ms_epoch
	int32	_z_count;					//!< The z count
	int32 	navigate;					//!< This has been tagged as a good measurement
	int32	sv;							//!< For this sv
	int32	chan;						//!< For this channel
	int32 	count;						//!< Corresponds to this tic

};


/*! \ingroup MESSAGES
	Pseudoranges structure, holds both time and meters
*/
typedef struct Pseudorange_M
{

	double gpstime;			//!< Time tag associated with pseudorange
	double time;			//!< pseudorange in seconds
	double time_rate;		//!< pseudorange rate in sec/sec
	double meters;			//!< pseudorange in meters
	double meters_rate;		//!< pseudorange rate in meters/sec
	double residual;		//!< residual in meters
	double rate_residual;	//!< rate residual (m/s)
	double time_uncorrected;//!< raw pseudorange measurements
	double previous;		//!< from previous step, used for err check

};


/*! \ingroup MESSAGES
	Decoded ephemeris struct
*/
typedef struct Ephemeris_M
{

	int32	valid;					//!< 0=No valid data.
	int32	tofxmission;			//!< Time of subframe 1 transmission, sec of week.
	int32	tow;					//!< Truncated TOW count
	int32	subframe_1_health;      //!< Subframe 1 health code.
	int32	code_on_L2;             //!< Code on L2 flag.
	int32	week_number;			//!< GPS week at time of subframe 1 reception.
	int32	L2pdata;                //!< L2 P data flag.
	int32	ura;                    //!< Satellite's URA code.
	int32	iodc;                   //!< Issue of data, clock.
	double	tgd;                    //!< Group delay parameter.
	int32	tocwk;					//!< GPS week corresponding to toc.
	double	toc;					//!< Reference time of clock data parameter set.
	double	af0;					//!< Clock correction polynomial coefficient.
	double	af1;					//!< Clock correction polynomial coefficient.
	double	af2;					//!< Clock correction polynomial coefficient.
	int32	iode;                   //!< Issue of data, ephemeris.
	double	crs;					//!< Sine harmonic correction to orbital radius.
	double	deltan;					//!< Mean motion delta from computed value.
	double	m0;                     //!< Mean anomaly at TOE.
	double	cuc;					//!< Cosine harmonic correction to orbital radius.
	double	ecc;                    //!< Eccentricity.
	double	cus;					//!< Sine harmonic corr to argument of latitude.
	double	sqrta;                  //!< Square root of semimajor axis.
	int32	toewk;                  //!< GPS week corresponding to toe.
	double	toe;					//!< Reference time of ephemeris data set.
	int32	fti;                    //!< Fit interval.
	double	cic;					//!< Cosine harmonic corr to inclination.
	double	om0;                    //!< Right ascension at TOE.
	double	cis;					//!< Sine harmonic corr to inclination.
	double	in0;                    //!< Inclination at TOE.
	double	crc;					//!< Cosine harmonic correction to orbital radius.
	double	argp;                   //!< Argument of perigee at TOE.
	double	omd;                    //!< Rate of right ascension.
	double	idot;                   //!< Rate of inclination.
	double	a;						//!< Derived qty: a = sqrta**2.
	double	n0;						//!< Derived qty: n0 = sqrt(GravConstant/(a*a*a)).
	double	relativistic;			//!< Relativistic correction
	int32	zcount;
	int32	sv;

};


/*! \ingroup MESSAGES
	Decoded almanac struct
*/
typedef struct Almanac_M
{

	uint32	decoded;				//!< Has this been decoded yet
	int32	health;					//!< Health code
	double	ecc;					//!< Eccentricity
	double	toa;					//!< Time of Almanac
	double	in0;					//!< Inclination
	double	omd;					//!< Rate of right ascention
	double	sqrta;					//!< Sqrt of Semi-Major Axis
	double	om0;					//!< Longitude of Ascending Node
	double	argp;					//!< Argument of perigee
	double	m0;						//!< Mean Anomaly
	double	af0;					//!< Clock parameter 0
	double	af1;					//!< Clock parameter 1
	int32	week;					//!< Week number

};


/*! \ingroup MESSAGES
	Acknowledge processing of command
*/
typedef struct Command_Ack_M
{

	uint32 command_id;
	uint32 command_count;

};

#endif /* MESSAGES_H */


