#ifndef AUCOMPONENT_H
#define AUCOMPONENT_H
#include <MacTypes.h>
#include <CoreServices/Components.h>
#include <CoreFoundation/CFString.h>

class AudioUnitComponent;

typedef Component AudioComponent;
typedef AudioUnitComponent* AudioComponentInstance;
typedef ComponentDescription AudioComponentDescription;

enum
{
	kAudioUnitType_Output  = 'auou',
	kAudioUnitType_MusicDevice = 'aumu',
	kAudioUnitType_MusicEffect = 'aumf',
	kAudioUnitType_FormatConverter = 'aufc',
	kAudioUnitType_Effect  = 'aufx',
	kAudioUnitType_Mixer  = 'aumx',
	kAudioUnitType_Panner  = 'aupn',
	kAudioUnitType_Generator = 'augn',
	kAudioUnitType_OfflineEffect = 'auol'
};

extern "C" {

AudioComponent AudioComponentFindNext(AudioComponent inAComponent, AudioComponentDescription *inDesc);
OSStatus AudioComponentGetDescription(AudioComponent inComponent, AudioComponentDescription  *outDesc);
OSStatus AudioComponentGetVersion(AudioComponent inComponent, UInt32 *outVersion);
Boolean AudioComponentInstanceCanDo(AudioComponentInstance inInstance, SInt16 inSelectorID);
OSStatus AudioComponentInstanceDispose(AudioComponentInstance inInstance);
AudioComponent AudioComponentInstanceGetComponent(AudioComponentInstance inInstance);
OSStatus AudioComponentInstanceNew(AudioComponent inComponent, AudioComponentInstance *outInstance);
OSStatus AudioComponentCopyName(AudioComponent inComponent, CFStringRef *outName);
UInt32 AudioComponentCount(AudioComponentDescription *inDesc);

}

#endif

