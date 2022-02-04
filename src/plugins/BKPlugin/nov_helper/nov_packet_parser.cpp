#include "nov_packet_parser.h"
#include <qdebug.h>

//*********************************************************************************************
// DEFINE VARIABLES
//*********************************************************************************************


// adc --------------------------------------------------------------------
#define F_CPU		                   			quint32(100000000)
#define SEMPLES_PER_MS                 			quint32(862)

#define REF_ADC		                   			double(4.096)
#define RESOLUTION_ADC		                    double(32768.0)


// cmds --------------------------------------------------------------
// ping
#define PING		                   			quity8(0x00)
// self writer cmd`s
#define SELF_WRITER_STATUS         		   quint8(0x01)
#define SELF_WRITER_START          		   quint8(0x02)
#define GET_SELF_WRITER_DATA       		   quint8(0x03)
// commutator cmd
#define DEBUG_ENABLE_ON_OFF                 quint8(0x04)
// stream cmd
#define  DEBUG_STREAM_ON_OFF	       			quint8(0x05)
// nvo cmd
#define  START_STOP_DEBUG_WORKING_NVO	     	quint8(0x06)
#define  START_ONCE_CYCLE_NVO					quint8(0x07)
#define  SET_PARAMS_NVO							quint8(0x08)
#define SET_FILTER_INTENSITY					quint8(0x09)
#define SET_BOOOOOOM                            quint8(0x0A)
#define SET_FILTER_INTENSITY_SECOND					quint8(0x0B)


// resed all & stream -----------------------------------------------------
#define HARDWARE_RESET	    					quint8(0xFF)

#define STREAM_EVENT	    					quint8(0xFE)

// rx board id ------------------------------------------------------------
#define NOV_BID 		quint8(0x0E)
#define NOV_BID_DEBUG   quint8(0x1E)

// self writer mode--------------------------------------------------------
#define  NOTHING			                    quint8(0)
#define WRITE_ADC_DATA_CH1                   	quint8(1)
#define WRITE_ADC_DATA_CH2                   	quint8(2)
#define WRITE_ADC_DATA_CH3                   	quint8(3)
#define WRITE_ADC_DATA_CH4                   	quint8(4)
#define WRITE_ADC_DATA_CH5                   	quint8(5)
#define WRITE_ADC_DATA_CH6                   	quint8(6)
#define WRITE_ADC_DATA_CH7                   	quint8(7)
#define WRITE_ADC_DATA_CH8                   	quint8(8)
#define WRITE_ADC_NVO_TESTER_LOG                quint8(9)

//*********************************************************************************************
// END VARIABLES
//*********************************************************************************************


// nvo params proceed -------------------------------------------------------------------------------
quint8 NovPacketParser::getBid()
{
    return NOV_BID;
}

quint8 NovPacketParser::getRamCmd()
{
    return GET_SELF_WRITER_DATA;
}

QByteArray NovPacketParser::nvo_par_send(quint8 tokenId, quint32 generationTimeMs, quint32 latencyTimeMs, bool IsSerialPattern, quint8 pattern, quint8 filtWindow)
{
    quint32 generation_tics = (F_CPU/1000.0) * generationTimeMs;
    quint32 latency_tics = (F_CPU/1000.0) * latencyTimeMs;

    qDebug() << "gen time: " <<generationTimeMs << "g tics:" << generation_tics  << "latency:" << latencyTimeMs << "m tics:" << latency_tics;

    QByteArray cmd;

    cmd.append(NOV_BID); // board id
    cmd.append(tokenId);       // token id
    cmd.append(SET_PARAMS_NVO);     // cmd

    // tester
    cmd.append(quint8(generation_tics >> 0));
    cmd.append(quint8(generation_tics >> 8));
    cmd.append(quint8(generation_tics >> 16));
    cmd.append(quint8(generation_tics >> 24));
    cmd.append(quint8(latency_tics >> 0));
    cmd.append(quint8(latency_tics >> 8));
    cmd.append(quint8(latency_tics >> 16));
    cmd.append(quint8(latency_tics >> 24));

    // worker
    cmd.append(quint8(IsSerialPattern));
    cmd.append(pattern);
    cmd.append(filtWindow);

    return cmd;
}

QByteArray NovPacketParser::debugGenerationSend(quint8 tokenId, bool enable)
{
    QByteArray cmd;
    cmd.append(NOV_BID); // board id
    cmd.append(tokenId);       // token id
    cmd.append(START_STOP_DEBUG_WORKING_NVO);     // cmd
    cmd.append(quint8(enable));
    return cmd;
}

QByteArray NovPacketParser::startOnceMeasure(quint8 tokenId)
{
    QByteArray cmd;
    cmd.append(NOV_BID); // board id
    cmd.append(tokenId);       // token id
    cmd.append(START_ONCE_CYCLE_NVO);
    return cmd;
}

QByteArray NovPacketParser::streamEnable(quint8 tokenId, bool enable)
{
    QByteArray cmd;
    cmd.append(NOV_BID); // board id
    cmd.append(tokenId);       // token id
    cmd.append(DEBUG_STREAM_ON_OFF);     // cmd
    cmd.append(quint8(enable));
    return cmd;
}

QByteArray NovPacketParser::setFirstOrderFilter(quint8 tokenId, quint16 value)
{
    QByteArray cmd;
    cmd.append(NOV_BID); // board id
    cmd.append(tokenId);       // token id
    cmd.append(SET_FILTER_INTENSITY);     // cmd
    cmd.append(quint8(value >> 0));
    cmd.append(quint8(value >> 8));
    return cmd;
}

QByteArray NovPacketParser::setSeconOrderFilter(quint8 tokenId, quint16 value)
{
    QByteArray cmd;
    cmd.append(NOV_BID); // board id
    cmd.append(tokenId);       // token id
    cmd.append(SET_FILTER_INTENSITY_SECOND);     // cmd
    cmd.append(quint8(value >> 0));
    cmd.append(quint8(value >> 8));
    return cmd;
}

//debugMode ==> 0-tester 1-worker
QByteArray NovPacketParser::setDebugEnable(quint8 tokenId, bool enable, bool debugMode)
{
    QByteArray cmd;
    cmd.append(NOV_BID); // board id
    cmd.append(tokenId);       // token id
    cmd.append(DEBUG_ENABLE_ON_OFF);     // cmd
    cmd.append(quint8(enable));
    cmd.append(quint8(debugMode));
    return cmd;
}

QByteArray NovPacketParser::BOOOOOOOOOOOOOOOOM(quint8 tokenId, bool boooomEnable)
{
    QByteArray cmd;
    cmd.append(NOV_BID); // board id
    cmd.append(tokenId);       // token id
    cmd.append(SET_BOOOOOOM);     // cmd
    cmd.append(quint8(boooomEnable));
    return cmd;
}

QByteArray NovPacketParser::startSelfWriter(quint8 tokenId, int selfMode)
{
    QByteArray cmd;
    cmd.append(NOV_BID); // board id
    cmd.append(tokenId);       // token id
    cmd.append(SELF_WRITER_START);     // cmd
    cmd.append(quint8(selfMode));
    return cmd;
}

