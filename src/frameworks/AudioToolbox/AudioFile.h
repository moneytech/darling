#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include <CoreServices/MacTypes.h>
#include <CoreFoundation/CFURL.h>

#define kAFInfoDictionary_Artist "artist"
#define kAFInfoDictionary_Album "album"
#define kAFInfoDictionary_Tempo "tempo"
#define kAFInfoDictionary_KeySignature "key signature"
#define kAFInfoDictionary_TimeSignature "time signature"
#define kAFInfoDictionary_TrackNumber "track number"
#define kAFInfoDictionary_Year "year"
#define kAFInfoDictionary_Composer "composer"
#define kAFInfoDictionary_Lyricist "lyricist"
#define kAFInfoDictionary_Genre "genre"
#define kAFInfoDictionary_Title "title"
#define kAFInfoDictionary_RecordedDate "recorded date"
#define kAFInfoDictionary_Comments "comments"
#define kAFInfoDictionary_Copyright "copyright"
#define kAFInfoDictionary_SourceEncoder "source encoder"
#define kAFInfoDictionary_EncodingApplication "encoding application"
#define kAFInfoDictionary_NominalBitRate "nominal bit rate"
#define kAFInfoDictionary_ChannelLayout "channel layout"
#define kAFInfoDictionary_ApproximateDurationInSeconds "approximate duration in seconds"
#define kAFInfoDictionary_SourceBitDepth "source bit depth"
#define kAFInfoDictionary_ISRC "ISRC"
#define kAFInfoDictionary_SubTitle "subtitle"

enum
{
	kAudioFileReadPermission		= 0x01,
	kAudioFileWritePermission		= 0x02,
	kAudioFileReadWritePermission	= 0x03
};

enum
{
	kAudioFileFlags_EraseFile				= 1,
	kAudioFileFlags_DontPageAlignAudioData	= 2
};

enum {
	kAudioFileAIFFType            = 'AIFF',
	kAudioFileAIFCType            = 'AIFC',
	kAudioFileWAVEType            = 'WAVE',
	kAudioFileSoundDesigner2Type  = 'Sd2f',
	kAudioFileNextType            = 'NeXT',
	kAudioFileMP3Type             = 'MPG3',
	kAudioFileMP2Type             = 'MPG2',
	kAudioFileMP1Type             = 'MPG1',
	kAudioFileAC3Type             = 'ac-3',
	kAudioFileAAC_ADTSType        = 'adts',
	kAudioFileMPEG4Type           = 'mp4f',
	kAudioFileM4AType             = 'm4af',
	kAudioFileCAFType             = 'caff',
	kAudioFile3GPType             = '3gpp',
	kAudioFile3GP2Type            = '3gp2',
	kAudioFileAMRType             = 'amrf'
};
typedef UInt32 AudioFileTypeID;

struct AudioFileTypeAndFormatID
{
	AudioFileTypeID mFileType;
	UInt32 mFormatID;
};

typedef OSStatus (*AudioFile_ReadProc)(void *inClientData, SInt64 inPosition, UInt32 requestCount, void *buffer, UInt32 *actualCount);
typedef SInt64 (*AudioFile_SetSizeProc)(void *inClientData);
typedef OSStatus (*AudioFile_WriteProc)(void *inClientData, SInt64 inPosition, UInt32 requestCount, const void *buffer, UInt32 *actualCount);
typedef SInt64 (*AudioFile_GetSizeProc)(void *inClientData);

class AudioFileImpl;
typedef AudioFileImpl* AudioFileID;

extern "C" {

OSStatus AudioFileOpenURL(CFURLRef inFileRef, SInt8 inPermissions, AudioFileTypeID inFileTypeHint, AudioFileID* outAudioFile);
OSStatus AudioFileOpenWithCallbacks(void* opaque, AudioFile_ReadProc inReadFunc, AudioFile_WriteProc inWriteFunc, AudioFile_GetSizeProc inGetSizeFunc, AudioFile_SetSizeProc inSetSizeFunc, AudioFileTypeID inFileTypeHint, AudioFileID* outAudioFile);
OSStatus AudioFileClose(AudioFileID inAudioFile);

}

#endif

